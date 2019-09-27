/*
 * buffer_sizing.h
 *
 */

#ifndef BUFFER_SIZING_H_
#define BUFFER_SIZING_H_

#include <models/Dataflow.h>
#include <commons/commons.h>

namespace models {
  class Dataflow;
}

class StorageDistribution {
public:
  StorageDistribution();
  StorageDistribution(unsigned int edge_count,
                      TIME_UNIT thr,
                      std::map<Edge, TOKEN_UNIT> channel_quantities,
                      TOKEN_UNIT distribution_size);
  //  StorageDistribution(const StorageDistribution&)=default; // using default copy constructor for now

  void setChannelQuantity(Edge e, TOKEN_UNIT quantity);
  void setDistributionSize(TOKEN_UNIT sz);
  void setThroughput(TIME_UNIT thr);
  TOKEN_UNIT getChannelQuantity(Edge e) const;
  TOKEN_UNIT getDistributionSize() const;
  TIME_UNIT getThroughput() const;
  unsigned int getEdgeCount() const;
  bool operator==(const StorageDistribution& distribution) const;
  bool operator!=(const StorageDistribution& distribution) const;
  void updateDistributionSize();
  void print_info();
private:
  unsigned int edge_count;
  TIME_UNIT thr; // throughput of given storage distribution
  TOKEN_UNIT distribution_size; // should be equal to sum of channel quantities
  std::map<Edge, TOKEN_UNIT> channel_quantities; // amount of space (in tokens per channel)
};

class StorageDistributionSet {
public:
  StorageDistributionSet();
  StorageDistributionSet(TOKEN_UNIT dist_sz,
                         StorageDistribution distribution);
  void addStorageDistribution(StorageDistribution new_distribution); // add storage distribution to set
  void removeStorageDistribution(StorageDistribution dist_to_rm); // remove storage distribution from set
  void removeDistributionSize(TOKEN_UNIT dist_sz); /* removes all storage distributions of
                                                      distribution size dist_sz */
  StorageDistribution getNextDistribution() const;
  size_t getSize() const;
  void minimizeStorageDistributions(StorageDistribution newDist);
  bool hasDistribution(TOKEN_UNIT dist_sz);
  bool isSearchComplete(StorageDistributionSet checklist, TIME_UNIT target_thr);
  void print_distributions(TOKEN_UNIT dist_sz); /* prints info of all storage distributions 
                                                   of given distribution size */
  void print_distributions(); // prints info of all storage distributions in set
  
private:
  std::map<TOKEN_UNIT, std::vector<StorageDistribution>> set; /* store storage distributions 
                                                                 by distribution size*/
  std::pair<TOKEN_UNIT, TIME_UNIT> p_max; /* stores the current maximum throughput and corresponding
                                             distribution size of set */
};

// to search parameters
void findMinimumStepSz(models::Dataflow *dataflow,
                       std::map<Edge, TOKEN_UNIT> &minStepSizes);
void findMinimumChannelSz(models::Dataflow *dataflow,
                          std::map<Edge, TOKEN_UNIT> &minChannelSizes);
TOKEN_UNIT findMinimumDistributionSz(models::Dataflow *dataflow,
                                     std::map<Edge, TOKEN_UNIT> minChannelSizes);
void initSearchParameters(models::Dataflow *dataflow,
                          std::map<Edge, TOKEN_UNIT> &minStepSizes,
                          std::map<Edge, TOKEN_UNIT> &minChannelSizes);
#endif /* BUFFER_SIZING_H_ */
