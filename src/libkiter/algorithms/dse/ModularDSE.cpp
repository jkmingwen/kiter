//
// Created by toky on 24/4/23.
//

#include "ModularDSE.h"
#include "TokenConfiguration.h"
#include "algorithms/liveness/LivenessConstraint.h"

#define VERBOSE_DSE_THREADS(msg) VERBOSE_CUSTOM_DEBUG ("DSE_THREAD", msg)

namespace algorithms {
    namespace dse {



        // This needs the lock
        bool ModularDSE::should_stop(size_t idle_threads, size_t explored, size_t limit) {
            if (stop_exploration) {
                return true;
            }
            else if (job_pool.empty() && idle_threads == num_threads) {
                stop_exploration = true;
                return true;
            }
            else if (limit != 0 && explored >= limit) {
                stop_exploration = true;
                return true;
            }
            return false;
        }


        void ModularDSE::explore_thread(std::atomic<unsigned int>& idle_threads,
                                        std::atomic<size_t>& explored, size_t limit,
                                        const std::chrono::steady_clock::time_point beginTime,
                                        bool bottom_up,
                                        bool realtime_output) {

            models::Dataflow sandbox = *this->dataflow;
            std::unique_ptr<TokenConfiguration> current_configuration;
            while (true) {

                {
                    // Idle phase, looking for a job
                    // =============================
                    std::unique_lock<std::mutex> lock(mtx);
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " enters its loop (got the lock)");
                    ++idle_threads;
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " set idle_threads to " << idle_threads
                                            << " and leave the lock.");

                    cv.wait_for(lock, std::chrono::milliseconds(1000),
                                [this,&idle_threads] { return stop_exploration || !job_pool.empty() || idle_threads == num_threads; });

                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id()
                                            << " get the lock again, wait_for satisfied stop_exploration="
                                            << (stop_exploration ? "true" : "false") << " and job_pool.size()="
                                            << job_pool.size() << ".");
                    if (should_stop(idle_threads, explored, limit)) {
                        VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id()
                                                << " says we should stop and stop_exploration is "
                                                << (stop_exploration ? "true" : "false"));
                        break;
                    }

                    // Busy phase,   getting a job
                    // ===============================

                    --idle_threads;
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " set idle_threads to " << idle_threads);

                    if (job_pool.empty()) {
                        VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " finds the pool empty ");
                        continue;
                    }

                    if (bottom_up) {
                        current_configuration = std::make_unique<TokenConfiguration>(job_pool.last());
                        job_pool.pop_last();
                    } else {
                    current_configuration = std::make_unique<TokenConfiguration>(job_pool.top());
                    job_pool.pop();
                    };
                    explored++;

                    bool stop_decision = stop_func(*current_configuration, results);
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id()
                                            << " will release the lock and stop the good work. his decision to stop is  "
                                            << (stop_decision ? "true" : "false"));
                    if (stop_decision) return;
                }
                // lock.unlock();

                // compute phase (perf current and find next)
                // ==========================================


                sandbox.reset_computation();
                current_configuration->computePerformance(this->performance_func, beginTime, &sandbox);
                auto [next_configurations, next_constraints] = next_func(*current_configuration);

                VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " is done and need the lock");

                {
                    // printing add next config phase
                    // ===============================
                    std::unique_lock<std::mutex> lock(mtx);

                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " locks again, computation is done " );
                    if (use_constraints) {
                        if (constraints == nullptr) {
                            constraints = next_constraints;
                        } else {
                            constraints->merge(*next_constraints);
                        }
                    }

                    if (realtime_output) std::cout << current_configuration->to_csv_line() << std::endl;
                    results.add(*current_configuration);
                    for (const auto& next_configuration : next_configurations) {
                        if (!results.contains(next_configuration)) {
                            if (use_constraints) {
                                auto updated_configuration = constraints->apply(next_configuration);
                                job_pool.push(updated_configuration);
                            } else {
                                job_pool.push(next_configuration);
                            }
                        }
                    }
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " notify all and will release the lock again, job_pool.size()=" << job_pool.size() << ".");
                }
                cv.notify_all();
            }
        }



        void ModularDSE::explore(const size_t limit, bool bottom_up,
                                 bool realtime_output) {

            VERBOSE_INFO("Start to explore");
            std::vector<std::future<void>> futures;
            std::atomic<unsigned int> idle_threads(0);
            std::atomic<size_t> explored(0);

            auto beginTime =  std::chrono::steady_clock::now()
                    - std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double, std::milli>((this->results.getTotalProcessTimeMs())));



            if (realtime_output) std::cout << TokenConfiguration::csv_header() << std::endl;

            for (unsigned int i = 0; i < num_threads; ++i) {
                futures.emplace_back(std::async(std::launch::async, &ModularDSE::explore_thread, this, std::ref(idle_threads), std::ref(explored), limit, beginTime, bottom_up, realtime_output));
            }

            for (auto& future : futures) {
                future.wait();
            }
        }


        void ModularDSE::add_initial_jobs(const std::vector<TokenConfiguration>& vec) {
            std::unique_lock<std::mutex> lock(mtx);
            for (auto tc : vec)
                job_pool.push(tc);
            cv.notify_one();
        }
        void ModularDSE::add_initial_job(const TokenConfiguration& tc) {
            std::unique_lock<std::mutex> lock(mtx);
            job_pool.push(tc);
            cv.notify_one();
        }

        void ModularDSE::stop() {
            std::unique_lock<std::mutex> lock(mtx);
            stop_exploration = true;
            cv.notify_all();
        }

        std::string ModularDSE::print_space(bool no_timing) {
            std::string res = TokenConfiguration::csv_header() + "\n";
            for (auto c : this->results) {
                res += c.to_csv_line(no_timing) + "\n";
            }
            for (auto c : this->job_pool) {
                res += c.to_csv_line(no_timing) + "\n";
            }
            return res;
        }

        void ModularDSE::import_results(std::istream& input) {
            std::string line;
            std::getline(input, line); // Skip header line

            int lineNumber = 1;
            while (std::getline(input, line)) {
                lineNumber++;

                try {
                    TokenConfiguration new_config = TokenConfiguration::from_csv_line(dataflow, line);
                    if (new_config.hasPerformance()) {
                        results.add(new_config);
                    } else {
                        job_pool.add(new_config);
                    }

                } catch (const std::exception& e) {
                    VERBOSE_ERROR("Error parsing token configuration on line " << lineNumber << ": " << e.what());
                }
            }
        }

        void ModularDSE::import_results(const std::string& filename) {
            std::ifstream infile(filename);
            if (infile.is_open()) {
                import_results(infile);
            } else {
                VERBOSE_ERROR("Error opening file: " << filename);
            }
            infile.close();
        }


    } // algorithms
} // dse
