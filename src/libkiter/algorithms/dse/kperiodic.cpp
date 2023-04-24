/*
 * kperiodic.cpp
 *
 * Author: jkmingwen
 *
 */

#include <map>
#include <vector>
#include <string>
#include <printers/stdout.h>
#include <commons/verbose.h>
#include <printers/SDF3Wrapper.h> // to write XML files
#include <commons/commons.h>
#include <models/Dataflow.h>
#include <models/EventGraph.h>
#include <algorithms/normalization.h>
#include <models/repetition_vector.h>
#include "buffer_sizing.h"
#include "kperiodic.h"
#include <chrono> // to take computation timings
#include <boost/filesystem.hpp>

std::pair<models::Dataflow*, std::map<Edge,Edge> > genGraphWFeedbackEdgesWithPairs(models::Dataflow* g){
    /* Generate dataflow_prime */

    std::map<Edge,Edge>  matching; // <dataflow_prime -> dataflow>
    models::Dataflow* dataflow_prime = new models::Dataflow(*g);
    dataflow_prime->reset_computation();
    // add feedback channels in new graph to model bounded channel quantities
    {ForEachEdge(g, c) {

            Edge original_edge = dataflow_prime->getEdgeById(g->getEdgeId(c));

            Edge new_edge = dataflow_prime->addEdge(dataflow_prime->getEdgeTarget(c),
                                                    dataflow_prime->getEdgeSource(c));
            dataflow_prime->setEdgeInPhases(new_edge,
                                            dataflow_prime->getEdgeOutVector(c));
            dataflow_prime->setEdgeOutPhases(new_edge,
                                             dataflow_prime->getEdgeInVector(c));
            dataflow_prime->setPreload(new_edge, g->getPreload(c));
            dataflow_prime->setEdgeName(new_edge,
                                        dataflow_prime->getEdgeName(c) + "_prime");
            dataflow_prime->setEdgeType(new_edge, EDGE_TYPE::FEEDBACK_EDGE);

            matching[original_edge] = c;
            matching[new_edge] = c;

        }}

    return std::pair<models::Dataflow*, std::map<Edge,Edge>>(dataflow_prime, matching);

}


models::Dataflow* getCCGraph(models::Dataflow* g, kperiodic_result_t result, bool print){
    /* get Dataflow* graph object from critical cycle edge set */
    models::Dataflow* cc_g = new models::Dataflow(); /***@@@*****/

    std::set<std::string> visited_v;

    // VERBOSE_INFO("Critical edges found:");
    for (Edge e_g : result.critical_edges){
        
        Vertex src_g = g->getEdgeSource(e_g);
        Vertex trg_g = g->getEdgeTarget(e_g);
        std::string src = g->getVertexName(src_g);
        std::string trg = g->getVertexName(trg_g);
        
        Vertex src_cc;
        Vertex trg_cc; 
        if (visited_v.find(src) == std::end(visited_v)){
            src_cc = cc_g->addVertex(g->getVertexName(src_g)); // Adds ID as name
            visited_v.insert(src);
        } else {
            src_cc = cc_g->getVertexByName(src);
        }
        if (visited_v.find(trg) == std::end(visited_v)){
            trg_cc = cc_g->addVertex(g->getVertexName(trg_g));
            visited_v.insert(trg);
        } else {
            trg_cc = cc_g->getVertexByName(trg);
        }

        Edge e_cc = cc_g->addEdge(src_cc, trg_cc, g->getEdgeName(e_g));
        cc_g->setPreload(e_cc, g->getPreload(e_g)); 
        cc_g->setEdgeInPhases(e_cc, g->getEdgeInVector(e_g));
        cc_g->setEdgeOutPhases(e_cc, g->getEdgeOutVector(e_g));
        cc_g->setReentrancyFactor(src_cc, g->getReentrancyFactor(src_g));
        cc_g->setVertexDuration(src_cc, g->getVertexPhaseDuration(src_g));
        cc_g->setReentrancyFactor(trg_cc, g->getReentrancyFactor(trg_g));
        cc_g->setVertexDuration(trg_cc, g->getVertexPhaseDuration(trg_g));

        if (print){
          VERBOSE_INFO("EdgeID: " << cc_g->getEdgeId(e_cc) 
                          << " | EdgeName: " << cc_g->getEdgeName(e_cc) 
                          << " | ChnQnt: " << cc_g->getPreload(e_cc));     
          VERBOSE_INFO("\tEdge(In)Phases: " << commons::toString(cc_g->getEdgeInVector(e_cc)));
          VERBOSE_INFO("\tEdge(Out)Phases: " << commons::toString(cc_g->getEdgeOutVector(e_cc)));
          VERBOSE_INFO("\tSrc(Name): " << cc_g->getVertexName(src_cc) 
                      << " -> Trg(Name): " << cc_g->getVertexName(trg_cc)); 
          VERBOSE_INFO("\tPhaseDurations(" << cc_g->getVertexName(src_cc) 
                          << "): " << commons::toString(cc_g->getVertexPhaseDuration(src_cc)));
          VERBOSE_INFO("\tPhaseDurations(" << cc_g->getVertexName(trg_cc) 
                          << "): " << commons::toString(cc_g->getVertexPhaseDuration(trg_cc)));
        }

    }

    return cc_g;

}


StorageDistribution updateStoreDistwCCSD(models::Dataflow* dataflow_prime, StorageDistribution sd_g,
                                    models::Dataflow* cc_g, StorageDistribution sd_cc){
    /* Update storage distribution with new critical cycle channel quantities */

    StorageDistribution sd(sd_g); // making copy to update

    for (Edge e_cc : sd_cc.getEdges()){

        std::string name = cc_g->getEdgeName(e_cc);    

        if (cc_g->getEdgeType(e_cc) == FEEDBACK_EDGE) {
            sd.setChannelQuantity(dataflow_prime->getEdgeByName(name), 
                                  sd_cc.getChannelQuantity(e_cc));
        }
    }

    return sd;
}

StorageDistribution updateStoreDistwStepSz(Edge e, models::Dataflow* dataflow_prime, 
                                            StorageDistribution checkDist, std::map<Edge, TOKEN_UNIT> minStepSizes){
  StorageDistribution newDist(checkDist);
  // only increase channel quantity on "modelled" channels
  VERBOSE_DSE("\tFound storage dependency in channel "
              << dataflow_prime->getEdgeName(e));
  // make new modelled storage distribution according to storage dependencies
  newDist.setChannelQuantity(e, (newDist.getChannelQuantity(e) +
                                    minStepSizes[e]));
  VERBOSE_DSE("\t\tIncreasing channel size of "
              << dataflow_prime->getEdgeName(e) << " to "
              << newDist.getChannelQuantity(e));
  VERBOSE_DSE("\tUpdating checklist with new storage distribution..."
             );
  return newDist;  
}

// Compute and return period and causal dependency cycles of given dataflow graph
kperiodic_result_t algorithms::compute_Kperiodic_throughput_and_cycles(models::Dataflow* const dataflow) {

    VERBOSE_ASSERT(dataflow->getVerticesCount() > 0, "Empty graph not supported");
    VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");

    EXEC_COUNT sumNi = 0;
    EXEC_COUNT sumKi = dataflow->getVerticesCount();

    {ForEachTask(dataflow,t) { sumNi += dataflow->getNi(t) ; }}

    // STEP 0.1 - PRE
    VERBOSE_ASSERT(dataflow,TXT_NEVER_HAPPEND);
    VERBOSE_ASSERT(computeRepetitionVector(dataflow),"inconsistent graph");
    EXEC_COUNT iteration_count = 0;

    // STEP 1 - generate initial vector
    std::map<Vertex,EXEC_COUNT> kvector;
    {ForEachVertex(dataflow,t) { kvector[t] = 1; }}

    kperiodic_result_t result;

    VERBOSE_DEBUG("KPeriodic EventGraph generation");

    //STEP 1 - Generate Event Graph
    models::EventGraph* eg = generateKPeriodicEventGraph(dataflow,&kvector);

    VERBOSE_DEBUG("KPeriodic EventGraph generation Done");

    //STEP 2 - resolve the MCRP on this Event Graph
    std::pair<TIME_UNIT,std::vector<models::EventGraphEdge> > howard_res = eg->MinCycleRatio();
    std::vector<models::EventGraphEdge> * critical_circuit = &(howard_res.second);

    //STEP 3 - convert CC(eg) => CC(graph)
    VERBOSE_DEBUG("Critical circuit is about " << critical_circuit->size() << " edges.");
    for (std::vector<models::EventGraphEdge>::iterator it = critical_circuit->begin() ; it != critical_circuit->end() ; it++ ) {
        VERBOSE_DEBUG("   -> " << eg->getChannelId(*it) << " : " << eg->getSchedulingEvent(eg->getSource(*it)).toString() << " to " <<  eg->getSchedulingEvent(eg->getTarget(*it)).toString() <<  " = (" << eg->getConstraint(*it)._w << "," << eg->getConstraint(*it)._d << ")" );
        ARRAY_INDEX channel_id = eg->getChannelId(*it);
        try {
            Edge        channel    = dataflow->getEdgeById(channel_id);
            result.critical_edges.insert(channel);
        } catch(...) {
            VERBOSE_DEBUG("      is loopback");
        }
    }

    TIME_UNIT frequency = howard_res.first;

    VERBOSE_DEBUG("KSchedule function get " << frequency << " from MCRP." );
    VERBOSE_DEBUG("  ->  then omega =  " <<  1 / frequency );

    result.throughput = frequency;

    ////////////// SCHEDULE CALL // END
    if (result.critical_edges.size() != 0) {

        VERBOSE_DEBUG("1-periodic throughput (" << result.throughput <<  ") is not enough.");
        VERBOSE_DEBUG("   Critical circuit is " << cc2string(dataflow,&(result.critical_edges)) <<  "");

        while (true) {

            iteration_count++;
            ////////////// SCHEDULE CALL // BEGIN : resultprime = KSchedule(dataflow,&kvector);

            kperiodic_result_t resultprime;

            //VERBOSE_ASSERT( algorithms::normalize(dataflow),"inconsistent graph");
            VERBOSE_DEBUG("KPeriodic EventGraph generation");

            //STEP 1 - Generate Event Graph and update vector
            if (!updateEventGraph( dataflow ,  &kvector, &(result.critical_edges), eg)) break ;

            VERBOSE_DEBUG("KPeriodic EventGraph generation Done");

            //STEP 2 - resolve the MCRP on this Event Graph
            std::pair<TIME_UNIT,std::vector<models::EventGraphEdge> > howard_res_bis = eg->MinCycleRatio();

            std::vector<models::EventGraphEdge> * critical_circuit = &(howard_res_bis.second);

            //STEP 3 - convert CC(eg) => CC(graph)
            VERBOSE_DEBUG("Critical circuit is about " << critical_circuit->size() << " edges.");
            for (std::vector<models::EventGraphEdge>::iterator it = critical_circuit->begin() ; it != critical_circuit->end() ; it++ ) {
                VERBOSE_DEBUG("   -> " << eg->getChannelId(*it) << " : " << eg->getSchedulingEvent(eg->getSource(*it)).toString() << " to " <<  eg->getSchedulingEvent(eg->getTarget(*it)).toString() <<  " = (" << eg->getConstraint(*it)._w << "," << eg->getConstraint(*it)._d << ")" );
                ARRAY_INDEX channel_id = eg->getChannelId(*it);
                try {
                    Edge        channel    = dataflow->getEdgeById(channel_id);
                    resultprime.critical_edges.insert(channel);
                } catch(...) {
                    VERBOSE_DEBUG("      is loopback");
                }
            }

            TIME_UNIT frequency = howard_res_bis.first;

            VERBOSE_DEBUG("KSchedule function get " << frequency << " from MCRP." );
            VERBOSE_DEBUG("  ->  then omega =  " <<  1 / frequency );

            resultprime.throughput = frequency;

            ////////////// SCHEDULE CALL // END
            if (sameset(dataflow,&(resultprime.critical_edges),&(result.critical_edges)))  {
                VERBOSE_DEBUG("Critical circuit is the same");
                result = resultprime;

                break;
            }
            result = resultprime;
            VERBOSE_DEBUG("Current K-periodic throughput (" << result.throughput <<  ") is not enough.");
            VERBOSE_DEBUG("   Critical circuit is " << cc2string(dataflow,&(result.critical_edges)) <<  "");

        }
    }


    VERBOSE_DEBUG( "K-periodic schedule: iterations count is " << iteration_count << "  final size is " << eg->getEventCount() << " events and " << eg->getConstraintsCount() << " constraints.");
    delete eg;

    EXEC_COUNT total_ki = 0;
    {ForEachVertex(dataflow,t) { total_ki += kvector[t]; }}

    VERBOSE_DEBUG("K-periodic schedule: total_ki=" << sumKi << " total_ni=" << sumNi );

    return result;
}

StorageDistribution initialiseDist(models::Dataflow* dataflow){


  std::map<Edge, BufferInfos> minChnSz;
  findMinimumChannelSz(dataflow,minChnSz);
  StorageDistribution initDist(dataflow, 0, minChnSz);
  return initDist;

}

void updateGraphwMatching(models::Dataflow* dataflow, const std::map<Edge,Edge>& matching, StorageDistribution checkDist){
    dataflow->reset_computation();
    {ForEachEdge(dataflow, c) {
            Edge original_edge = matching.at(c);
            if (dataflow->getEdgeType(c) == FEEDBACK_EDGE) {
                dataflow->setPreload(c, checkDist.getChannelQuantity(original_edge) -
                                              checkDist.getInitialTokens(original_edge));
            } else {
                //dataflow_prime->setPreload(c, checkDist.getInitialTokens(matching.at(c)));

                VERBOSE_ASSERT_EQUALS(checkDist.getInitialTokens(original_edge), dataflow->getPreload(c));
            }
    }}

}

/**
 * Given a particular distribution and a template of the graph with feedback_edges
 *  1) Modify the template with the correct preload
 *  2) compute the throughput
 *  3) look for future distributions
 *
 * @param checkDist
 * @param dataflow_prime
 * @return
 */
std::pair<TIME_UNIT, std::vector<StorageDistribution>> get_next_storage_distribution_from_cc(StorageDistribution checkDist, models::Dataflow *dataflow_prime,
                                            const std::map<Edge,Edge>& matching, const std::map<Edge, TOKEN_UNIT> & minStepSizes) {

    std::vector<StorageDistribution> new_distributions;

    // Update graph with storage distribution just removed from checklist
    updateGraphwMatching(dataflow_prime, matching, checkDist);

    // Compute throughput and storage deps
    VERBOSE_DEBUG_DSE("Compute throughput and storage deps");
    kperiodic_result_t result = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow_prime);

    // Create new storage distributions for every storage dependency found; add new storage distributions to checklist
    VERBOSE_DEBUG_DSE(" Create new storage distributions");
    for (Edge c : result.critical_edges) {
        VERBOSE_DEBUG_DSE(" - Critical Edge " << dataflow_prime->getEdgeName(c));
        if (dataflow_prime->getEdgeType(c) == FEEDBACK_EDGE) {
            VERBOSE_DEBUG_DSE("   is interesting ");
            Edge original_edge = matching.at(c);
            StorageDistribution newDist(checkDist);
            newDist.setChannelQuantity(original_edge, (newDist.getChannelQuantity(original_edge) + minStepSizes.at(original_edge)));
            new_distributions.push_back(newDist);
        }
    }

    return std::pair<TIME_UNIT, std::vector<StorageDistribution>> (result.throughput, new_distributions);
}



StorageDistributionSet algorithms::new_compute_Kperiodic_dse_with_init_dist(models::Dataflow *const dataflow,
                                                                StorageDistribution initDist,
                                                                TIME_UNIT target_thr) {

    // Compute the minimal steps
    std::map<Edge, TOKEN_UNIT> minStepSizes;
    findMinimumStepSz(dataflow, minStepSizes);

    // Compute the throughput target
    kperiodic_result_t result_max = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow);
    
    TIME_UNIT thrTarget = (0 < target_thr) ? target_thr : result_max.throughput;
    VERBOSE_DSE("Max Throughput: " << result_max.throughput << " | Target Throughput: " << thrTarget);


    // Produce the feedback buffers
    std::pair<models::Dataflow*, std::map<Edge,Edge> > dataflow_and_matching = genGraphWFeedbackEdgesWithPairs(dataflow);
    models::Dataflow* dataflow_prime = dataflow_and_matching.first;
    std::map<Edge,Edge>& matching = dataflow_and_matching.second;

    // Prepare the checklist of distribution to explore
    StorageDistributionSet checklist = StorageDistributionSet(initDist);

    // Start the exploration loop
    StorageDistributionSet minStorageDist;

    // Add a storage distribution full of zeros to avoid comparison problems
    StorageDistribution zeroDist (dataflow);
    {ForEachEdge(dataflow, c) {
        zeroDist.setChannelQuantity(c, 0);
    }}
    minStorageDist.addStorageDistribution(zeroDist);

    while (!minStorageDist.isSearchComplete(checklist, thrTarget)) {

        // Pop checkDist
        StorageDistribution checkDist(checklist.getNextDistribution());
        checklist.removeStorageDistribution(checklist.getNextDistribution());
        VERBOSE_DSE("Pop SD = " << checkDist.getQuantitiesStr());

        // Compute the next distribution to explore
        std::pair<TIME_UNIT, std::vector<StorageDistribution>> new_points = get_next_storage_distribution_from_cc(checkDist, dataflow_prime, matching, minStepSizes);
        checklist.addStorageDistributions(new_points.second);
        checkDist.setThroughput(new_points.first);
        minStorageDist.addStorageDistribution(checkDist);
        /*NOTE: check if pareto? minStorageDist should only hold pareto
          also, checklist storagedistributions incorrect? */

        // clean up distributions
        minStorageDist.minimizeStorageDistributions(checkDist);


    }

    // The minimum storage distribution for a throughput of 0 is (0, 0,..., 0)
//    if (minStorageDist.getNextDistribution().getThroughput() == 0) {
//        StorageDistribution zeroDist(minStorageDist.getNextDistribution());
//        {ForEachEdge(dataflow_prime, c) { zeroDist.setChannelQuantity(c, 0); }}
//        minStorageDist.removeStorageDistribution(minStorageDist.getNextDistribution());
//        minStorageDist.addStorageDistribution(zeroDist);
//    }


    return minStorageDist;

}


StorageDistributionSet algorithms::new_compute_Kperiodic_dse(models::Dataflow *const dataflow) {
    /* NOTE: initDist copies and cleans code that was here before into a function, but
      important to note that it is instantiated with dataflow and thus no feedback edges are
      modelled (unless this is intrinsic to StorageDistribution, but then this begs the
      question of why are we instantiating later ones with feedback edges).
      Jaime's code initialises initDist with dataflow_prime. */
    return new_compute_Kperiodic_dse_with_init_dist(dataflow, initialiseDist(dataflow));
}



/***********************************************************************************/

void algorithms::mod_Kperiodic_throughput_dse (models::Dataflow* const dataflow,
                                                   parameters_list_t  params) {

  std::string dirName = (params.find("LOGDIR") != params.end()) ? params["LOGDIR"] : "./data/";
  std::string ppDirName = dirName + "/pp_logs/"; // logs of pareto points
  std::string logDirName = dirName + "/dse_logs/";
	boost::filesystem::create_directory(ppDirName);
	boost::filesystem::create_directory(logDirName);
  // initialise data logging file
  std::ofstream dseLog;
  dseLog.open(logDirName + dataflow->getGraphName() + "_dselog" + "_kiter" + ".csv");
  dseLog << "storage distribution size,throughput,channel quantities,dependency mask,computation duration,cumulative duration" << std::endl;
  std::chrono::duration<double, std::milli> cumulativeTime;


  StorageDistribution initDist = initialiseDist(dataflow);

  // Compute the minimal steps
  std::map<Edge, TOKEN_UNIT> minStepSizes;
  findMinimumStepSz(dataflow, minStepSizes);

  // Compute the throughput target
  kperiodic_result_t result_max = algorithms::compute_Kperiodic_throughput_and_cycles(dataflow);
  
  TIME_UNIT thrTarget = result_max.throughput;
  VERBOSE_DSE("Target Throughput: " << thrTarget);

  // Produce the feedback buffers
  std::pair<models::Dataflow*, std::map<Edge,Edge> > dataflow_and_matching = genGraphWFeedbackEdgesWithPairs(dataflow);
  models::Dataflow* dataflow_prime = dataflow_and_matching.first;
  std::map<Edge,Edge>& matching = dataflow_and_matching.second;

  kperiodic_result_t result = compute_Kperiodic_throughput_and_cycles(dataflow_prime);
  initDist.setThroughput(result.throughput);

  StorageDistributionSet checklist = StorageDistributionSet(initDist);
  StorageDistributionSet minStorageDist; /* dse pareto results */


  // Start DSE search
  VERBOSE_DSE("DSE BEGIN:");
  while (!minStorageDist.isSearchComplete(checklist, thrTarget)) {

    VERBOSE_DSE("Checking next distribution (checklist size=" << checklist.getSize() << ")\n");
    StorageDistribution checkDist(checklist.getNextDistribution()); // copy distribution for checking (first in checklist)
    checklist.removeStorageDistribution(checklist.getNextDistribution()); // remove said storage distribution from checklist
    
    // Update dataflow_prime
    updateGraphwMatching(dataflow_prime, matching, checkDist);

    // Compute throughput and storage deps
    auto startTime = std::chrono::steady_clock::now();
    result = compute_Kperiodic_throughput_and_cycles(dataflow_prime); // @@@@
    auto endTime = std::chrono::steady_clock::now();
    auto execTime = endTime - startTime; // duration in ms
    cumulativeTime += execTime;

    checkDist.setThroughput(result.throughput);
    
    minStorageDist.addStorageDistribution(checkDist);

    // write current storage distribution info to DSE log
    dseLog << checkDist.getDistributionSize() << ","
            << checkDist.getThroughput() << ","
            << checkDist.print_quantities_csv() << ","
            << checkDist.print_dependency_mask( result) << ","
            << execTime.count() << ","
            << cumulativeTime.count() << std::endl;
   

    /* MODIFICATIONS
    1. Generate new dataflow of critical cycle
    2. Compute new buffer sizings for cc which exhibits local improvement
        2.1. Find current throughput given buffer constraints
        2.2. Set throughput target to be next after (or thrTarget)
        2.3. Get StorageDistributionSet of all SDs which exhibit this marginal improvement
    3. For each SD, update current graph SD with new Channel quantities from 2
    4. Add new StorageDist to search space
    */
    models::Dataflow* cc_g = getCCGraph(dataflow, result, true); //1
    kperiodic_result_t  cc_res = compute_Kperiodic_throughput_and_cycles(cc_g); //2.1
    TIME_UNIT nxt_thr = std::min(thrTarget, cc_res.throughput+0.000001); //2.2
    StorageDistributionSet cc_new_sd = new_compute_Kperiodic_dse_with_init_dist(cc_g, checkDist, nxt_thr); //2.3
    
    for (std::pair<TOKEN_UNIT, std::vector<StorageDistribution>> cc_pair : cc_new_sd.getSet()){
      for (StorageDistribution cc_sd : cc_pair.second){
        if (cc_sd.getThroughput() >= nxt_thr){ //NOTE: Can be source of bugs!
          StorageDistribution g_sd = updateStoreDistwCCSD(dataflow_prime, checkDist, cc_g, cc_sd); //3
          checklist.addStorageDistribution(g_sd); //4
        }
      }
    }
    
    minStorageDist.minimizeStorageDistributions(checkDist);
  
  }


  // The minimum storage distribution for a throughput of 0 is (0, 0,..., 0)
  if (minStorageDist.getNextDistribution().getThroughput() == 0) {
    StorageDistribution zeroDist(minStorageDist.getNextDistribution());
    {ForEachEdge(dataflow_prime, c) { zeroDist.setChannelQuantity(c, 0); }}
    minStorageDist.removeStorageDistribution(minStorageDist.getNextDistribution());
    minStorageDist.addStorageDistribution(zeroDist);
  }

  VERBOSE_DSE("\nDSE RESULTS [START] (Target Throughput: " << thrTarget << "):");
  VERBOSE_DSE("\n" << minStorageDist.printDistributions());
  VERBOSE_DSE("DSE RESULTS [END]");
  VERBOSE_DSE("Number of pareto points: " << minStorageDist.getSize());
  dseLog.close();

}



/*************************************************************************************/

StorageDistributionSet algorithms::compute_Kperiodic_throughput_dse_sd (models::Dataflow* const dataflow,
                                                                        parameters_list_t  parameters) {

    auto beginTime = std::chrono::steady_clock::now();

    bool writeLogFiles = false;
    bool thrTargetSpecified = false;
    bool isMaxSet = false;
    bool initializedWFeedback = false;
    std::string dirName = "./data/"; // default

    // parse parameters for KDSE
    if (parameters.find("LOGDIR") != parameters.end()) { // log output of DSE (includes pareto points and all search points)
        writeLogFiles = true;
        dirName = parameters["LOGDIR"];
    }

    // parse parameters for KDSE
    // TODO : I would remove this now, only using LOGDIR
    if (parameters.find("LOG") != parameters.end()) { // log output of DSE (includes pareto points and all search points) NOTE: parent directories need to be created beforehand
        writeLogFiles = true;
    }

    if (parameters.find("THR") != parameters.end()) { // specify target throughput of DSE
        thrTargetSpecified = true;
    } else {
        VERBOSE_INFO("No target throughput specified (target throughput will be set to max throughput by default) --- specify target throughput with '-p THR=n' flag");
    }
    if (parameters.find("MAX_SET") != parameters.end()) { // specify if only maximal SDs returned
        isMaxSet = true;
    }
    if (parameters.find("FEEDBACK") != parameters.end()) { // specify if only maximal SDs returned
        initializedWFeedback = true;
    }



    TIME_UNIT thrTarget;
    long int computation_counter = 0;
    std::string ppDirName = dirName + "/pp_logs/"; // logs of pareto points
    std::string logDirName = dirName + "/dse_logs/";
    std::string debugXMLName = dirName + "/xmls/";

    if (writeLogFiles) {
        VERBOSE_ASSERT(boost::filesystem::is_directory(dirName), "Please create the log directory " << dirName << " or specify a different one with the LOGDIR parameter.");  // true, directory exists
        boost::filesystem::create_directory(ppDirName);
        boost::filesystem::create_directory(logDirName);
        // boost::filesystem::create_directory(debugXMLName);
    }


    std::string methodName;
#ifdef WRITE_GRAPHS
    std::string dotfileDirName = dirName +  "/dotfiles/";
    boost::filesystem::create_directory(dotfileDirName);
#endif

    // create new graph with modelled bounded channel quantities
    models::Dataflow* dataflow_prime;
    if (!initializedWFeedback){
        auto dataflow_and_matching = genGraphWFeedbackEdgesWithPairs(dataflow);
        dataflow_prime = dataflow_and_matching.first;
    } else {
        dataflow_prime = new models::Dataflow(*dataflow);
        dataflow_prime->reset_computation();
    }

    // initialise search parameters
    std::map<Edge, TOKEN_UNIT> minStepSizes;
    std::map<Edge, BufferInfos> minChannelSizes;


    VERBOSE_DSE("INITIALISING SEARCH PARAMETERS:");
    initSearchParameters(dataflow_prime,
                         minStepSizes,
                         minChannelSizes);

    /* Setting lower bound channel quantities:
       In order to model bounded channel sizes, we make an edge
       that feeds back from the destination node to the source node.
       The capacity of a channel is thus modelled by:
       - INITIAL TOKENS, for original edges and
       - CHANNEL QUANTITY - INITIAL TOKENS, for feedback edges
       where the first element in the pair of minChannelSizes is the
       initial tokens in the channel while the second element is the
       capacity of the channel (in tokens). */
    {ForEachEdge(dataflow_prime, c) {
            if (dataflow_prime->getEdgeId(c) <= dataflow->getEdgesCount()) {
                minChannelSizes[c].buffer_size = 0; // original edges must have capacity 0
            }
            minChannelSizes[c].preload = dataflow_prime->getPreload(c);
        }}


    // initialise and store initial storage distribution state
    StorageDistribution initDist(dataflow_prime,
                                 0,
                                 minChannelSizes);

    // initialise modelled graph with lower bound distribution
    {ForEachEdge(dataflow_prime, c) {
            if (dataflow_prime->getEdgeId(c) <= dataflow->getEdgesCount()) { // original channel IDs
                dataflow_prime->setPreload(c, initDist.getInitialTokens(c));
            } else {
                // subtract initial tokens from buffer size to model any initial tokens in buffer
                dataflow_prime->setPreload(c, (initDist.getChannelQuantity(c) -
                                               initDist.getInitialTokens(c)));
            }
        }}

    // uncomment code block to get XMLs of lower bound distribution
    // printers::writeSDF3File(dirName + "dse_min_distribution_" +
    //                        dataflow_prime->getGraphName() + "_kiter"
    //                        ".xml", dataflow_prime);

    // calculate max throughput and current throughput with lower bound distribution
    kperiodic_result_t result_max = compute_Kperiodic_throughput_and_cycles(dataflow);

    VERBOSE_DSE("Max throughput: " << result_max.throughput);
    if (!thrTargetSpecified) {
        thrTarget = result_max.throughput;
        VERBOSE_DSE("Target throughput set to max of " << thrTarget);
    } else { // target throughput specified
        thrTarget = std::stold(parameters.find("THR")->second);
        if (thrTarget <= result_max.throughput) {
            VERBOSE_DSE("Target throughput set to " << thrTarget);
        } else { // invalid target throughput
            //std::cerr << "ERROR: Specified target throughput (" << thrTarget
            //          << ") is larger than maximum throughput (" << result_max.throughput
            //          << ")" << std::endl;

            StorageDistributionSet temp; // NOTE just a workaround for now
            //return temp; // TODO need to properly define storage distribution set here
        }
    }
    kperiodic_result_t result = compute_Kperiodic_throughput_and_cycles(dataflow_prime);

    /* a negative throughput indicates a deadlocked graph and so
       we set it to 0 to avoid bugs from having a negative throughput */
    if (result.throughput < 0) {
        initDist.setThroughput(0);
    } else { // set throughput to one computed from initial distribution otherwise
        initDist.setThroughput(result.throughput);
    }

    // add initial distribution to list of storage distributions
    StorageDistributionSet checklist;
    std::chrono::duration<double, std::milli> cumulativeTime; // store timings
      {
        methodName = "_kiter";
        checklist = StorageDistributionSet(initDist);
    }


    /* Initialise set of minimal storage distributions
       (a set of pairs of throughput and storage distribution)
       --- this will store the results of our DSE */
    StorageDistributionSet minStorageDist;
    StorageDistributionSet maxStorageDist; // for SDs with thr>=targetThr

    // initialise data logging file
    std::ofstream dseLog;
    if (writeLogFiles) {
        dseLog.open(logDirName + dataflow_prime->getGraphName() + "_dselog" + methodName + ".csv");
        dseLog << "storage distribution size,throughput,channel quantities,dependency mask,computation duration,cumulative duration"
               << std::endl; // initialise headers
    } else {
        std::cout << "storage distribution size,throughput,channel quantities,computation duration,cumulative duration"
                  << std::endl; // initialise headers
    }

    // Start search algorithm
    VERBOSE_DSE("\n");
    VERBOSE_DSE("DSE BEGIN:");

    while (!minStorageDist.isSearchComplete(checklist, thrTarget)) {
        StorageDistribution checkDist(checklist.getNextDistribution()); // copy distribution for checking (first in checklist)
        checklist.removeStorageDistribution(checklist.getNextDistribution()); // remove said storage distribution from checklist
        VERBOSE_DSE("Pop SD = " << checkDist.getQuantitiesStr());

        // Update graph with storage distribution just removed from checklist
        dataflow_prime->reset_computation(); // make graph writeable to alter channel size
        {ForEachEdge(dataflow_prime, c) {
                if (dataflow_prime->getEdgeId(c) > dataflow->getEdgesCount()) { // only modelled buffer preloads change
                    dataflow_prime->setPreload(c, (checkDist.getChannelQuantity(c) -
                                                   checkDist.getInitialTokens(c))); // always account for initial tokens in buffer
                }
            }}
        // UNCOMMENT TO WRITE XMLs OF EXPLORED GRAPHS
        // commons::writeSDF3File(debugXMLName + "dse_min_distribution_" +
        // 			   dataflow_prime->getGraphName() + "_kiter" + std::to_string(computation_counter) +
        // 			   ".xml", dataflow_prime);

        // Compute throughput and storage deps
        auto startTime = std::chrono::steady_clock::now();
        result = compute_Kperiodic_throughput_and_cycles(dataflow_prime);
        auto endTime = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::milli> execTime = endTime - startTime; // duration in ms
        std::chrono::duration<double, std::milli> cumulTime = endTime - beginTime; // duration in ms

        computation_counter++;


        checkDist.setThroughput(result.throughput);
        checkDist.setExecutionTime(execTime.count());
        checkDist.setCumulativeTime(cumulTime.count());
        // write current storage distribution info to DSE log
        if (writeLogFiles) {
            dseLog << checkDist.get_csv_line() << std::endl;
        } else {
            std::cout << checkDist.get_csv_line() << std::endl;
        }

        // Add storage distribution and computed throughput to set of minimal storage distributions
        minStorageDist.addStorageDistribution(checkDist);
        if (result.throughput >= thrTarget){
            maxStorageDist.addStorageDistribution(checkDist);
        }

        // Create new storage distributions for every storage dependency found; add new storage distributions to checklist
        for (std::set<Edge>::iterator it = (result.critical_edges).begin();
             it != (result.critical_edges).end(); it++) {
            if (dataflow_prime->getEdgeType(*it) == FEEDBACK_EDGE){
                StorageDistribution newDist(checkDist);
                // only increase channel quantity on "modelled" channels
                VERBOSE_DEBUG_DSE("\tFound storage dependency in channel "<< dataflow_prime->getEdgeName(*it));
                // make new modelled storage distribution according to storage dependencies
                newDist.setChannelQuantity(*it, (newDist.getChannelQuantity(*it) +
                                                  minStepSizes[*it]));
                VERBOSE_DEBUG_DSE("\t\tIncreasing channel size of "<< dataflow_prime->getEdgeName(*it) << " to "<< newDist.getChannelQuantity(*it));
                VERBOSE_DEBUG_DSE("\tUpdating checklist with new storage distribution...");
                checklist.addStorageDistribution(newDist);
            }
        }

        // Ensure minimal set is indeed minimal
        minStorageDist.minimizeStorageDistributions(checkDist);
        maxStorageDist.minimizeStorageDistributions(checkDist);
    }

    // The minimum storage distribution for a throughput of 0 is (0, 0,..., 0)
    if (minStorageDist.getNextDistribution().getThroughput() == 0) {
        StorageDistribution zeroDist(minStorageDist.getNextDistribution());
        {ForEachEdge(dataflow_prime, c) {
                zeroDist.setChannelQuantity(c, 0);
            }}
        minStorageDist.removeStorageDistribution(minStorageDist.getNextDistribution());
        minStorageDist.addStorageDistribution(zeroDist);
    }

    if (parameters.count("output"))
        minStorageDist.writeCSV(parameters["output"]);

    VERBOSE_DEBUG_DSE("DSE RESULTS [START] (target throughput: " << thrTarget   << "):");
    VERBOSE_DEBUG_DSE("\n" << minStorageDist.printDistributions());
    VERBOSE_DEBUG_DSE("DSE RESULTS [END]");
    VERBOSE_DEBUG_DSE("Number of computations: " << computation_counter);
    VERBOSE_DEBUG_DSE("Number of pareto points: " << minStorageDist.getSize());

    // Write log files and print file paths
    if (writeLogFiles) {
        dseLog.close();
        std::cout << "\nDSE log has been written to: "
                  << logDirName + dataflow_prime->getGraphName() + "_dselog" + methodName + ".csv"
                  << std::endl;
        minStorageDist.writeCSV(ppDirName + dataflow_prime->getGraphName() +
                                "_pp" + methodName + ".csv" );
        std::cout << "\nPareto points have been written to: "
                  << ppDirName + dataflow_prime->getGraphName() + "_pp" + methodName + ".csv"
                  << std::endl;

#ifdef WRITE_GRAPHS
        minStorageDist.printGraphs(dataflow_prime,
                               dirName + dotfileDirName);
    std::cout << "\n" + std::to_string(minStorageDist.getSize()) + " "
              << "graphs of minimal storage distributions written to: "
              << dirName + dotfileDirName + dataflow_prime->getGraphName() + "_n.dot"
              << std::endl;
#endif
    } else {
        VERBOSE_INFO("Note that you can use flag '-p LOG=true' or '-p LOGDIR=/whereyougo/' to write logs of DSE");
    }

    VERBOSE_DEBUG_DSE("End of The DSE function.");

    if (isMaxSet) {
        return maxStorageDist; // only return SDs with thr >= targetThr
    } else {
        return minStorageDist;
    }
}


void algorithms::compute_Kperiodic_throughput_dse (models::Dataflow* const dataflow,
                                                   parameters_list_t  parameters) {

    StorageDistributionSet sd = compute_Kperiodic_throughput_dse_sd(dataflow, parameters);
    
  }




