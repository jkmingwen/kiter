//
// Created by jachym on 2/4/23.
//
#define BOOST_TEST_MODULE ModelCommunicationsTest
#include "helpers/test_classes.h"
#include "helpers/sample.h"
#include "helpers/random_generator.h"
#include "algorithms/mapping/modelCommunications.h"


BOOST_FIXTURE_TEST_SUITE( model_communications_test , WITH_SAMPLE)


    // TODO: This test runs the overlap function, but we need test to trigger the overlap function to find overlaps.
    BOOST_AUTO_TEST_CASE( sample_model_communications_test )
    {
        // 1. Create a dataflow with a 4x4 NoC
        models::Dataflow* mapped_sample = generateSamplePipeline () ;
        NoC noc (4, 4);
        mapped_sample->setNoC(noc);

        BOOST_ASSERT(mapped_sample->getNoC().getNodes().size() > 0);
        int idx = 0;
        for (Vertex v : mapped_sample->vertices()) {
            for (; idx < mapped_sample->getNoC().getNodes().size(); idx++) {
                if (mapped_sample->getNoC().getNodes().at(idx).type == NetworkNodeType::Core) {
                    mapped_sample->setMapping(v, mapped_sample->getNoC().getNodes().at(idx++).id);
                    break;
                }
            }

        }
        auto overlaps = get_overlaps(mapped_sample);
        BOOST_ASSERT(overlaps.size() == 0);
        BOOST_TEST_MESSAGE("Overlaps computed");
        delete mapped_sample;
    }

BOOST_AUTO_TEST_SUITE_END()
