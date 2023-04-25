//
// Created by toky on 24/4/23.
//

#include "ModularDSE.h"
#include "TokenConfiguration.h"

namespace algorithms {
    namespace dse {

        void ModularDSE::explore() {

            VERBOSE_INFO("Start to explore");
            std::vector<std::future<void>> futures;
            std::atomic<unsigned int> idle_threads(0);

            for (unsigned int i = 0; i < num_threads; ++i) {
                futures.emplace_back(std::async(std::launch::async, [this, &idle_threads] {

                    VERBOSE_INFO("Start Thread...");

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
                            --idle_threads;
                            current_configuration = std::make_unique<TokenConfiguration>(job_pool.top());
                            job_pool.pop();

                            VERBOSE_INFO("Pop " << *current_configuration << " out of " << job_pool.size() << " left.");

                        }
                        //VERBOSE_INFO("Start to compute throughput.");
                        bool stop_decision = stop_func(*current_configuration,results);
                        if (stop_decision) continue;
                        current_configuration->computeThroughput(this->performance_func);
                        VERBOSE_INFO("computeThroughput: " << current_configuration->getThroughput());
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

        std::string ModularDSE::print_space() {
            return this->results.toString();
        }


    } // algorithms
} // dse