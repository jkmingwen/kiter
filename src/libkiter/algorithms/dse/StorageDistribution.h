//
// Created by toky on 31/3/23.
//

#ifndef KITER_STORAGEDISTRIBUTION_H
#define KITER_STORAGEDISTRIBUTION_H

#include <models/Dataflow.h>
#include <commons/commons.h>
#include <algorithms/dse/BufferInfos.h>
#include "algorithms/schedulings.h"

namespace models {
    class Dataflow;
}


class StorageDistribution {
public:
    StorageDistribution();
    StorageDistribution(const models::Dataflow* dataflow);
    StorageDistribution(const models::Dataflow* dataflow,
                        TIME_UNIT thr,
                        std::map<Edge, BufferInfos> channel_quantities);
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
    bool dominates(const StorageDistribution& other) const {
        {ForEachEdge(this->getDataflow(), e) {
                if (this->getChannelQuantity(e) < other.getChannelQuantity(e)) {
                    return false;
                }
            }}
        return true;
    }
    bool equals(const StorageDistribution& other) const {
        {ForEachEdge(this->getDataflow(), e) {
                if (this->getChannelQuantity(e) != other.getChannelQuantity(e)) {
                    return false;
                }
            }}
        return true;
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

    static std::string get_csv_header() {
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


    std::string getQuantitiesStr() const {
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
#endif //KITER_STORAGEDISTRIBUTION_H
