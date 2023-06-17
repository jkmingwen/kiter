//
// Created by jachym on 28/3/23.
//

#define BOOST_TEST_MODULE DsePeriodicTest

#include <future>
#include <thread>
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/dse/dichotomous_search.h"


BOOST_FIXTURE_TEST_SUITE( dse_periodic_test , WITH_SAMPLE)

BOOST_AUTO_TEST_CASE( sample_periodic_test )
{
    //commons::set_verbose_mode(commons::WARNING_LEVEL);
    parameters_list_t params;
    BOOST_REQUIRE(pipeline_sample);

    VERBOSE_INFO("DSE: periodic - integer solving = false");
    algorithms::compute_csdf_dse_periodic(pipeline_sample, params);

    VERBOSE_INFO("DSE: periodic - integer solving = true");
    params["ILP"] = true;
    algorithms::compute_csdf_dse_periodic(pipeline_sample, params);
}

BOOST_AUTO_TEST_CASE( sample_speriodic_test )
{
    parameters_list_t params;
    BOOST_REQUIRE(pipeline_sample);

    VERBOSE_INFO("DSE: speriodic - integer solving = false");
    algorithms::compute_csdf_dse_speriodic(pipeline_sample, params);

    VERBOSE_INFO("DSE: speriodic - integer solving = true");
    params["ILP"] = true;
    algorithms::compute_csdf_dse_speriodic(pipeline_sample, params);
}

#define MAX_PHASE_COUNT 5

BOOST_AUTO_TEST_CASE( random_periodic_test )
{
    std::vector<models::Dataflow *> graphs;
    parameters_list_t params;

    VERBOSE_INFO("Generating Graphs");
    for (int i = 10; i <= 300; i += 50) {

        int buf_num = std::rand() % (i / 2) + i;

        VERBOSE_INFO("generate graph " << i << " with " << buf_num);

        models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
        graphs.push_back(g);
    }

    VERBOSE_INFO("Running periodic");
    for(auto graph : graphs) {
        algorithms::compute_csdf_dse_periodic(graph, params);
    }
}

BOOST_AUTO_TEST_CASE( random_speriodic_test )
{
    std::vector<models::Dataflow *> graphs;
    parameters_list_t params;

    VERBOSE_INFO("Generating Graphs");
    for (int i = 10; i <= 300; i += 50) {

        int buf_num = std::rand() % (i / 2) + i;

        VERBOSE_INFO("generate graph " << i << " with " << buf_num);

        models::Dataflow *g = generate_random_graph(i, buf_num, MAX_PHASE_COUNT, 2, 1);
        graphs.push_back(g);
    }

    VERBOSE_INFO("Running speriodic");

    size_t happy_outcomes = 0;
    for(auto graph : graphs) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // This is the child process.
            algorithms::compute_csdf_dse_speriodic(graph, params);
            exit(EXIT_SUCCESS);
        } else {
            // This is the parent process.
            sleep(10); // wait for 10 second

            // Kill the child process after 10 second
            if (kill(pid, SIGKILL) == -1) {
                perror("kill");
                exit(EXIT_FAILURE);
            }

            // Wait for child process to terminate
            int status;
            waitpid(pid, &status, 0);
            if (WIFSIGNALED(status)) {
                std::cout << "Child process was killed by signal " << WTERMSIG(status) << std::endl;

            } else if (WIFEXITED(status)) {
                std::cout << "Child process exited with status " << WEXITSTATUS(status) << std::endl;
                happy_outcomes += 1;
            }

            BOOST_TEST_REQUIRE(status == 0);

        }
    }

    BOOST_TEST_REQUIRE(happy_outcomes == graphs.size());

//    for(auto graph : graphs) {
//
//        std::cout << "Create thread...";
//        std::thread exploration_thread([&graph, &params] {
//            algorithms::compute_csdf_dse_speriodic(graph, params);
//        });
//
//        // Sleep for some time
//        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//
//        std::cout << "Kill thread...";
//        // Stop the thread
//        if (exploration_thread.joinable()) {
//            pthread_cancel(exploration_thread.native_handle());
//            exploration_thread.join();
//        }
//
//        std::cout << "Thread killed...";
//
//
//
//    }
}

BOOST_AUTO_TEST_SUITE_END()
