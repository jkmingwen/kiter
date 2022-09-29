//
// Created by toky on 9/9/22.
//

#ifndef KITER_ABSTRACTDATAFLOW_H
#define KITER_ABSTRACTDATAFLOW_H

#include <vector>
#include <numeric>
#include <string>

namespace models {

    enum EDGE_TYPE {
        NORMAL_EDGE, BUFFERLESS_EDGE, VIRTUAL_EDGE
    };

    enum VERTEX_TYPE {
        NORMAL_VERTEX, PERIODIC_VERTEX
    };

    typedef  unsigned long edge_id_t;
    typedef  unsigned long vertex_id_t;
    typedef  unsigned long graph_id_t;
    typedef unsigned long vertex_degree_t;


    template <typename Vertex, typename Edge>
    class AbstractDataflow {
    public:
        typedef  typename std::vector<Edge>::const_iterator edge_iterator;
        typedef  typename std::vector<Edge>::const_iterator  in_edge_iterator;
        typedef  typename std::vector<Edge>::const_iterator  out_edge_iterator;
        typedef  typename std::vector<Vertex>::const_iterator  vertex_iterator;


    public :
        AbstractDataflow () : read_only (false) {};

        /*
         *  Read only property
         */
    private:
        bool read_only;
    public :
        inline void set_read_only() {this->read_only = true;};
        inline void reset_read_only() {this->read_only = false;};
        inline bool is_read_only() const {return this->read_only;};
        virtual void when_read_only_reset() = 0;



        /*
         * Basic modification operations (Add and remove Edges/Vertex)
         */

        virtual const Vertex &addVertex() = 0;
        virtual const Vertex &addVertex(const vertex_id_t id) = 0;
        virtual const Vertex &addVertex(const vertex_id_t id, const std::string &name) = 0;
        virtual const Vertex &addVertex(const std::string &name) = 0;
        virtual void removeVertex(const Vertex& t) = 0;

        virtual const Edge &addEdge(const Vertex& from, const Vertex& to) = 0;
        virtual const Edge &addEdge(const Vertex& from, const Vertex& to, const edge_id_t id) = 0;
        virtual const Edge &addEdge(const Vertex& from, const Vertex& to, const edge_id_t id, const std::string &name) = 0;
        virtual const Edge &addEdge(const Vertex& from, const Vertex& to, const std::string &name) = 0;
        virtual void removeEdge(const Edge& c) = 0;


        /*
         * Basic accessors operations (Get Edges/Vertex)
         */
        virtual std::pair<edge_iterator, edge_iterator> getEdges() const  = 0;
        virtual std::pair<vertex_iterator, vertex_iterator> getVertices() const = 0;
        virtual std::pair<in_edge_iterator, in_edge_iterator> getInputEdges(const Vertex &t) const = 0;
        virtual std::pair<out_edge_iterator, out_edge_iterator> getOutputEdges(const Vertex &t) const = 0;

        virtual const Vertex& getFirstVertex() const = 0;
        virtual const Edge& getFirstEdge() const = 0;
        virtual const Vertex& getEdgeSource(const Edge& c) const = 0;
        virtual const Vertex& getEdgeTarget(const Edge& c) const = 0;
        virtual size_t getVerticesCount() const = 0;
        virtual size_t getEdgesCount() const = 0;
        virtual vertex_degree_t getVertexDegree(const Vertex &t) const = 0;
        virtual vertex_degree_t getVertexInDegree(const Vertex &t) const = 0;
        virtual vertex_degree_t getVertexOutDegree(const Vertex &t) const = 0;
        virtual bool edgeExist(const Vertex &from, const Vertex &to) const = 0;


        /*
         * Graph properties
         */
    private:
        std::string filename;
        std::string graph_name;
        std::string graph_type;
        std::string app_name;
        graph_id_t graph_id;

    public:
        void setFilename(std::string & f) {this->filename = f;};
        const std::string &getFilename() const {return this->filename;};

        void setGraphName(const std::string &name) {this->graph_name = name;};
        const std::string& getGraphName() const {return this->graph_name;};

        void setGraphType(const std::string &name) {this->graph_type = name;};
        const std::string& getGraphType() const {return this->graph_type;};

        void setAppName(const std::string &name) {this->app_name = name;};
        const std::string& getAppName() const {return this->app_name;};

        void setId(graph_id_t id) {this->graph_id = id;};
        graph_id_t getId() const  {return this->graph_id;};


        /*
         * Basic Vertex and edge properties (ID)
         */

        virtual vertex_id_t getVertexId(const Vertex &t) const = 0;
        virtual edge_id_t getEdgeId(const Edge &c) const = 0;

        virtual const Vertex &getVertexById(const vertex_id_t id) const = 0;
        virtual const Edge &getEdgeById(const edge_id_t id) const = 0;

        virtual void setVertexId(const Vertex &t, const vertex_id_t id) = 0;
        virtual void setEdgeId(const Edge &c, const edge_id_t id) = 0;


        /*
         * Basic Vertex and edge properties (NAME)
         */

        virtual void setVertexName(const Vertex &t,
                           const std::string &name) = 0;
        virtual void setEdgeName(const Edge &c,
                         const std::string &name) = 0;
        virtual const std::string &getVertexName(const Vertex &t) const = 0;
        virtual const Vertex &getVertexByName(const std::string &s) const = 0;
        virtual const std::string &getEdgeName(const Edge &c) const = 0;
        virtual const Edge &getEdgeByName(const std::string &s) const = 0;

//        /*
//         * Basic Vertex and edge properties (TYPE)
//         */
//
//        void setEdgeType(const Edge e,
//                         const EDGE_TYPE t);
//        EDGE_TYPE getEdgeType(const Edge e) const;
//        std::string getEdgeTypeStr(const Edge e) const;
//        void setVertexType(const Vertex v,
//                           const VERTEX_TYPE t);
//        VERTEX_TYPE getVertexType(const Vertex v) const;
//
//
//
//
//        /*
//         * Rwentrancy
//         */
//
//        void setReentrancyFactor(const Vertex t,
//                                 const EXEC_COUNT r);
//        EXEC_COUNT getReentrancyFactor(const Vertex t) const;
//
//        /*
//         * Marking
//         */
//
//        TOKEN_UNIT getPreload(const Edge c) const;
//        void setPreload(const Edge c, const TOKEN_UNIT p);
//        DATA_UNIT getTokenSize(const Edge c) const;
//        void setTokenSize(const Edge c,
//                          const DATA_UNIT ts);
//
//
//        /*
//         * Prod, Cons, and Phases
//         */
//
//        void setPhasesQuantity(const Vertex t,
//                               const EXEC_COUNT phi);
//
//        EXEC_COUNT getPhasesQuantity(const Vertex t) const;
//
//        void setInitPhasesQuantity(const Vertex t,
//                                   const EXEC_COUNT phi);
//
//        EXEC_COUNT getInitPhasesQuantity(const Vertex t) const;
//
//        EXEC_COUNT getEdgeOutPhasesCount(const Edge c) const;
//        EXEC_COUNT getEdgeInPhasesCount(const Edge c) const;
//        EXEC_COUNT getEdgeOutInitPhasesCount(const Edge c) const;
//        EXEC_COUNT getEdgeInInitPhasesCount(const Edge c) const;
//        TOKEN_UNIT getEdgeOut(const Edge c) const;
//
//        const std::vector<TOKEN_UNIT> &getEdgeOutVector(const Edge c) const;
//        const std::vector<TOKEN_UNIT> &getEdgeInVector(const Edge c) const;
//        const std::vector<TOKEN_UNIT> &getEdgeInitOutVector(const Edge c) const;
//        const std::vector<TOKEN_UNIT> &getEdgeInitInVector(const Edge c) const;
//        TOKEN_UNIT getEdgeOutPhase(const Edge c, PHASE_INDEX k) const;
//        void setEdgeOutPhases(const Edge c,
//                              std::vector<TOKEN_UNIT> l);
//        TOKEN_UNIT getEdgeOutInitPhase(const Edge c, PHASE_INDEX k) const;
//        void setEdgeOutInitPhases(const Edge c,
//                                  std::vector<TOKEN_UNIT> l);
//        TOKEN_UNIT getEdgeIn(const Edge c) const;
//        TOKEN_UNIT getEdgeInPhase(const Edge c, PHASE_INDEX k) const;
//        void setEdgeInPhases(const Edge c,
//                             std::vector<TOKEN_UNIT> l);
//        TOKEN_UNIT getEdgeInInitPhase(const Edge c, PHASE_INDEX k) const;
//        void setEdgeInInitPhases(const Edge c,
//                                 std::vector<TOKEN_UNIT> l);
//
//
//        /*
//         * Ports
//         */
//        const std::string getEdgeInputPortName(const Edge c) const;
//        void setEdgeInputPortName(const Edge c,
//                                  const std::string name);
//        const std::string getEdgeOutputPortName(const Edge c) const;
//        void setEdgeOutputPortName(const Edge c,
//                                   const std::string name);
//        Edge getInputEdgeByPortName(const Vertex t, const std::string &name) const;
//        Edge getOutputEdgeByPortName(const Vertex t, const std::string &name) const;
//
//
//
//        /*
//         * Durations
//         */
//
//        const std::vector<TIME_UNIT> &getVertexPhaseDuration(const Vertex t) const;
//        const std::vector<TIME_UNIT> &getVertexInitPhaseDuration(const Vertex t) const;
//        TIME_UNIT getVertexTotalDuration(const Vertex t) const;
//        TIME_UNIT getVertexDuration(const Vertex t) const;
//        TIME_UNIT getVertexDuration(const Vertex t, EXEC_COUNT k) const;
//        TIME_UNIT getVertexInitDuration(const Vertex t, EXEC_COUNT k) const;
//        void setVertexDuration(const Vertex t, std::vector<TIME_UNIT> l);
//        void setVertexInitDuration(const Vertex t, std::vector<TIME_UNIT> l);
//
//        /**
//         * GCDA work
//         */
//        TOKEN_UNIT computeFineGCD(Edge c) const;
//        TOKEN_UNIT getFineGCD(Edge c) const;
//        TOKEN_UNIT getFineGCD(Edge c);
//
//
//        /**
//         * Normalized access
//         */
//        TOKEN_UNIT getNormMop(const Edge c);
//        void setZi(const Vertex t, const TOKEN_UNIT Zi);
//        TOKEN_UNIT getZi(const Vertex t) const;
//        TOKEN_FRACT getAlpha(const Edge c) const;
//        void setAlpha(const Edge c, const TOKEN_FRACT a);
//
//        void set_normalize();
//        bool is_normalized() const;
//
//        /*
//         *
//         * Repetition vector / consistency
//         *
//         */
//
//        void set_repetition_vector();
//        bool has_repetition_vector() const;
//        bool is_consistent();
//
//        void setNi(const Vertex t,
//                   const EXEC_COUNT Ni);
//
//        EXEC_COUNT getNi(const Vertex t) const;
//
//        /*
//         * Mapping related
//         */
//
//
//        const NoC &getNoC() const;
//        void setNoC(NoC &noc);
//
//        void setRoute(const Edge c, const route_t &route);
//        const route_t &getRoute(const Edge c) const;
//
//        void setMapping(const Vertex t,   const node_id_t core_id);
//        node_id_t getMapping(const Vertex t) const;

    };

} // end of namespace models


#endif //KITER_ABSTRACTDATAFLOW_H
