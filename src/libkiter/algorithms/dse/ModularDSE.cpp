//
// Created by toky on 24/4/23.
//

#include "ModularDSE.h"
#include "TokenConfiguration.h"
#include "Constraint.h"

#define VERBOSE_DSE_THREADS(msg) VERBOSE_CUSTOM_DEBUG ("DSE_THREAD", msg)


namespace algorithms {
    namespace dse {

        // This needs the lock
        bool ModularDSE::should_stop(size_t idle_threads, size_t explored, size_t limit) {
            if (stop_exploration) {
                return true;
            }
            else if (job_pool.empty() && in_progress.empty()) {
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
                                        std::atomic<size_t>& explored,
                                        const std::chrono::steady_clock::time_point beginTime,
                                        const ExplorationParameters& parameters) {

            models::Dataflow sandbox = *this->dataflow;
            std::unique_ptr<TokenConfiguration> current_configuration;
            while (true) {

                {
                    // Idle phase, looking for a job
                    // =============================
                    std::unique_lock<std::mutex> lock(mtx);
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " enters its loop (got the lock)");
                    ++idle_threads;
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " increased idle_threads to " << idle_threads
                                            << " and leave the lock.");

                    cv.wait_for(lock, std::chrono::milliseconds(1000),
                                [this,&idle_threads] { return stop_exploration || !job_pool.empty() || in_progress.empty(); });

                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id()
                                            << " get the lock again, wait_for satisfied stop_exploration="
                                            << (stop_exploration ? "true" : "false")
                                            << ", job_pool.size()=" << job_pool.size()
                                            << ", in_progress.size()=" << in_progress.size() << ".");
                    if (should_stop(idle_threads, explored, parameters.limit)) {
                        VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id()
                                                << " says we should stop and stop_exploration is "
                                                << (stop_exploration ? "true" : "false"));
                        break;
                    }

                    // Busy phase,   getting a job
                    // ===============================

                    --idle_threads;
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " decreased idle_threads to " << idle_threads);

                    if (job_pool.empty()) {
                        VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " found the pool empty ");
                        continue;
                    } else {
                        VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " found a configuration inside the pool.");
                    }

                    if (parameters.bottom_up) {
                        current_configuration = std::make_unique<TokenConfiguration>(job_pool.last());
                        job_pool.pop_last();
                    } else {
                        current_configuration = std::make_unique<TokenConfiguration>(job_pool.top());
                        job_pool.pop();
                    };

                    // TODO: Constraint can return more than one solution, the rest should be stocked back in the pool.
                    if (use_constraints) {
                        current_configuration = std::make_unique<TokenConfiguration>(constraints.apply(*current_configuration)[0]);
                    }

                    explored++;

                    bool stop_decision = stop_func(*current_configuration, results);
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id()
                                            << " will release the lock and start computePerformance. his decision to stop is  "
                                            << (stop_decision ? "true" : "false"));
                    if (stop_decision) {

                        ++idle_threads;
                        VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " increased idle_threads to " << idle_threads
                                                      << " and break itself.");

                        break;
                    }

                    VERBOSE_ASSERT(!in_progress.contains(*current_configuration), "A thread is already working on this same configuration!");
                    in_progress.add(*current_configuration);
                }
                // lock.unlock();

                // compute phase (perf current and find next)
                // ==========================================


                sandbox.reset_computation();
                current_configuration->computePerformance(this->performance_func, beginTime, &sandbox);
                auto [next_configurations, next_constraints] = next_func(*current_configuration);

                // VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " is done and need the lock");

                {


                    // printing add next config phase
                    // ===============================
                    std::unique_lock<std::mutex> lock(mtx);
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " locks again, computation is done, found " << next_configurations.size() << " new configurations." );

                    in_progress.remove(*current_configuration);

                    if (use_constraints) {
                        VERBOSE_DEBUG("Update constraints");
                        // VERBOSE_DEBUG("Update constraints " << constraints.toString());
                            constraints.update(next_constraints);

                    }

                    if (parameters.realtime_output) std::cout << current_configuration->to_csv_line() << std::endl;
                    results.add(*current_configuration);

                    // Keep pareto optimal only
                    VERBOSE_DEBUG("Clean the result log");
                    auto deleted = results.cleanByDominance(*current_configuration);
                    VERBOSE_DEBUG("Removed " << deleted << " items.");

                    for (const auto& next_configuration : next_configurations) {
                        if (!results.contains(next_configuration) && !in_progress.contains(next_configuration)) {
                            job_pool.push(next_configuration);
                        }
                    }
                    VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << " notify all and will release the lock again, job_pool.size()=" << job_pool.size() << ", in_progress.size()=" << in_progress.size() << ".");
                }
                cv.notify_all();
            }

            {
                std::unique_lock<std::mutex> lock(mtx);
                VERBOSE_DSE_THREADS("Thread " << std::this_thread::get_id() << "is DEAD DEAD, " << " idle_threads is " << idle_threads);
            }

        }



        void ModularDSE::explore(const ExplorationParameters& exploration_parameters) {

            VERBOSE_INFO("Start to explore");
            std::vector<std::future<void>> futures;
            std::atomic<unsigned int> idle_threads(0);
            std::atomic<size_t> explored(0);

            auto beginTime =  std::chrono::steady_clock::now()
                    - std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double, std::milli>((this->results.getTotalProcessTimeMs())));



            if (exploration_parameters.realtime_output) std::cout << TokenConfiguration::csv_header() << std::endl;

            for (unsigned int i = 0; i < exploration_parameters.thread_count; ++i) {
                futures.emplace_back(std::async(std::launch::async, &ModularDSE::explore_thread, this, std::ref(idle_threads), std::ref(explored), beginTime, exploration_parameters));
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
            for (auto c : this->in_progress) {
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
