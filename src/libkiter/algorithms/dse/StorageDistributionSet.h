//
// Created by toky on 31/3/23.
//

#ifndef KITER_STORAGEDISTRIBUTIONSET_H
#define KITER_STORAGEDISTRIBUTIONSET_H

#include <models/Dataflow.h>
#include <commons/commons.h>
#include <algorithms/dse/BufferInfos.h>
#include <algorithms/dse/StorageDistribution.h>
#include "algorithms/schedulings.h"

namespace models {
    class Dataflow;
}


class StorageDistributionSet {
public:
    StorageDistributionSet();
    StorageDistributionSet(StorageDistribution distribution);
    void addStorageDistribution(StorageDistribution new_distribution); // add storage distribution to set
    void addStorageDistributions(std::vector<StorageDistribution>& new_distributions); // add storage distribution to set
    void removeStorageDistribution(StorageDistribution dist_to_rm); // remove storage distribution from set
    void removeDistributionSize(TOKEN_UNIT dist_sz); /* removes all storage distributions of
                                                      distribution size dist_sz */
    StorageDistribution getNextDistribution() const;
    size_t getSize() const;
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> getSet();
    std::pair<TOKEN_UNIT, TIME_UNIT> getThrDist();
    void minimizeStorageDistributions(StorageDistribution newDist);
    bool hasDistribution(TOKEN_UNIT dist_sz);
    bool hasStorageDistribution(StorageDistribution checkDist);
    bool isSearchComplete(StorageDistributionSet checklist, TIME_UNIT target_thr);
    bool isInBackCone(StorageDistribution checkDist,
                      std::map<Edge, TOKEN_UNIT> bufferLb);
    bool isInForeCone(StorageDistribution checkDist);
    void removeNonMaximum(StorageDistribution checkDist,
                          std::map<Edge, TOKEN_UNIT> bufferLb);
    void removeNonMinimum(StorageDistribution checkDist);
    void updateKneeSet(models::Dataflow* const dataflow,
                       StorageDistributionSet infeasibleSet,
                       StorageDistributionSet feasibleSet,
                       std::map<Edge, TOKEN_UNIT> bufferLb);
    void addEdgeKnees(models::Dataflow* const dataflow,
                      StorageDistributionSet infeasibleSet,
                      StorageDistributionSet feasibleSet,
                      std::map<Edge, TOKEN_UNIT> bufferLb);
    void updateInfeasibleSet(StorageDistribution new_sd,
                             std::map<Edge, TOKEN_UNIT> bufferLb); // add new SD to infeasible set of SDs for monotonic optimisation
    void updateFeasibleSet(StorageDistribution new_sd); // add new SD to infeasible set of SDs for monotonic optimisation
    std::string printDistributions(TOKEN_UNIT dist_sz); /* prints info of all storage distributions
								       of given distribution size */
    std::string printDistributions(); // prints info of all storage distributions in set
    void writeCSV(std::string filename); // writes to a CSV file for plots
    void printGraphs(std::string filename); // iterate through storage distribution set and print graphs
    // BASE MONOTONIC OPTIMISATION FUNCTIONS
    bool addToUnsat(StorageDistribution sd);
    void addToSat(StorageDistribution sd);
    void add(StorageDistribution sd, StorageDistributionSet &kneePoints);
    void addCut(models::Dataflow *dataflow, StorageDistribution sd,
                kperiodic_result_t result, StorageDistributionSet &kneePoints);
    bool satContains(StorageDistribution sd);
    bool unsatContains(StorageDistribution sd);

private:
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> set; /* store storage distributions
                                                                 by distribution size*/
    std::pair<TOKEN_UNIT, TIME_UNIT> p_max; /* stores the current maximum throughput and corresponding
                                             distribution size of set */
};
#endif //KITER_STORAGEDISTRIBUTIONSET_H
