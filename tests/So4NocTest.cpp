//
// Created by jachym on 27/3/23.
//

#define BOOST_TEST_MODULE So4NocTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
//#include "helpers/random_generator.h"
#include "algorithms/transformation/singleOutput.h"


BOOST_FIXTURE_TEST_SUITE( so4noc_test , WITH_SAMPLE)

BOOST_AUTO_TEST_CASE( sample_so4_noc_test )
{
  //std::pair<TIME_UNIT, scheduling_t> computeComponentSo4Schedule(models::Dataflow* const dataflow,
  //                    std::pair<ARRAY_INDEX, EXEC_COUNT> &minActorInfo, scheduling_t schedule, std::string filename, std::set<ARRAY_INDEX> edges);
  //std::vector<models::Dataflow*> generateSCCs(models::Dataflow* const dataflow,
  //                                            std::map<int, std::vector<ARRAY_INDEX>> sccMap);

  //parameters_list_t params;
  //BOOST_REQUIRE(pipeline_sample);
  //params["name"] = "a";

  //algorithms::transformation::singleOutput(pipeline_sample, params);
}

BOOST_AUTO_TEST_SUITE_END()
