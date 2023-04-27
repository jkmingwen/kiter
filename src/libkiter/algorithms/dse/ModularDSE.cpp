//
// Created by toky on 24/4/23.
//

#include "ModularDSE.h"
#include "TokenConfiguration.h"

namespace algorithms {
    namespace dse {

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




        void ModularDSE::explore(const size_t limit, bool realtime_output) {

            VERBOSE_INFO("Start to explore");
            std::vector<std::future<void>> futures;
            std::atomic<unsigned int> idle_threads(0);
            std::atomic<size_t> explored(0);

            if (realtime_output) std::cout << TokenConfiguration::csv_header() << std::endl;

            for (unsigned int i = 0; i < num_threads; ++i) {
                futures.emplace_back(std::async(std::launch::async, [this, &idle_threads, &explored, limit, realtime_output] {

                    VERBOSE_INFO("Init Thread...");
                    // Here, computing performance implies the need or a sandbox dataflow to make some changes
                    // each thread will have a sandbox dataflow, and will pass it to performance func.
                    models::Dataflow sandbox = *this->dataflow;

                    VERBOSE_INFO("Start Thread loop ...");
                    while (true) {
                        ++idle_threads;
                        std::unique_ptr<TokenConfiguration> current_configuration;
                        {
                            std::unique_lock<std::mutex> lock(mtx);
                            cv.wait_for(lock, std::chrono::milliseconds(100), [this] { return stop_exploration || !job_pool.empty(); });
                            if (stop_exploration) {
                                break;
                            } else if (job_pool.empty()) {
                                if (idle_threads == num_threads) {
                                    VERBOSE_INFO("We should stop...");
                                    stop_exploration = true;
                                    cv.notify_all();
                                }
                                continue;
                            }

                            if (limit != 0 and explored >= limit) {
                                stop_exploration = true;
                                continue;
                            }

                            --idle_threads;
                            current_configuration = std::make_unique<TokenConfiguration>(job_pool.top());
                            job_pool.pop();
                            VERBOSE_INFO( "Pop " << *current_configuration << " out of " << job_pool.size() << " left.");
                            explored++;
                        }
                        //VERBOSE_INFO("Start to compute throughput.");
                        bool stop_decision = stop_func(*current_configuration,results);
                        if (stop_decision) continue;

                        sandbox.reset_computation();
                        VERBOSE_ASSERT(!current_configuration->hasPerformance(), "job_pool config should not be computed.");
                        current_configuration->computePerformance(&sandbox, this->performance_func);
                        VERBOSE_ASSERT(current_configuration->hasPerformance(), "result config should be computed.");

                        VERBOSE_INFO("computePerformance: " << current_configuration->getPerformance());
                        if (realtime_output) std::cout << current_configuration->to_csv_line() << std::endl;
                        results.add(*current_configuration);
                        auto next_configurations = next_func(*current_configuration);
                        VERBOSE_INFO("Next are " << commons::toString(next_configurations));

                        {
                            std::unique_lock<std::mutex> lock(mtx);
                            for (const auto& next_configuration : next_configurations) {
                                // TODO: Need better way/datastructures to check that a config hasn't been explored before
                                // FIXME: the job_pool can be flooded by similar configs.
                                if (!results.contains(next_configuration)) {
                                    job_pool.push(next_configuration);
                                }
                            }
                            cv.notify_all();
                        }
                    }


                    VERBOSE_INFO("End Thread.");

                }));
            }

            for (auto& future : futures) {
                future.wait();
            }
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


    } // algorithms
} // dse