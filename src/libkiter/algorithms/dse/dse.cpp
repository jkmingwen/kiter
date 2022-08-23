/*
 * dse.cpp
 *
 * Created on: Oct 27, 2021
 * Author: jkmingwen
 *
 */

#include <map>
#include <vector>
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
#include "monotonic_optimisation.h"
#include "base_monotonic_optimisation.h"
#include <chrono> // to take computation timings
#include <boost/filesystem.hpp>
#include "dse.h"

// #define WRITE_GRAPHS // uncomment to write dot files of explored graphs

void algorithms::throughput_buffering_tradeoff_dse(models::Dataflow* const dataflow,
                                                   parameters_list_t parameters) {
  bool writeLogFiles = false;
  bool isMonoOpt = false;
  bool isBaseMonoOpt = false;
  bool thrTargetSpecified = false;
  bool modelBoundedBuffers = true;
  bool useCoarse = false;
  int coarseMultiplier = 2;
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
  if (parameters.find("M_OPT") != parameters.end()) { // use monotonic optimisation
    isMonoOpt = true;
  }
  if (parameters.find("B_M_OPT") != parameters.end()) { // use base monotonic optimisation
    isBaseMonoOpt = true;
  }
  if (parameters.find("THR") != parameters.end()) { // specify target throughput of DSE
    thrTargetSpecified = true;
  } else {
    VERBOSE_WARNING("No target throughput specified (target throughput will be set to max throughput by default) --- specify target throughput with '-p THR=n' flag");
  }
  if (parameters.find("SYMB_EXEC_ORIGINAL") != parameters.end() ||
      parameters.find("SYMB_EXEC_CORRECTED") != parameters.end()) {
    modelBoundedBuffers = false;
  }
  if (parameters.find("COARSE") != parameters.end()) { // use coarse step sizes (multiply by certain amount)
    useCoarse = true;
    coarseMultiplier = std::stoi(parameters["COARSE"]);
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
  models::Dataflow* dataflow_prime = new models::Dataflow(*dataflow);
  // add feedback channels in new graph to model bounded channel quantities
  if (modelBoundedBuffers) {
    {ForEachEdge(dataflow, c) {
        auto new_edge = dataflow_prime->addEdge(dataflow_prime->getEdgeTarget(c),
                                                dataflow_prime->getEdgeSource(c));
        dataflow_prime->setEdgeInPhases(new_edge,
                                        dataflow_prime->getEdgeOutVector(c));
        dataflow_prime->setEdgeOutPhases(new_edge,
                                         dataflow_prime->getEdgeInVector(c));
        dataflow_prime->setPreload(new_edge, dataflow->getPreload(c));
        dataflow_prime->setEdgeName(new_edge,
                                    dataflow_prime->getEdgeName(c) + "_prime");
      }}
  }


  // initialise search parameters
  std::map<Edge, TOKEN_UNIT> minStepSizes;
  std::map<Edge, std::pair<TOKEN_UNIT, TOKEN_UNIT>> minChannelSizes;
  TOKEN_UNIT minDistributionSize;
  std::chrono::duration<double, std::milli> cumulativeTime; // store timings
  auto cumulativeStart = std::chrono::steady_clock::now(); // DSE cumulative time starts here

  VERBOSE_DSE("INITIALISING SEARCH PARAMETERS:" << std::endl);
  if (modelBoundedBuffers) {
    initSearchParameters(dataflow_prime,
                         minStepSizes,
                         minChannelSizes);
  } else { // no need to model bounded buffer sizes for symbolic execution implementation
    initSearchParameters(dataflow,
                         minStepSizes,
                         minChannelSizes);
  }

  /* Setting lower bound channel quantities:
     In order to model bounded channel sizes, we make an edge
     that feeds back from the destination node to the source node.
     The capacity of a channel is thus modelled by:
     - INITIAL TOKENS, for original edges and
     - CHANNEL QUANTITY - INITIAL TOKENS, for feedback edges
     where the first element in the pair of minChannelSizes is the
     initial tokens in the channel while the second element is the
     capacity of the channel (in tokens). */
  if (modelBoundedBuffers) {
    {ForEachEdge(dataflow_prime, c) {
        if (dataflow_prime->getEdgeId(c) <= dataflow->getEdgesCount()) {
          minChannelSizes[c].second = 0; // original edges must have capacity 0
        }
        minChannelSizes[c].first = dataflow_prime->getPreload(c);
      }}
  }

  minDistributionSize = findMinimumDistributionSz(minChannelSizes);

  // initialise and store initial storage distribution state
  ARRAY_INDEX edgeCount = 0;
  if (modelBoundedBuffers) {
    edgeCount = dataflow_prime->getEdgesCount();
  } else {
    edgeCount = dataflow->getEdgesCount();
  }
  StorageDistribution initDist(edgeCount,
                               0,
                               minChannelSizes,
                               minDistributionSize);

  // initialise modelled graph with lower bound distribution
  if (modelBoundedBuffers) {
    {ForEachEdge(dataflow_prime, c) {
        if (dataflow_prime->getEdgeId(c) <= dataflow->getEdgesCount()) { // original channel IDs
          dataflow_prime->setPreload(c, initDist.getInitialTokens(c));
        } else {
          // subtract initial tokens from buffer size to model any initial tokens in buffer
          dataflow_prime->setPreload(c, (initDist.getChannelQuantity(c) -
                                         initDist.getInitialTokens(c)));
        }
      }}
  }


  // uncomment code block to get XMLs of lower bound distribution
  // printers::writeSDF3File(dirName + "dse_min_distribution_" +
  //                        dataflow_prime->getGraphName() + "_kiter"
  //                        ".xml", dataflow_prime);

  // calculate max throughput and current throughput with lower bound distribution
  // kperiodic_result_t result_max;
  TIME_UNIT maxThr;
  if (modelBoundedBuffers) {
    kperiodic_result_t result_max = compute_Kperiodic_throughput_and_cycles(dataflow, parameters);
    maxThr = result_max.throughput;
  } else {
    maxThr = compute_asap_throughput(dataflow, parameters);
  }
  VERBOSE_DSE("Max throughput: " << maxThr << std::endl);
  if (!thrTargetSpecified) {
    thrTarget = maxThr;
    VERBOSE_DSE("Target throughput set to max of " << thrTarget << std::endl);
  } else { // target throughput specified
    thrTarget = std::stold(parameters.find("THR")->second);
    if (thrTarget <= maxThr) {
      VERBOSE_DSE("Target throughput set to " << thrTarget << std::endl);
    } else { // invalid target throughput
      std::cerr << "ERROR: Specified target throughput (" << thrTarget
                << ") is larger than maximum throughput (" << maxThr
                << ")" << std::endl;
      return;
    }
  }

  kperiodic_result_t result;
  if (modelBoundedBuffers) {
    result = compute_Kperiodic_throughput_and_cycles(dataflow_prime, parameters);
  } else {
    result = compute_asap_throughput_and_cycles(dataflow, parameters, initDist);
  }

  /* a negative throughput indicates a deadlocked graph and so
     we set it to 0 to avoid bugs from having a negative throughput */
  if (result.throughput < 0) {
    initDist.setThroughput(0);
  } else { // set throughput to one computed from initial distribution otherwise
    initDist.setThroughput(result.throughput);
  }

  // add initial distribution to list of storage distributions
  StorageDistributionSet checklist;
  // if (isMonoOpt) {
  //   methodName = "_m_opt";
  //   auto startTime = std::chrono::steady_clock::now();
  //   checklist = algorithms::monotonic_optimised_Kperiodic_throughput_dse(dataflow_prime,
  //                                                                        initDist,
  //                                                                        thrTarget,
  //                                                                        computation_counter,
  //                                                                        parameters);
  //   auto endTime = std::chrono::steady_clock::now();
  //   std::chrono::duration<double, std::milli> execTime = endTime - startTime; // duration in ms
  //   cumulativeTime += execTime;
  //   std::cout << "M_OPT: time taken: " << cumulativeTime.count() << std::endl;
  // } else if (isBaseMonoOpt) {
  //   methodName = "_base_m_opt";
  //   auto startTime = std::chrono::steady_clock::now();
  //   checklist = algorithms::base_monotonic_optimised_Kperiodic_throughput_dse(dataflow_prime,
  //                                                                             initDist,
  //                                                                             thrTarget,
  //                                                                             computation_counter,
  //                                                                             parameters);
  //   auto endTime = std::chrono::steady_clock::now();
  //   std::chrono::duration<double, std::milli> execTime = endTime - startTime; // duration in ms
  //   cumulativeTime += execTime;
  //   std::cout << "B_M_OPT: time taken: " << cumulativeTime.count() << std::endl;
  // }
  if (isBaseMonoOpt) {
    methodName = "_base_m_opt";
    auto startTime = std::chrono::steady_clock::now();
    if (modelBoundedBuffers) {
      checklist = algorithms::base_monotonic_optimised_symbexec_throughput_dse(dataflow_prime,
                                                                               initDist,
                                                                               thrTarget,
                                                                               computation_counter,
                                                                               parameters);
    } else {
      checklist = algorithms::base_monotonic_optimised_symbexec_throughput_dse(dataflow,
                                                                               initDist,
                                                                               thrTarget,
                                                                               computation_counter,
                                                                               parameters);
    }
    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> execTime = endTime - startTime; // duration in ms
    cumulativeTime += execTime; // NOTE this will need to be updated: we want to track cumulative time as the global time elapsed rather than the sum of execution times
    std::cout << "B_M_OPT: time taken: " << cumulativeTime.count() << std::endl;
  }
  else if (modelBoundedBuffers) {
    methodName = "_kiter";
    checklist = StorageDistributionSet(initDist.getDistributionSize(),
                                       initDist);
  }
  else if (parameters.find("SYMB_EXEC_ORIGINAL") != parameters.end()) {
    methodName = "_symbexec_original";
    checklist = StorageDistributionSet(initDist.getDistributionSize(),
                                       initDist);
  }
  else if (parameters.find("SYMB_EXEC_CORRECTED") != parameters.end()) {
    methodName = "_symbexec_corrected";
    checklist = StorageDistributionSet(initDist.getDistributionSize(),
                                       initDist);
  }
  else {
    VERBOSE_WARNING("No DSE supported method specified; ending search.");
  }
  if (useCoarse) {
    methodName += "_coarse";
  }


  /* Initialise set of minimal storage distributions
     (a set of pairs of throughput and storage distribution)
     --- this will store the results of our DSE */
  StorageDistributionSet minStorageDist;

  // initialise data logging file
  std::ofstream dseLog;
  std::string colDelimiter = ";";
  if (writeLogFiles) {
    if (modelBoundedBuffers) {
      dseLog.open(logDirName + dataflow_prime->getGraphName() + "_dselog" + methodName + ".csv");
    } else {
      dseLog.open(logDirName + dataflow->getGraphName() + "_dselog" + methodName + ".csv");
    }
    dseLog << "storage distribution size" << colDelimiter
           << "throughput" << colDelimiter
           << "channel quantities" << colDelimiter
           // << "dependency mask" << colDelimiter
           << "computation duration" << colDelimiter
           << "cumulative duration"
           << std::endl; // initialise headers
  } else {
    std::cout << "storage distribution size;throughput;channel quantities;computation duration;cumulative duration"
              << std::endl; // initialise headers
  }

  // Start search algorithm
  VERBOSE_DSE("\n");
  VERBOSE_DSE("DSE BEGIN:" << std::endl);

  while (!minStorageDist.isSearchComplete(checklist, thrTarget)) {
    VERBOSE_DSE("Checking next storage distribution in checklist --- current checklist size: "
                << checklist.getSize() << std::endl);
    StorageDistribution checkDist(checklist.getNextDistribution()); // copy distribution for checking (first in checklist)
    checklist.removeStorageDistribution(checklist.getNextDistribution()); // remove said storage distribution from checklist

    // Update graph with storage distribution just removed from checklist
    VERBOSE_DSE(std::endl);
    VERBOSE_DSE("Exploring new storage distribution: " << std::endl);
    if (modelBoundedBuffers) {
      dataflow_prime->reset_computation(); // make graph writeable to alter channel size
      {ForEachEdge(dataflow_prime, c) {
          if (dataflow_prime->getEdgeId(c) > dataflow->getEdgesCount()) { // only modelled buffer preloads change
            dataflow_prime->setPreload(c, (checkDist.getChannelQuantity(c) -
                                           checkDist.getInitialTokens(c))); // always account for initial tokens in buffer
          }
        }}
    }

    // UNCOMMENT TO WRITE XMLs OF EXPLORED GRAPHS
    // commons::writeSDF3File(debugXMLName + "dse_min_distribution_" +
    //                     dataflow_prime->getGraphName() + "_kiter" + std::to_string(computation_counter) +
    //                     ".xml", dataflow_prime);

    // Compute throughput and storage deps
    auto startTime = std::chrono::steady_clock::now();
    if (modelBoundedBuffers) {
      result = compute_Kperiodic_throughput_and_cycles(dataflow_prime, parameters);
    } else {
      result = compute_asap_throughput_and_cycles(dataflow, parameters, checkDist);
    }

    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> execTime = endTime - startTime; // duration in ms
    computation_counter++;

    if (result.throughput < 0) { // all deadlocked graphs are equal in terms of throughput
      checkDist.setThroughput(0);
    } else {
      checkDist.setThroughput(result.throughput);
    }
    if (modelBoundedBuffers) {
      VERBOSE_DSE(checkDist.printInfo(dataflow_prime, modelBoundedBuffers));
    } else {
      VERBOSE_DSE(checkDist.printInfo(dataflow, modelBoundedBuffers));
    }

    // Add storage distribution and computed throughput to set of minimal storage distributions
    VERBOSE_DSE("\n");
    VERBOSE_DSE("\tUpdating set of minimal storage distributions..."
                << std::endl);
    minStorageDist.addStorageDistribution(checkDist);
    VERBOSE_DSE(std::endl);

    // Create new storage distributions for every storage dependency found; add new storage distributions to checklist
    for (std::set<Edge>::iterator it = (result.critical_edges).begin();
         it != (result.critical_edges).end(); it++) {
      StorageDistribution newDist(checkDist);
      // only increase channel quantity on "modelled" channels
      if (modelBoundedBuffers) {
        if (dataflow_prime->getEdgeId(*it) > dataflow->getEdgesCount()) {
          VERBOSE_DSE("\tFound storage dependency in channel "
                      << dataflow_prime->getEdgeName(*it) << std::endl);
          // make new modelled storage distribution according to storage dependencies
          if (!useCoarse) {
            newDist.setChannelQuantity(*it, (newDist.getChannelQuantity(*it) +
                                             minStepSizes[*it]));
          } else {
            newDist.setChannelQuantity(*it, (newDist.getChannelQuantity(*it) +
                                             (minStepSizes[*it] * coarseMultiplier)));
          }
          VERBOSE_DSE("\t\tIncreasing channel size of "
                      << dataflow_prime->getEdgeName(*it) << " to "
                      << newDist.getChannelQuantity(*it) << std::endl);
          VERBOSE_DSE("\tUpdating checklist with new storage distribution..."
                      << std::endl);
          checklist.addStorageDistribution(newDist);
        }
      } else {
        VERBOSE_DSE("\tFound storage dependency in channel "
                    << dataflow->getEdgeName(*it) << std::endl);
        // make new modelled storage distribution according to storage dependencies
          if (!useCoarse) {
            newDist.setChannelQuantity(*it, (newDist.getChannelQuantity(*it) +
                                             minStepSizes[*it]));
          } else {
            newDist.setChannelQuantity(*it, (newDist.getChannelQuantity(*it) +
                                             (minStepSizes[*it] * coarseMultiplier)));
          }
        VERBOSE_DSE("\t\tIncreasing channel size of "
                    << dataflow->getEdgeName(*it) << " to "
                    << newDist.getChannelQuantity(*it) << std::endl);
        VERBOSE_DSE("\tUpdating checklist with new storage distribution..."
                    << std::endl);
        checklist.addStorageDistribution(newDist);
      }

    }
    // NOTE we track cumulative time before the minimization algorithm in order to keep the experiment consistent with SDF3
    cumulativeTime = std::chrono::steady_clock::now() - cumulativeStart; // track time since start of DSE
    // Ensure minimal set is indeed minimal
    VERBOSE_DSE("\tTrying to minimise set with distribution size: "
                << checkDist.getDistributionSize() << std::endl);
    minStorageDist.minimizeStorageDistributions(checkDist);
    VERBOSE_DSE(std::endl);

    // write current storage distribution info to DSE log
    if (writeLogFiles) {
      dseLog << checkDist.getDistributionSize() << colDelimiter
             << checkDist.getThroughput() << colDelimiter;
      if (modelBoundedBuffers) {
        dseLog << checkDist.print_quantities_csv(dataflow_prime, modelBoundedBuffers) << colDelimiter
          // << checkDist.print_dependency_mask(dataflow_prime, result, modelBoundedBuffers) << ","
               << execTime.count() << colDelimiter
               << cumulativeTime.count() << std::endl;
      } else {
        dseLog << checkDist.print_quantities_csv(dataflow, modelBoundedBuffers) << colDelimiter
          // << checkDist.print_dependency_mask(dataflow, result, modelBoundedBuffers) << ","
               << execTime.count() << colDelimiter
               << cumulativeTime.count() << std::endl;
      }

    } else {
      std::cout << checkDist.getDistributionSize() << colDelimiter
                << checkDist.getThroughput() << colDelimiter;
      if (modelBoundedBuffers) {
        std::cout << checkDist.print_quantities_csv(dataflow_prime, modelBoundedBuffers) << colDelimiter
                  << execTime.count() << colDelimiter
                  << cumulativeTime.count() << std::endl;
      } else {
        std::cout << checkDist.print_quantities_csv(dataflow, modelBoundedBuffers) << colDelimiter
                  << execTime.count() << colDelimiter
                  << cumulativeTime.count() << std::endl;
      }
    }
  }

  // The minimum storage distribution for a throughput of 0 is (0, 0,..., 0)
  if (minStorageDist.getNextDistribution().getThroughput() == 0) {
    StorageDistribution zeroDist(minStorageDist.getNextDistribution());
    if (modelBoundedBuffers) {
      {ForEachEdge(dataflow_prime, c) {
          zeroDist.setChannelQuantity(c, 0);
        }}
    } else {
      {ForEachEdge(dataflow, c) {
          zeroDist.setChannelQuantity(c, 0);
        }}
    }

    minStorageDist.removeStorageDistribution(minStorageDist.getNextDistribution());
    minStorageDist.addStorageDistribution(zeroDist);
  }
  VERBOSE_DSE("\n");
  VERBOSE_DSE("DSE RESULTS [START] (target throughput: " << thrTarget
              << "):" << std::endl);
  if (modelBoundedBuffers) {
    VERBOSE_DSE("\n" << minStorageDist.printDistributions(dataflow_prime, modelBoundedBuffers));
  } else {
    VERBOSE_DSE("\n" << minStorageDist.printDistributions(dataflow, modelBoundedBuffers));
  }
  VERBOSE_DSE("DSE RESULTS [END]" << std::endl);
  VERBOSE_DSE("Done with search!" << std::endl);
  VERBOSE_DSE("Number of computations: " << computation_counter << std::endl);
  VERBOSE_DSE("Number of pareto points: " << minStorageDist.getSize() << std::endl);

  // Write log files and print file paths
  if (writeLogFiles) { // TODO account for SymbExec case
    dseLog.close();
    std::string graphName;
    if (modelBoundedBuffers) {
      graphName = dataflow_prime->getGraphName();
    } else {
      graphName = dataflow->getGraphName();
    }
    std::cout << "\nDSE log has been written to: "
              << logDirName + graphName + "_dselog" + methodName + ".csv"
              << std::endl;
    if (modelBoundedBuffers) {
      minStorageDist.writeCSV(ppDirName + graphName +
                              "_pp" + methodName + ".csv", dataflow_prime,
                              modelBoundedBuffers);
    } else {
      minStorageDist.writeCSV(ppDirName + graphName +
                              "_pp" + methodName + ".csv", dataflow,
                              modelBoundedBuffers);
    }

    std::cout << "\nPareto points have been written to: "
              << ppDirName + graphName + "_pp" + methodName + ".csv"
              << std::endl;

#ifdef WRITE_GRAPHS
    if (modelBoundedBuffers) {
      minStorageDist.printGraphs(dataflow_prime,
                                 dirName + dotfileDirName);
    } else {
      minStorageDist.printGraphs(dataflow,
                                 dirName + dotfileDirName);
    }
    std::cout << "\n" + std::to_string(minStorageDist.getSize()) + " "
              << "graphs of minimal storage distributions written to: "
              << dirName + dotfileDirName + graphName() + "_n.dot"
              << std::endl;
#endif
  } else {
    std::cout << "\nNote that you can use flag '-p LOG=true' or '-p LOGDIR=/whereyougo/' to write logs of DSE"
              << std::endl;
  }
}
