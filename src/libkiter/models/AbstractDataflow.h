//
// Created by toky on 9/9/22.
//

#ifndef KITER_ABSTRACTDATAFLOW_H
#define KITER_ABSTRACTDATAFLOW_H


#include <vector>
#include <numeric>
#include <string>
#include <commons/commons.h>
#include <models/NoC.h>


/* Dataflow defintion */

enum EDGE_TYPE {
    NORMAL_EDGE, BUFFERLESS_EDGE, VIRTUAL_EDGE
};
typedef  std::string VERTEX_TYPE;
//enum VERTEX_TYPE {NORMAL_VERTEX, PERIODIC_VERTEX};


struct Vertex {};
struct Edge {};
class edge_iterator {};
class in_edge_iterator {};
class out_edge_iterator {};
class vertex_iterator {};

namespace models {

    class AbstractDataflow {


        /* all about getters */
    public:


    public :
        Dataflow(unsigned int nVertex = 0);


        /*
         * Problematic ones
         */

        void set_read_only();
        void reset_computation();
        bool is_read_only() const;


        /*
         * Basic modification operations (Add and remove Edges/Vertex)
         */

        Edge addEdge(const Vertex from, const Vertex to);
        Edge addEdge(const Vertex from, const Vertex to, const ARRAY_INDEX id);
        Edge addEdge(const Vertex from, const Vertex to, const ARRAY_INDEX id, const std::string &name);
        Edge addEdge(const Vertex from, const Vertex to, const std::string &name);

        Vertex addVertex();
        Vertex addVertex(const ARRAY_INDEX id);
        Vertex addVertex(const ARRAY_INDEX id, const std::string &name);
        Vertex addVertex(const std::string &name);

        void removeVertex(const Vertex t);
        void removeEdge(const Edge c);

        /*
         * Basic accessors operations (Get Edges/Vertex)
         */
        std::pair<edge_iterator, edge_iterator> getEdges() const;
        std::pair<vertex_iterator, vertex_iterator> getVertices() const;
        std::pair<in_edge_iterator, in_edge_iterator> getInputEdges(Vertex t) const;
        std::pair<out_edge_iterator, out_edge_iterator> getOutputEdges(Vertex t) const;

        Vertex getFirstVertex() const;
        Edge getFirstEdge() const;
        Vertex getEdgeSource(const Edge c) const;
        Vertex getEdgeTarget(const Edge c) const;
        ARRAY_INDEX getVerticesCount() const;
        ARRAY_INDEX getEdgesCount() const;
        unsigned int getVertexDegree(const Vertex t) const;
        unsigned int getVertexInDegree(const Vertex t) const;
        unsigned int getVertexOutDegree(const Vertex t) const;

        bool edgeExist(const Vertex from, const Vertex to) const;


        /*
         * Graph properties
         */
        void setFilename(std::string f);
        void setGraphName(const std::string &name);
        void setAppName(const std::string &name);
        void setGraphType(const std::string &name);
        void setId(ARRAY_INDEX id);

        const std::string &getFilename() const;
        ARRAY_INDEX getId() const;
        const std::string getGraphName() const;
        const std::string getGraphType() const;
        const std::string getAppName() const;

        /*
         * Basic Vertex and edge properties (ID)
         */

        ARRAY_INDEX getVertexId(const Vertex t) const;
        ARRAY_INDEX getEdgeId(const Edge c) const;

        Vertex getVertexById(const ARRAY_INDEX id) const;
        Edge getEdgeById(const ARRAY_INDEX id) const;

        void setVertexId(const Vertex t, const ARRAY_INDEX id);
        void setEdgeId(const Edge c, const ARRAY_INDEX id);

        ARRAY_INDEX getMaxEdgeId() const;
        ARRAY_INDEX getMaxVertexId() const;

        /*
         * Basic Vertex and edge properties (NAME)
         */

        void setVertexName(const Vertex t,
                           const std::string name);
        const std::string getVertexName(const Vertex t) const;
        Vertex getVertexByName(const std::string s) const;
        const std::string getEdgeName(const Edge c) const;
        Edge getEdgeByName(const std::string s) const;
        void setEdgeName(const Edge c,
                         const std::string name);

        /*
         * Basic Vertex and edge properties (TYPE)
         */

        void setEdgeType(const Edge e,
                         const EDGE_TYPE t);
        EDGE_TYPE getEdgeType(const Edge e) const;
        std::string getEdgeTypeStr(const Edge e) const;
        void setVertexType(const Vertex v,
                           const VERTEX_TYPE t);
        VERTEX_TYPE getVertexType(const Vertex v) const;




        /*
         * Rwentrancy
         */

        void setReentrancyFactor(const Vertex t,
                                 const EXEC_COUNT r);
        EXEC_COUNT getReentrancyFactor(const Vertex t) const;

        /*
         * Marking
         */

        TOKEN_UNIT getPreload(const Edge c) const;
        void setPreload(const Edge c, const TOKEN_UNIT p);
        DATA_UNIT getTokenSize(const Edge c) const;
        void setTokenSize(const Edge c,
                          const DATA_UNIT ts);


        /*
         * Prod, Cons, and Phases
         */

        void setPhasesQuantity(const Vertex t,
                               const EXEC_COUNT phi);

        EXEC_COUNT getPhasesQuantity(const Vertex t) const;

        void setInitPhasesQuantity(const Vertex t,
                                   const EXEC_COUNT phi);

        EXEC_COUNT getInitPhasesQuantity(const Vertex t) const;

        EXEC_COUNT getEdgeOutPhasesCount(const Edge c) const;
        EXEC_COUNT getEdgeInPhasesCount(const Edge c) const;
        EXEC_COUNT getEdgeOutInitPhasesCount(const Edge c) const;
        EXEC_COUNT getEdgeInInitPhasesCount(const Edge c) const;
        TOKEN_UNIT getEdgeOut(const Edge c) const;

        const std::vector<TOKEN_UNIT> &getEdgeOutVector(const Edge c) const;
        const std::vector<TOKEN_UNIT> &getEdgeInVector(const Edge c) const;
        const std::vector<TOKEN_UNIT> &getEdgeInitOutVector(const Edge c) const;
        const std::vector<TOKEN_UNIT> &getEdgeInitInVector(const Edge c) const;
        TOKEN_UNIT getEdgeOutPhase(const Edge c, PHASE_INDEX k) const;
        void setEdgeOutPhases(const Edge c,
                              std::vector<TOKEN_UNIT> l);
        TOKEN_UNIT getEdgeOutInitPhase(const Edge c, PHASE_INDEX k) const;
        void setEdgeOutInitPhases(const Edge c,
                                  std::vector<TOKEN_UNIT> l);
        TOKEN_UNIT getEdgeIn(const Edge c) const;
        TOKEN_UNIT getEdgeInPhase(const Edge c, PHASE_INDEX k) const;
        void setEdgeInPhases(const Edge c,
                             std::vector<TOKEN_UNIT> l);
        TOKEN_UNIT getEdgeInInitPhase(const Edge c, PHASE_INDEX k) const;
        void setEdgeInInitPhases(const Edge c,
                                 std::vector<TOKEN_UNIT> l);


        /*
         * Ports
         */
        const std::string getEdgeInputPortName(const Edge c) const;
        void setEdgeInputPortName(const Edge c,
                                  const std::string name);
        const std::string getEdgeOutputPortName(const Edge c) const;
        void setEdgeOutputPortName(const Edge c,
                                   const std::string name);
        Edge getInputEdgeByPortName(const Vertex t, const std::string &name) const;
        Edge getOutputEdgeByPortName(const Vertex t, const std::string &name) const;



        /*
         * Durations
         */

        const std::vector<TIME_UNIT> &getVertexPhaseDuration(const Vertex t) const;
        const std::vector<TIME_UNIT> &getVertexInitPhaseDuration(const Vertex t) const;
        TIME_UNIT getVertexTotalDuration(const Vertex t) const;
        TIME_UNIT getVertexDuration(const Vertex t) const;
        TIME_UNIT getVertexDuration(const Vertex t, EXEC_COUNT k) const;
        TIME_UNIT getVertexInitDuration(const Vertex t, EXEC_COUNT k) const;
        void setVertexDuration(const Vertex t, std::vector<TIME_UNIT> l);
        void setVertexInitDuration(const Vertex t, std::vector<TIME_UNIT> l);

        /**
         * GCDA work
         */
        TOKEN_UNIT computeFineGCD(Edge c) const;
        TOKEN_UNIT getFineGCD(Edge c) const;
        TOKEN_UNIT getFineGCD(Edge c);


        /**
         * Normalized access
         */
        TOKEN_UNIT getNormMop(const Edge c);
        void setZi(const Vertex t, const TOKEN_UNIT Zi);
        TOKEN_UNIT getZi(const Vertex t) const;
        TOKEN_FRACT getAlpha(const Edge c) const;
        void setAlpha(const Edge c, const TOKEN_FRACT a);

        void set_normalize();
        bool is_normalized() const;

        /*
         *
         * Repetition vector / consistency
         *
         */


        void set_repetition_vector();
        bool has_repetition_vector() const;
        bool is_consistent();

        void setNi(const Vertex t,
                   const EXEC_COUNT Ni);

        EXEC_COUNT getNi(const Vertex t) const;

        /*
         * Mapping related
         */


        const NoC &getNoC() const;
        void setNoC(NoC &noc);

        void setRoute(const Edge c, const route_t &route);
        const route_t &getRoute(const Edge c) const;

        void setMapping(const Vertex t,   const node_id_t core_id);
        node_id_t getMapping(const Vertex t) const;

    };

} // end of namespace models


#endif //KITER_ABSTRACTDATAFLOW_H
