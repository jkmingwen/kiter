/*
 * scc.h
 *
 *  Created on: 1 June 2021
 *      Author: jkmingwen
 */
#ifndef SCC_H_
#define SCC_H_

#include <models/Dataflow.h>

namespace models {
  class Dataflow;
}

namespace algorithms {
  std::map<int, std::vector<ARRAY_INDEX>> computeSCCKosaraju(const models::Dataflow* const dataflow);
  void computeDFSStack(const models::Dataflow* const dataflow,
                       ARRAY_INDEX startId,
                       std::map<ARRAY_INDEX, bool> &visitedActors,
                       std::stack<ARRAY_INDEX> &dfsOrder);
  void dfsTranspose(const models::Dataflow* const dataflow,
                    ARRAY_INDEX startId,
                    std::map<ARRAY_INDEX, bool> &visitedActors,
                    std::vector<ARRAY_INDEX> &sccActors);
}
#endif /* SCC_H_ */
