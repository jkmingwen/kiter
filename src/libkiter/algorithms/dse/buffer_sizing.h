/*
 * buffer_sizing.h
 *
 */

#ifndef BUFFER_SIZING_H_
#define BUFFER_SIZING_H_

#include <models/Dataflow.h>
#include <commons/commons.h>
#include <algorithms/dse/StorageDistributionSet.h>


#define VERBOSE_DEBUG_DSE(m)    if (VERBOSE_IS_DEBUG())    	std::cerr << PURPLE_COLOR  << "[DSE]" << __SHOW_LEVEL << m << std::string(20,' ') << "\n"
#define VERBOSE_DSE(m)    if (VERBOSE_IS_INFO())    	std::cerr << PURPLE_COLOR  << "[DSE]" << __SHOW_LEVEL << m << std::string(20,' ') << "\n"


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
