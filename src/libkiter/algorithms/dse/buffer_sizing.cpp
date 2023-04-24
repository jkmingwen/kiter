/*
 * buffer_sizing.cpp
 *
 */


#include "buffer_sizing.h"
#include <models/Dataflow.h>
#include <iostream>


// Returns the minimum step size for each channel in the given dataflow graph
void findMinimumStepSz(models::Dataflow *dataflow,
                       std::map<Edge, TOKEN_UNIT> &minStepSizes) {
    VERBOSE_DEBUG_DSE("Calculating minimal channel step sizes..." );
  {ForEachEdge(dataflow, c) { // get GCD of all possible combinations of rates of production and consumption in channel
      TOKEN_UNIT minStepSz;
      minStepSz = dataflow->getEdgeInVector(c)[0]; // initialise with first value
      for (EXEC_COUNT i = 0; i < dataflow->getEdgeInPhasesCount(c); i++)
        minStepSz = std::gcd(minStepSz, dataflow->getEdgeInVector(c)[i]);
      for (EXEC_COUNT i = 0; i < dataflow->getEdgeOutPhasesCount(c); i++)
        minStepSz = std::gcd(minStepSz, dataflow->getEdgeOutVector(c)[i]);
      minStepSizes[c] = minStepSz;
          VERBOSE_DEBUG_DSE("Min. step size for channel " << dataflow->getEdgeName(c)
                  << ": " << minStepSz );
    }}
}

/* Returns the minimum channel size for each channel for which we might have non-zero throughput
   in the given dataflow graph */
void findMinimumChannelSz(models::Dataflow *dataflow,
                          std::map<Edge,
                                  BufferInfos> &minChannelSizes) {
    VERBOSE_DEBUG_DSE("Calculating minimal channel sizes (for postive throughput)..." );
  
  {ForEachEdge(dataflow, c) {
      // initialise channel size to maximum int size
          minChannelSizes[c].preload = dataflow->getPreload(c);
          minChannelSizes[c].buffer_size = INT_MAX; // NOTE (should use ULONG_MAX but it's a really large value)
      TOKEN_UNIT ratePeriod = (TOKEN_UNIT) std::gcd(dataflow->getEdgeInPhasesCount(c),
                                                            dataflow->getEdgeOutPhasesCount(c));
      
      for (TOKEN_UNIT i = 0; i < ratePeriod; i++) {
        // might want to change variables to p, c, and t for legibility
        TOKEN_UNIT tokensProduced = dataflow->getEdgeInVector(c)[i % dataflow->getEdgeInPhasesCount(c)];
        TOKEN_UNIT tokensConsumed = dataflow->getEdgeOutVector(c)[i % dataflow->getEdgeOutPhasesCount(c)];
        TOKEN_UNIT tokensInitial = dataflow->getPreload(c);
          VERBOSE_DEBUG_DSE("p, c, t: " << tokensProduced << ", "
                    << tokensConsumed << ", " << tokensInitial );
        TOKEN_UNIT lowerBound;

        if (std::gcd(tokensProduced, tokensConsumed)) {
          lowerBound = tokensProduced + tokensConsumed -
            std::gcd(tokensProduced, tokensConsumed) +
            tokensInitial % std::gcd(tokensProduced, tokensConsumed);
        } else {
          lowerBound = tokensProduced + tokensConsumed -
            std::gcd(tokensProduced, tokensConsumed);
        }
        lowerBound = (lowerBound > tokensInitial ? lowerBound : tokensInitial);
        
        // take the lowest bound amongst phases of prod/cons
        if (lowerBound < minChannelSizes[c].buffer_size) {
          minChannelSizes[c].buffer_size = lowerBound;
        }
      }
          VERBOSE_DEBUG_DSE("Minimum channel size for " << dataflow->getEdgeName(c)
                  << ": " << minChannelSizes[c].buffer_size );
    }}
}

/*
 * Returns the distribution size of a given graph by summing the channel quantities 
 * on every channel
 */
TOKEN_UNIT findMinimumDistributionSz(std::map<Edge,
        BufferInfos> minChannelSizes) {
  TOKEN_UNIT minDistributionSize = 0;
  
  for (auto it = minChannelSizes.begin(); it != minChannelSizes.end(); it++) {
    minDistributionSize += it->second.buffer_size;
  }
  
  VERBOSE_DSE("Lower bound distribution size: " << minDistributionSize );
  return minDistributionSize;
}

// Calculates and writes necessary information for DSE algorithm to input arguments
void initSearchParameters(models::Dataflow *dataflow,
                          std::map<Edge, TOKEN_UNIT> &minStepSizes,
                          std::map<Edge,
                                  BufferInfos> &minChannelSizes) {
  findMinimumStepSz(dataflow, minStepSizes);
  findMinimumChannelSz(dataflow, minChannelSizes);
}


// compares 2 storage distributions and returns a SD that is the local minima of the two
// NOTE this assumes that both storage distributions have the same number of edges
StorageDistribution makeMinimalSD(StorageDistribution sd1,
                                  StorageDistribution sd2) {
  StorageDistribution minSD(sd1);
  std::vector<Edge> edgeSet = sd1.getEdges(); // use edges from sd1
  for (auto it = edgeSet.begin(); it != edgeSet.end(); it++) {
    // NOTE might need to set initial tokens to 0
    minSD.setChannelQuantity(*it, std::min(sd1.getChannelQuantity(*it),
                                           sd2.getChannelQuantity(*it)));
  }
  return minSD;
}

