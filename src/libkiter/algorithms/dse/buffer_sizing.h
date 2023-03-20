/*
 * buffer_sizing.h
 *
 */

#ifndef BUFFER_SIZING_H_
#define BUFFER_SIZING_H_

#include <models/Dataflow.h>
#include <commons/commons.h>
#include <algorithms/throughput/kperiodic.h>

#define VERBOSE_DEBUG_DSE(m)    if (VERBOSE_IS_DEBUG())    	std::cerr << PURPLE_COLOR  << "[DSE]" << __SHOW_LEVEL << m << std::string(20,' ') << "\n"
#define VERBOSE_DSE(m)    if (VERBOSE_IS_INFO())    	std::cerr << PURPLE_COLOR  << "[DSE]" << __SHOW_LEVEL << m << std::string(20,' ') << "\n"

namespace models {
  class Dataflow;
}

struct BufferInfos {
    TOKEN_UNIT preload;
    TOKEN_UNIT buffer_size;
};
class StorageDistribution {
public:
  StorageDistribution();
  StorageDistribution(const models::Dataflow* dataflow);
  StorageDistribution(const models::Dataflow* dataflow,
                      TIME_UNIT thr,
                      std::map<Edge,
                              BufferInfos> channel_quantities);
  void setChannelQuantity(Edge e, TOKEN_UNIT quantity);
  void setInitialTokens(Edge e, TOKEN_UNIT token_count);
  void setDistributionSize(TOKEN_UNIT sz);
    void setThroughput(TIME_UNIT thr);
    void setCumulativeTime(double t) {this->cumulativeTime = t;};
    void setExecutionTime(double t)  {this->executionTime = t;};
  TOKEN_UNIT getChannelQuantity(Edge e) const;
  TOKEN_UNIT getInitialTokens(Edge e) const;
  const models::Dataflow * getDataflow() const {
      return this->dataflow;
  }
  bool hasEdge(Edge e) const;
  std::string getGraphName() const {
      return dataflow->getGraphName();
  }
  TOKEN_UNIT getDistributionSize() const;
  TIME_UNIT getThroughput() const;
  ARRAY_INDEX getEdgeCount() const;
  std::vector<Edge> getEdges() const;
  bool operator==(const StorageDistribution& distribution) const;
  bool operator!=(const StorageDistribution& distribution) const;
  void updateDistributionSize();

    std::string get_csv_header() {
        return "storage distribution size,throughput,channel quantities,computation duration,cumulative duration";
    }
    std::string get_csv_line() {
        std::ostringstream stream;

        stream << this->getDistributionSize() << ","
                  << this->getThroughput() << ","
                  << this->print_quantities_csv() << ","
                  << this->executionTime << ","
                  << this->cumulativeTime ;
        return stream.str();
    }


  std::string getQuantitiesStr() {
      std::string output("\"");
      std::string delim("");

      {ForEachEdge(this->dataflow, c) {
          output += delim;
          output += std::to_string(this->getChannelQuantity(c));
          delim = ",";
      }}
      output += "\"";
      return output;
  }

  std::string printInfo() const;
  std::string print_quantities_csv() const;
  std::string print_dependency_mask(kperiodic_result_t const result) const;
  std::string printGraph() const;
  void updateGraph(models::Dataflow* dataflow);
  // BASE MONOTONIC OPTIMISATION FUNCTIONS
  bool inBackConeOf(StorageDistribution checkDist);
  bool inForConeOf(StorageDistribution checkDist);

private:
    const models::Dataflow* dataflow;
  //ARRAY_INDEX edge_count;
  TIME_UNIT thr; // throughput of given storage distribution
  double cumulativeTime, executionTime;
  std::map<Edge, BufferInfos> channel_quantities; // amount of space (in tokens per channel)
  TOKEN_UNIT distribution_size; // should be equal to sum of channel quantities
};

std::ostream& operator<<(std::ostream& out, const StorageDistribution& f);
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

// to search parameters
void findMinimumStepSz(models::Dataflow *dataflow,
                       std::map<Edge, TOKEN_UNIT> &minStepSizes);
void findMinimumChannelSz(models::Dataflow *dataflow,
                          std::map<Edge,
                                  BufferInfos> &minChannelSizes); // initial tokens in first element of pair and channel quantity in second element of pair
TOKEN_UNIT findMinimumDistributionSz(std::map<Edge,
        BufferInfos> minChannelSizes);
void initSearchParameters(models::Dataflow *dataflow,
                          std::map<Edge, TOKEN_UNIT> &minStepSizes,
                          std::map<Edge,
                                  BufferInfos> &minChannelSizes);
std::string timeToString(TIME_UNIT t);
StorageDistribution makeMinimalSD(StorageDistribution sd1,
                                  StorageDistribution sd2);
void handleInfeasiblePoint(models::Dataflow* const dataflow,
                           StorageDistributionSet &infeasibleSet,
                           StorageDistributionSet feasibleSet,
                           StorageDistributionSet &kneeSet,
                           StorageDistribution newSD,
                           kperiodic_result_t deps,
                           std::map<Edge, TOKEN_UNIT> &bufferLb);
// BASE MONOTONIC OPTIMISATION FUNCTIONS
void addToExtensions(StorageDistributionSet &extensions,
                     StorageDistribution sd);
StorageDistribution createPoint(models::Dataflow *dataflow,
                                StorageDistribution sd,
                                StorageDistribution hp,
                                TOKEN_UNIT m);
#endif /* BUFFER_SIZING_H_ */
