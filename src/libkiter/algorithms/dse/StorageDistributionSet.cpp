//
// Created by toky on 31/3/23.
//

#include <algorithms/dse/StorageDistributionSet.h>
#include <algorithms/dse/buffer_sizing.h>
#include "algorithms/schedulings.h"


void addToExtensions(StorageDistributionSet &extensions,
                     StorageDistribution sd) {
    StorageDistributionSet subsumed;
    for (auto &dist_sz : extensions.getSet()) {
        for (auto &storage_dist : dist_sz.second) {
            if (storage_dist.inForConeOf(sd)) {
                subsumed.addStorageDistribution(storage_dist);
            } else if (sd.inForConeOf(storage_dist)) {
                return;
            }
        }
    }
    for (auto &dist_sz : subsumed.getSet()) {
        for (auto &storage_dist : dist_sz.second) {
            extensions.removeStorageDistribution(storage_dist);
        }
    }
    extensions.addStorageDistribution(sd);
}
// tries to add new SD to U; returns true if successful
bool StorageDistributionSet::addToUnsat(StorageDistribution sd) {
    StorageDistributionSet subsumed;
    for (auto &dist_sz : this->set) {
        for (auto &storage_dist : dist_sz.second) {
            if (storage_dist.inBackConeOf(sd) && storage_dist != sd) {
                subsumed.addStorageDistribution(storage_dist);
            } else if (sd.inBackConeOf(storage_dist)) {
                return false;
            }
        }
    }
    for (auto &dist_sz : subsumed.getSet()) {
        for (auto &storage_dist : dist_sz.second) {
            this->removeStorageDistribution(storage_dist);
        }
    }
    this->addStorageDistribution(sd);
    return true;
}

void StorageDistributionSet::addToSat(StorageDistribution sd) {
    StorageDistributionSet subsumed;
    for (auto &dist_sz : this->set) {
        for (auto &storage_dist : dist_sz.second) {
            if (sd.inForConeOf(storage_dist)) {
                return;
            } else if (storage_dist.inBackConeOf(sd)) {
                subsumed.addStorageDistribution(storage_dist);
            }
        }
    }
    for (auto &dist_sz : subsumed.getSet()) {
        for (auto &storage_dist : dist_sz.second) {
            this->removeStorageDistribution(storage_dist);
        }
    }
    this->addStorageDistribution(sd);
}

void StorageDistributionSet::add(StorageDistribution sd,
                                 StorageDistributionSet &kneePoints) {
    if (!this->addToUnsat(sd)) {
        return;
    }
    // update knee points
    StorageDistributionSet subsumed;
    for (auto &dist_sz : kneePoints.getSet()) {
        for (auto &knee : dist_sz.second) {
            if (knee.inBackConeOf(sd)) {
                subsumed.addStorageDistribution(knee);
            }
        }
    }
    // remove subsumed knees
    for (auto &dist_sz : subsumed.getSet()) {
        for (auto &storage_dist : dist_sz.second) {
            kneePoints.removeStorageDistribution(storage_dist);
        }
    }
    // create extensions
    StorageDistributionSet extensions;
    for (auto &dist_sz : subsumed.getSet()) {
        for (auto &storage_dist : dist_sz.second) {
            for (auto &e : storage_dist.getEdges()) {
                StorageDistribution newExt(storage_dist);
                newExt.setChannelQuantity(e, sd.getChannelQuantity(e));
                addToExtensions(extensions, newExt);
            }
        }
    }
    for (auto &dist_sz : extensions.getSet()) {
        for (auto &storage_dist : dist_sz.second) {
            kneePoints.addStorageDistribution(storage_dist);
        }
    }
    return;
}

void StorageDistributionSet::addCut(models::Dataflow *dataflow,
                                    StorageDistribution sd,
                                    kperiodic_result_t result,
                                    StorageDistributionSet &kneePoints) {
    // TODO need to check if there aren't any critical edges --- throw error if so
    if (!(result.critical_edges).empty()) {
        bool isCut = false;
        StorageDistribution tempDist(sd);
        {ForEachEdge(dataflow, e) {
                if (dataflow->getEdgeId(e) > dataflow->getEdgesCount()/2) {
                    if ((result.critical_edges).find(e) == (result.critical_edges).end()) {
                        tempDist.setChannelQuantity(e, INT_MAX);
                        isCut = true;
                    }
                }
            }}
        if (isCut) {
            this->add(tempDist, kneePoints);
        }
    }
}


bool StorageDistributionSet::satContains(StorageDistribution sd) {
    for (auto &dist_sz : this->set) {
        for (auto &storage_dist : dist_sz.second) {
            if (sd.inForConeOf(storage_dist)) {
                return true;
            }
        }
    }
    return false;
}

bool StorageDistributionSet::unsatContains(StorageDistribution sd) {
    for (auto &dist_sz : this->set) {
        for (auto &storage_dist : dist_sz.second) {
            if (sd.inBackConeOf(storage_dist)) {
                return true;
            }
        }
    }
    return false;
}

// Print info of all storage distributions of a given distribution size in set
std::string StorageDistributionSet::printDistributions(TOKEN_UNIT dist_sz) {
    assert(set.find(dist_sz) != set.end());

    std::string dInfo;
    dInfo += "Printing storage distributions of distribution size: "
             + std::to_string(dist_sz) + "\n";
    for (auto &i : set[dist_sz]) {
        dInfo += i.printInfo();
    }
    return dInfo;
}

// Print info of all storage distributions in set
std::string StorageDistributionSet::printDistributions() {
    std::string allInfo;
    for (auto &it : this->set) {
        allInfo += printDistributions(it.first);
    }
    return allInfo;
}

/* Writes storage distribution set info to a CSV file to plot data
   Takes in file name as argument --- explicitly state file
   format (e.g. "example_filename.csv") */
void StorageDistributionSet::writeCSV(std::string filename) {
    std::ofstream outputFile;
    outputFile.open(filename);

    outputFile << StorageDistribution::get_csv_header() << std::endl; // initialise headers
    for (auto &it : this->set) {
        for (auto &sd : this->set[it.first]) {
            outputFile << sd.get_csv_line() << std::endl;
        }
    }
    outputFile.close();
}

/* Iterate through storage distribution set and print graphs
   in DOT format */
void StorageDistributionSet::printGraphs(std::string pathName) {
    std::ofstream outputFile;
    int graphCount = 0;

    for (auto &it : this->set) {
        for (auto &sd : this->set[it.first]) {

            std::string fileName = pathName +sd.getGraphName() +
                                   "_" + std::to_string(graphCount) + ".dot";
            outputFile.open(fileName);
            outputFile << sd.printGraph();
            outputFile.close();
            graphCount++;
        }
    }
}

StorageDistributionSet::StorageDistributionSet() {
    // TODO: Implement default constructor
}

StorageDistributionSet::StorageDistributionSet(StorageDistribution distribution) {
    // FIXME: initialize map (using initializer_list?)// : set{{dist_sz, distribution}} {
    this->set[distribution.getDistributionSize()].push_back(distribution);
    // TODO: initialize p_max variable
    this->p_max = std::make_pair(distribution.getDistributionSize(), distribution.getThroughput());
}



void StorageDistributionSet::addStorageDistributions(std::vector<StorageDistribution>& new_distributions) {
    for (auto sd : new_distributions) {
        this->addStorageDistribution(sd);
    }
}

/*
 * Add a new storage distribution to the set of storage distributions
 * Will not add storage distribution if:
 * - An identical storage distribution already exists in the set
 */
void StorageDistributionSet::addStorageDistribution(StorageDistribution new_distribution) {
    VERBOSE_DEBUG("\t\tAttempting to add storage distribution of dist sz: "
                              << new_distribution.getDistributionSize()
                              << " to set" );
    if (!hasDistribution(new_distribution.getDistributionSize())) { // first storage distribution of this distribution size
        VERBOSE_DEBUG("\t\t\tFirst of this distribution size: adding new distribution" );
        this->set[new_distribution.getDistributionSize()].push_back(new_distribution);
    } else { // there's already a storage distribution with the same distribution size
        for (auto &distribution : this->set[new_distribution.getDistributionSize()]) {
            // don't add storage distributions that are already in checklist
            if (new_distribution == distribution) {
                VERBOSE_DEBUG("\t\t\tFound matching distribution: not adding new distribution" );
                return;
            }
        }
        // no matching storage distributions of equal distribution size
        this->set[new_distribution.getDistributionSize()].push_back(new_distribution);
        VERBOSE_DEBUG("\t\t\tNew storage distribution added: " << new_distribution.getQuantitiesStr() );
    }

    // update maximum throughput and corresponding distribution size in set
    if (new_distribution.getThroughput() > this->p_max.second) {
        this->p_max = std::make_pair(new_distribution.getDistributionSize(),
                                     new_distribution.getThroughput());
    }
}

// Removes the given storage distribution from the storage distribution set
void StorageDistributionSet::removeStorageDistribution(StorageDistribution dist_to_rm) {
    // Use erase-remove idiom to remove matching storage distribution
    this->set[dist_to_rm.getDistributionSize()].erase(std::remove(this->set[dist_to_rm.getDistributionSize()].begin(),
                                                                  this->set[dist_to_rm.getDistributionSize()].end(),
                                                                  dist_to_rm), this->set[dist_to_rm.getDistributionSize()].end());

    // remove distribution size from set if there aren't any storage distributions left
    if (this->set[dist_to_rm.getDistributionSize()].empty()) {
        removeDistributionSize(dist_to_rm.getDistributionSize());
    }
}

// Removes all storage distributions of the given distribution size from set
void StorageDistributionSet::removeDistributionSize(TOKEN_UNIT dist_sz) {
    this->set.erase(dist_sz);
}

StorageDistribution StorageDistributionSet::getNextDistribution() const {
    return this->set.begin()->second.front();
}

// returns number of storage distributions in set
size_t StorageDistributionSet::getSize() const {
    size_t total_size = 0;
    for (auto &d : this->set) {
        total_size += d.second.size();
    }
    return total_size;
}

// returns a copy of the set of storage distributions
std::map<TOKEN_UNIT, std::vector<StorageDistribution>> StorageDistributionSet::getSet() {
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);
    return reference_set;
}

std::pair<TOKEN_UNIT, TIME_UNIT> StorageDistributionSet::getThrDist() {
    std::pair<TOKEN_UNIT, TIME_UNIT>res(this->p_max);
    return res;
}

/* Removes the new storage distribution from the storage distribution set if:
   - there already is a storage distribution with equal distribution size with
   throughput >= to new storage distribution's throughput
   OR
   - the new storage distribution has throughput that is <= throughput of storage
   distributions with a smaller distribution size
*/
void StorageDistributionSet::minimizeStorageDistributions(StorageDistribution newDist) {
    // FIXME: get rid of nested for-loop
    // get distribution size and throughput early to avoid repeated function calls
    TOKEN_UNIT newDistSz = newDist.getDistributionSize();
    TIME_UNIT newThr = newDist.getThroughput();
    // make copy of minimal distribution set
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);

    // remove non-minimal storage distributions
    /* NOTE: we don't need to update the p_max values here as the conditions ensure
       that we always store the max throughput with the minimum distribution size
       i.e. the only way we would have a falsely high p_max is if we add a storage
       distribution with an absurdly high distribution size and thus get max
       throughput --- that is, we will only need to consider updating p_max outside
       of the addStorageDistribution function if we don't increment by minStepSizes */
    for (auto &distribution_sz : reference_set) {
        for (auto &storage_dist : distribution_sz.second) {
            if ((newDistSz > storage_dist.getDistributionSize() &&
                 (newThr < storage_dist.getThroughput() ||
                  commons::AreSame(newThr, storage_dist.getThroughput()))) || // needed to compare floats
                (newDistSz >= storage_dist.getDistributionSize() &&
                 newThr < storage_dist.getThroughput())) {
                VERBOSE_DEBUG("\t\tNew storage distribution of size "
                                          << newDist.getDistributionSize()
                                          << " found to be non-minimal: removing from set"
                );
                this->removeStorageDistribution(newDist);
                return; // no need to iterate through rest of set once newDist has been removed
            } else if (newDistSz == storage_dist.getDistributionSize() && // remove existing equal storage dist (size) with lower thr
                       newThr > storage_dist.getThroughput()) {
                VERBOSE_DEBUG("\t\tExisting storage distribution of size "
                                          << storage_dist.getDistributionSize()
                                          << " found to be non-minimal: removing from set"
                );
                this->removeStorageDistribution(storage_dist);
            }
        }
    }
}

/* Check if storage distributions of a given distribution size exist in the
   storage distribution set
   Returns true if storage distribution of the given distribution size is found */
bool StorageDistributionSet::hasDistribution(TOKEN_UNIT dist_sz) {
    return (this->set.find(dist_sz) != this->set.end());
}

// returns true if the given SD is in the SD set
bool StorageDistributionSet::hasStorageDistribution(StorageDistribution checkDist) {
    bool isInSet = false;
    for (auto &dist : this->set) {
        for (auto &sd : dist.second) {
            if (checkDist == sd) {
                isInSet = true;
            }
        }
    }
    return isInSet;
}

// Check if DSE completion conditions have been met
bool StorageDistributionSet::isSearchComplete(StorageDistributionSet checklist,
                                              TIME_UNIT target_thr) {
    /* search is complete when the max throughput has been found and there
       isn't any more storage distributions of the same distribution size
       left to check */
    VERBOSE_DEBUG(" ** Checking if search is complete: (current/target throughput:" <<  this->p_max.second << "/" << target_thr << ")"
                                                                                        << " (current ds: "<<  this->p_max.first << ")"
                                                                                        << " (checklist size: "<<  checklist.getSize() << ")"
                                                                                        << " (same ds available: "<<  checklist.hasDistribution(this->p_max.first) << ")");


    return ((commons::AreSame(this->p_max.second, target_thr) &&
             (!checklist.hasDistribution(this->p_max.first))) ||
            (checklist.getSize() <= 0)); // also end when we're out of distributions to check
}


/* returns true if given SD is within the backward cone of the set of SDs
   (i.e. given SD is not maximal in set) */
bool StorageDistributionSet::isInBackCone(StorageDistribution checkDist,
                                          std::map<Edge, TOKEN_UNIT> bufferLb) {
    bool isMinimal;
    std::vector<Edge> checkDistEdges = checkDist.getEdges();
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);

    for (auto &distribution_sz : reference_set) {
        for (auto &storage_dist : distribution_sz.second) {
            if (checkDist != storage_dist) { // don't check against itself
                isMinimal = true;
                for (auto it = checkDistEdges.begin();
                     it != checkDistEdges.end(); it++) {
                    if ((checkDist.getChannelQuantity(*it) > storage_dist.getChannelQuantity(*it)) &&
                        (checkDist.getChannelQuantity(*it) > bufferLb[*it])) {
                        isMinimal = false;
                    }
                }
            }
            /* if isMinimal = true at this point, should remove checkDist as it's def non-maximal
               if isMinimal = false at this point, should check the remaining SDs as there could
               still be another SD that is strictly larger than checkDist */
            if (isMinimal) {
                // std::cout << "SD of size " << checkDist.getDistributionSize()
                //           << " found to be non-maximal in set" << std::endl;
                return isMinimal;
            }
        }
    }
    isMinimal = false; // if checkDist hasn't been removed at this point, then it's not in the backward cone
    return isMinimal;
}


/* returns true if given SD is within the foreward cone of the set of SDs
   (i.e. given SD is not minimal in set) */
bool StorageDistributionSet::isInForeCone(StorageDistribution checkDist) {
    bool isMaximal;
    std::vector<Edge> checkDistEdges = checkDist.getEdges();
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);

    for (auto &distribution_sz : reference_set) {
        for (auto &storage_dist : distribution_sz.second) {
            if (checkDist != storage_dist) { // don't check against itself
                isMaximal = true;
                for (auto it = checkDistEdges.begin();
                     it != checkDistEdges.end(); it++) {
                    if (checkDist.getChannelQuantity(*it) < storage_dist.getChannelQuantity(*it)) {
                        isMaximal = false;
                    }
                }
                /* if isMaximal = true at this point, should remove checkDist as it's def non-minimal
                   if isMaximal = false at this point, should check the remaining SDs as there could
                   still be another SD that is strictly smaller than checkDist */
                if (isMaximal) {
                    // std::cout << "SD of size " << checkDist.getDistributionSize()
                    //           << " found to be non-minimal in set" << std::endl;
                    return isMaximal;
                } else {
                    isMaximal = true; // reset for check against next SD in set
                }
            }
        }
    }
    isMaximal = false; // if checkDist hasn't been removed at this point, then it's not in the foreward cone
    return isMaximal;
}

void StorageDistributionSet::removeNonMaximum(StorageDistribution checkDist,
                                              std::map<Edge, TOKEN_UNIT> bufferLb) {
    // bool isMinimal = true;
    // std::cout << "Removing non-maximal SDs..." << std::endl;
    if (this->getSize() <= 1) {
        // std::cout << "0/1 SD in set; skipping check" << std::endl;
        return; // only check if there's more than one SD in set
    }
    else if (this->isInBackCone(checkDist, bufferLb)) {
        this->removeStorageDistribution(checkDist);
    }
}


void StorageDistributionSet::removeNonMinimum(StorageDistribution checkDist) {
    // bool isMaximal = true;
    // std::vector<Edge> checkDistEdges = checkDist.getEdges();
    // std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);
    // std::cout << "Removing non-minimal SDs..." << std::endl;
    if (this->getSize() <= 1) {
        // std::cout << "0/1 SD in set; skipping check" << std::endl;
        return; // only check if there's more than one SD in set
    }
    else if (this->isInForeCone(checkDist)) {
        this->removeStorageDistribution(checkDist);
    }
}


// Updates the given set of knee points according to the given set of infeasible SDs
// the new SD point
void StorageDistributionSet::updateKneeSet(models::Dataflow* const dataflow,
                                           StorageDistributionSet infeasibleSet,
                                           StorageDistributionSet feasibleSet,
                                           std::map<Edge, TOKEN_UNIT> bufferLb) {
    // StorageDistribution newDist) {
    StorageDistributionSet checkQueue(infeasibleSet); // store queue of SDs to be compared
    StorageDistributionSet checkedSDs;
    // go through every permutation of pairs of SDs (including newly found knee points) and find local min
    /* NOTE this is a little inefficient as it generates a whole new set of knee points every time it's called
       - might be better to find a way if a new point would affect the current knee set in any way before iterating */
    bool checkFinished = false;
    if (infeasibleSet.getSize() > 1) { // no need to check if there's only one point
        while(!checkFinished) {
            StorageDistribution checkDist;
            if (!checkedSDs.getSize()) {
                checkDist = checkQueue.getNextDistribution();
            } else {
                for (auto &d_sz_a : checkQueue.getSet()) {
                    for (auto &sd_a : d_sz_a.second) {
                        bool isChecked = false;
                        for (auto &d_sz_b : checkedSDs.getSet()) {
                            for (auto &sd_b : d_sz_b.second) {
                                if (sd_a == sd_b) {
                                    isChecked = true;
                                }
                            }
                        }
                        if (!isChecked) {
                            checkDist = sd_a;
                        }
                    }
                }
            }
            std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set = checkQueue.getSet();
            for (auto &distribution_sz : reference_set) {
                for (auto &storage_dist : distribution_sz.second) {
                    if (checkDist != storage_dist) { // don't check against itself
                        StorageDistribution kneePoint = makeMinimalSD(checkDist, storage_dist);
                        this->addStorageDistribution(kneePoint);
                        checkQueue.addStorageDistribution(kneePoint);
                    }
                }
            }
            checkedSDs.addStorageDistribution(checkDist);
            if (checkQueue.getSet().size() == checkedSDs.getSet().size()) { // TODO replace this stand-in SD set equality check
                checkFinished = true;
                for (auto &d_sz : checkQueue.getSet()) {
                    for (auto &sd : d_sz.second) {
                        if (!checkedSDs.hasStorageDistribution(sd)) {
                            checkFinished = false;
                        }
                    }
                }
            }
            // checkFinished = (checkQueue.getSet() == checkedSDs.getSet());
        }
    }
    this->addEdgeKnees(dataflow, infeasibleSet, feasibleSet, bufferLb);
    // remove knee points in backwards cone of knee set
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);
    for (auto &distribution_sz : reference_set) {
        for (auto &storage_dist : distribution_sz.second) {
            this->removeNonMaximum(storage_dist, bufferLb);
        }
    }
}


// Adds the edge cases to the knee set
void StorageDistributionSet::addEdgeKnees(models::Dataflow* const dataflow,
                                          StorageDistributionSet infeasibleSet,
                                          StorageDistributionSet feasibleSet,
                                          std::map<Edge, TOKEN_UNIT> bufferLb) {
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set = infeasibleSet.getSet();
    StorageDistribution maximalSD(infeasibleSet.getNextDistribution());
    std::vector<Edge> edges = maximalSD.getEdges();
    // find maximal buffer sizes for all buffers
    for (auto &distribution_sz : reference_set) {
        for (auto &storage_dist : distribution_sz.second) {
            for (auto it = edges.begin(); it != edges.end(); it++) {
                if (storage_dist.getChannelQuantity(*it) > maximalSD.getChannelQuantity(*it)) {
                    maximalSD.setChannelQuantity(*it, storage_dist.getChannelQuantity(*it));
                }
            }
        }
    }
    // construct and add edge knee points
    {ForEachEdge(dataflow, c) {
            if (dataflow->getEdgeId(c) > dataflow->getEdgesCount() / 2) {
                StorageDistribution tempSD(maximalSD);
                tempSD.setChannelQuantity(c, bufferLb[c]);
                if (!infeasibleSet.hasStorageDistribution(tempSD) &&
                    !feasibleSet.hasStorageDistribution(tempSD) &&
                    tempSD != maximalSD) {
                    // std::cout << "Adding edge knee:" << std::endl;
                    // std::cout << tempSD.printInfo(dataflow) << std::endl;
                    this->addStorageDistribution(tempSD);
                }
            }
        }}
}

// add new SD to set of infeasible SDs
void StorageDistributionSet::updateInfeasibleSet(StorageDistribution newDist,
                                                 std::map<Edge, TOKEN_UNIT> bufferLb) {
    TOKEN_UNIT newDistSz = newDist.getDistributionSize();
    TOKEN_UNIT maxDistSz;
    bool isFound = false; // track if newDist found to be maximal

    // identify current maximum distribution size
    if (!this->getSize()) { // if there aren't any other SDs in set, just add
        // std::cout << "Adding SD of size " << newDist.getDistributionSize()
        //           << " to U" << std::endl;
        this->addStorageDistribution(newDist);
        // std::cout << "Distribution sizes of U (" << this->getSize() << " distributions):" << std::endl;
        for (auto &distribution_sz : this->set) {
            for (auto &storage_dist : distribution_sz.second) {
                std::cout << storage_dist.getDistributionSize() << std::endl;
            }
        }
        return;
    } else {
        maxDistSz = this->set.rbegin()->first; // store largest SD
        // std::cout << "Max size: " << maxDistSz << std::endl;
    }

    // if new SD has larger distribution size, then can definitely add
    if (newDistSz >= maxDistSz) {
        // std::cout << "Adding SD of size " << newDist.getDistributionSize()
        //           << " to U" << std::endl;
        this->addStorageDistribution(newDist);
        // std::cout << "Distribution sizes of U (" << this->getSize() << " distributions):" << std::endl;
        for (auto &distribution_sz : this->set) {
            for (auto &storage_dist : distribution_sz.second) {
                std::cout << storage_dist.getDistributionSize() << std::endl;
            }
        }
    } else {
        std::vector<Edge> newDistEdges = newDist.getEdges();
        std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);
        // only add new SD if at least one channel has larger size than an SD currently in infeasible set
        for (auto &distribution_sz : reference_set) {
            for (auto &storage_dist : distribution_sz.second) {
                for (auto it = newDistEdges.begin();
                     it != newDistEdges.end(); it++) {
                    if (newDist.getChannelQuantity(*it) > storage_dist.getChannelQuantity(*it) &&
                        !isFound) {
                        // std::cout << "Adding SD of size " << newDist.getDistributionSize()
                        //           << " to U" << std::endl;
                        isFound = true;
                        this->addStorageDistribution(newDist);
                        // std::cout << "Distribution sizes of U (" << this->getSize() << " distributions):" << std::endl;
                        for (auto &distribution_sz : this->set) {
                            for (auto &storage_dist : distribution_sz.second) {
                                std::cout << storage_dist.getDistributionSize() << std::endl;
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);
    // remove SDs subsumed by edge of infeasible set (non-maximal SDs)
    for (auto &distribution_sz : reference_set) {
        for (auto &storage_dist : distribution_sz.second) {
            this->removeNonMaximum(storage_dist, bufferLb);
        }
    }
    // std::cout << "Distribution sizes of U (" << this->getSize() << " distributions):" << std::endl;
    for (auto &distribution_sz : this->set) {
        for (auto &storage_dist : distribution_sz.second) {
            std::cout << storage_dist.getDistributionSize() << std::endl;
        }
    }
}


// add new SD to set of feasible SDs
void StorageDistributionSet::updateFeasibleSet(StorageDistribution newDist) {
    TOKEN_UNIT newDistSz = newDist.getDistributionSize();
    TOKEN_UNIT minDistSz;
    bool isFound = false; // track if newDist found to be minimal

    // identify current minimum distribution size
    if (!this->getSize()) {
        // std::cout << "Adding SD of size " << newDist.getDistributionSize()
        //           << " to S" << std::endl;
        this->addStorageDistribution(newDist); // if there aren't any other SDs in set, just add
        // std::cout << "Distribution sizes of S (" << this->getSize() << " distributions):" << std::endl;
        // for (auto &distribution_sz : this->set) {
        //   for (auto &storage_dist : distribution_sz.second) {
        //     // std::cout << storage_dist.getDistributionSize() << std::endl;
        //   }
        // }
        return;
    } else {
        minDistSz = this->set.begin()->first; // store smallest SD
        // std::cout << "Min size: " << minDistSz << std::endl;
    }

    // if new SD has smaller distribution size, then can definitely add
    if (newDistSz <= minDistSz) {
        // std::cout << "Adding SD of size " << newDist.getDistributionSize()
        //           << " to S" << std::endl;
        this->addStorageDistribution(newDist);
        // std::cout << "Distribution sizes of S (" << this->getSize() << " distributions):" << std::endl;
        // for (auto &distribution_sz : this->set) {
        //   for (auto &storage_dist : distribution_sz.second) {
        //     // std::cout << storage_dist.getDistributionSize() << std::endl;
        //   }
        // }
    } else {
        std::vector<Edge> newDistEdges = newDist.getEdges();
        std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);
        // only add new SD if at least one channel has smaller size than an SD currently in feasible set
        for (auto &distribution_sz : reference_set) {
            for (auto &storage_dist : distribution_sz.second) {
                for (auto it = newDistEdges.begin();
                     it != newDistEdges.end(); it++) {
                    if (newDist.getChannelQuantity(*it) < storage_dist.getChannelQuantity(*it) &&
                        !isFound) {
                        // std::cout << "Adding SD of size " << newDist.getDistributionSize()
                        //           << " to S" << std::endl;
                        isFound = true;
                        this->addStorageDistribution(newDist);
                        // std::cout << "Distribution sizes of S (" << this->getSize() << " distributions):" << std::endl;
                        // for (auto &distribution_sz : this->set) {
                        //   for (auto &storage_dist : distribution_sz.second) {
                        //     std::cout << storage_dist.getDistributionSize() << std::endl;
                        //   }
                        // }
                        break;
                    }
                }
            }
        }
    }
    std::map<TOKEN_UNIT, std::vector<StorageDistribution>> reference_set(this->set);
    // remove SDs subsumed by edge of feasible set (non-minimal SDs)
    for (auto &distribution_sz : reference_set) {
        for (auto &storage_dist : distribution_sz.second) {
            this->removeNonMinimum(storage_dist);
        }
    }
    // std::cout << "Distribution sizes of S(" << this->getSize() << " distributions):" << std::endl;
    // for (auto &distribution_sz : this->set) {
    //   for (auto &storage_dist : distribution_sz.second) {
    //     std::cout << storage_dist.getDistributionSize() << std::endl;
    //   }
    // }
}
