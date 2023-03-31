//
// Created by toky on 31/3/23.
//

#include <algorithms/dse/StorageDistribution.h>
#include "algorithms/schedulings.h"

StorageDistribution::StorageDistribution()
        :dataflow{NULL}, thr{0},cumulativeTime{0},executionTime{0}, channel_quantities(), distribution_size{0} {
}
StorageDistribution::StorageDistribution(const models::Dataflow* dataflow)
        :dataflow{dataflow}, thr{0},cumulativeTime{0},executionTime{0}, channel_quantities(), distribution_size{0} {

    {ForEachEdge(dataflow, c) {
            channel_quantities[c].preload = dataflow->getPreload(c);
            channel_quantities[c].buffer_size = 0;
        }}
}

StorageDistribution::StorageDistribution(const models::Dataflow* dataflow,
                                         TIME_UNIT thr,
                                         std::map<Edge, BufferInfos> channel_quantities)
        :dataflow{dataflow}, thr{thr}, channel_quantities{channel_quantities},
         distribution_size{distribution_size} {
    {ForEachEdge(dataflow, c) {
            VERBOSE_ASSERT_EQUALS(channel_quantities[c].preload, dataflow->getPreload(c));
        }}
    this->updateDistributionSize();
}

// Set edge to given quantity
void StorageDistribution::setChannelQuantity(Edge e,
                                             TOKEN_UNIT quantity) {
    this->channel_quantities[e].buffer_size = quantity;
    this->updateDistributionSize();
}

// Set initial token count of edge
void StorageDistribution::setInitialTokens(Edge e,
                                           TOKEN_UNIT token_count) {
    VERBOSE_ERROR("This should never be permitted");
    VERBOSE_FAILURE();
    this->channel_quantities[e].preload = token_count;
}


// Set distribution size of storage distribution
void StorageDistribution::setDistributionSize(TOKEN_UNIT sz) {
    VERBOSE_ERROR("This should never be permitted");
    VERBOSE_FAILURE();
    this->distribution_size = sz;
}

// Set throughput of storage distribution (0 lower bound)
void StorageDistribution::setThroughput(TIME_UNIT thr) {
    this->thr = std::max(TIME_UNIT(0.0), thr);
}

// Return channel quantity of edge
TOKEN_UNIT StorageDistribution::getChannelQuantity(Edge e) const {
    assert(channel_quantities.find(e) != channel_quantities.end()); // edge quantity not in map
    return this->channel_quantities.at(e).buffer_size;
}
// Indicate if the Edge is set
bool StorageDistribution::hasEdge(Edge e) const {
    return (channel_quantities.find(e) != channel_quantities.end()); // edge in map
}

// Return initial token count of edge
TOKEN_UNIT StorageDistribution::getInitialTokens(Edge e) const {
    assert(channel_quantities.find(e) != channel_quantities.end()); // edge quantity not in map
    return this->channel_quantities.at(e).preload;
}

// Return distribution size of storage distribution
TOKEN_UNIT StorageDistribution::getDistributionSize() const {
    return this->distribution_size;
}

// Return throughput of storage distribution
TIME_UNIT StorageDistribution::getThroughput() const {
    return this->thr;
}

// Return number of edges
ARRAY_INDEX StorageDistribution::getEdgeCount() const {
    return this->dataflow->getEdgesCount();
}

// Return set of edges
std::vector<Edge> StorageDistribution::getEdges() const {
    std::vector<Edge> edgeSet;
    for (auto it = this->channel_quantities.begin();
         it != this->channel_quantities.end(); it++) {
        edgeSet.push_back(it->first);
    }
    return edgeSet;
}



// Storage distributions are equal if every channel has the same quantity
bool StorageDistribution::operator==(const StorageDistribution& distribution) const {
    assert(this->getEdgeCount() == distribution.getEdgeCount());

    for (auto it = this->channel_quantities.begin();
         it != this->channel_quantities.end(); it++) {
        if (this->getChannelQuantity(it->first) !=
            distribution.getChannelQuantity(it->first)) {
            return false;
        }
    }
    return true;
}

bool StorageDistribution::operator!=(const StorageDistribution& distribution) const {
    // NOTE: could this overload be simplified to return !(this == distribution)?
    assert(this->getEdgeCount() == distribution.getEdgeCount());

    for (auto it = this->channel_quantities.begin();
         it != this->channel_quantities.end(); it++) {
        if (this->getChannelQuantity(it->first) !=
            distribution.getChannelQuantity(it->first)) {
            return true;
        }
    }
    return false;
}

/*
 * Checks if distribution size has changed and updates it accordingly
 * Called whenever channel quantity has been altered with setChannelQuantity
 */
void StorageDistribution::updateDistributionSize() {
    TOKEN_UNIT new_dist_sz = 0;
    for (auto it = this->channel_quantities.begin();
         it != this->channel_quantities.end(); it++) {
        new_dist_sz += it->second.buffer_size;
    }
    if (new_dist_sz != this->getDistributionSize()) {
        this->distribution_size = new_dist_sz;
    }
}

// convert TIME_UNIT to string, accounting for really small numbers
std::string timeToString(TIME_UNIT t)
{
    std::stringstream s;
    s << std::setprecision(6) << t;
    return s.str();
}
// Prints member data of StorageDistribution for debugging
std::string StorageDistribution::printInfo() const {
    std::string sdInfo;
    ARRAY_INDEX ch_count = 0;

    sdInfo += "\tCurrent StorageDistribution info:\n";
    sdInfo += "\tNumber of edges: " + std::to_string(this->getEdgeCount()) + "\n";
    sdInfo += "\tChannel quantities stored in the data structure:\n";
    for (auto it = this->channel_quantities.begin();
         it != this->channel_quantities.end(); it++) {
        if (!ch_count) {
            sdInfo += "\t";
        }
        sdInfo += std::to_string(it->second.buffer_size) + " ";
        ch_count++;
    }
    sdInfo += "\n\tChannel quantities using the dataflow edges:\n";
    {ForEachEdge(dataflow, c) {
            if (!ch_count) {
                sdInfo += "\t";
            }
            if (this->hasEdge(c)) {
                sdInfo += std::to_string(this->getChannelQuantity(c)) + " ";
            } else {
                sdInfo += "X ";
            }
            ch_count++;
            if (ch_count == (getEdgeCount() / 2)) {
                sdInfo += "\n\t";
            }
        }}
    sdInfo += "\n"; // double line return to mark end of channel quantities
    sdInfo += "\tDistribution size: " + std::to_string(this->getDistributionSize()) + "\n";
    sdInfo += "\tThroughput: " + timeToString(this->getThroughput()) + "\n";

    return sdInfo;
}

std::string StorageDistribution::print_quantities_csv() const{
    std::string output("\"");
    std::string delim("");
    {ForEachEdge(dataflow, c) {
            output += delim;
            output += std::to_string(this->getChannelQuantity(c));
            delim = ",";

        }}
    output += "\"";
    return output;
}

// prints mask of critical channels identified in storage distribution where 1 = channel
// associated with critical cycle
std::string StorageDistribution::print_dependency_mask(kperiodic_result_t const result) const{
    std::string output("\"");
    std::string delim("");

    {ForEachEdge(dataflow, c) {
            output += delim;
            if (result.critical_edges.find(c) != result.critical_edges.end()) {
                output += "1";
            } else {
                output += "0";
            }
            delim = ",";
        }}
    output += "\"";
    return output;
}


// BASE MONOTONIC OPTIMISATION FUNCTIONS
/* returns true if given SD is within the backward cone of the SD
   (i.e. given SD is not maximal in set) */
bool StorageDistribution::inBackConeOf(StorageDistribution checkDist) {
    for (auto &e : this->getEdges()) {
        if (this->getChannelQuantity(e) > checkDist.getChannelQuantity(e)) {
            return false;
        }
    }
    return true;
}

/* returns true if given SD is within the forward cone of the SD
   (i.e. given SD is not minimal in set) */
bool StorageDistribution::inForConeOf(StorageDistribution checkDist) {
    for (auto &e : this->getEdges()) {
        if (this->getChannelQuantity(e) < checkDist.getChannelQuantity(e)) {
            return false;
        }
    }
    return true;
}
void StorageDistribution::updateGraph(models::Dataflow *dataflow){
    dataflow->reset_computation(); // make graph writeable to alter channel size
    {ForEachEdge(dataflow, c) {
            if (dataflow->getEdgeType(c) == FEEDBACK_EDGE) { // only modelled buffer preloads change
                dataflow->setPreload(c, (this->getChannelQuantity(c) - this->getInitialTokens(c)));
            }
        }}
}


// Print a DOT file of the given graph modelled with a storage distribution
std::string StorageDistribution::printGraph() const{
    // copy graph to model
    /* NOTE doesn't work with a copy:
       can't find channel quantity for given edges  */
    // models::Dataflow* modelledGraph = new models::Dataflow(*dataflow);
    //  dataflow->reset_computation();
    //  {ForEachEdge(dataflow, c) {
    //      // FIXME feels kinda hacky to be using half of the edge count to check for modelled edges
    //      if (dataflow->getEdgeId(c) > (dataflow->getEdgesCount() / 2)) { /* don't deal with non-modelled edges
    //                                                                         as those quantities don't change */
    //        dataflow->setPreload(c, (this->getChannelQuantity(c) -
    //                                 this->getInitialTokens(c))); // subtract initial tokens in buffer
    //      }
    //    }}
    //  return printers::GenerateGraphDOT(dataflow);
    // FIXME: This is wrong
    VERBOSE_FAILURE();
}

std::ostream& operator<<(std::ostream& out, const StorageDistribution& f)
{
    return out << f.print_quantities_csv();
}
