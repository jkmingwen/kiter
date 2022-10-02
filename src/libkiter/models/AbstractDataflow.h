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
    typedef  unsigned long vertex_degree_t;
    typedef  unsigned long reentrancy_count_t;
    typedef  unsigned long token_size_t;
    typedef           long token_quantity_t;
    typedef  unsigned long phase_quantity_t;
    typedef           long phase_index_t;
    typedef         double time_unit_t;


    template <typename Vertex, typename Edge>
    class AbstractDataflow {
    public:
        typedef  typename std::vector<Edge> edge_iterable;
        typedef  typename std::vector<Edge> in_edge_iterable;
        typedef  typename std::vector<Edge> out_edge_iterable;
        typedef  typename std::vector<Vertex> vertex_iterable;


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
        virtual const edge_iterable &getEdges() const  = 0;
        virtual const vertex_iterable & getVertices() const = 0;
        virtual const in_edge_iterable & getInputEdges(const Vertex &t) const = 0;
        virtual const out_edge_iterable & getOutputEdges(const Vertex &t) const = 0;

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

        /*
         * Basic Vertex and edge properties (TYPE)
         */

        virtual void setEdgeType(const Edge &e, const EDGE_TYPE t) = 0;
        virtual void setVertexType(const Vertex &v, const VERTEX_TYPE t) = 0;
        virtual EDGE_TYPE getEdgeType(const Edge &e) const = 0;
        virtual VERTEX_TYPE getVertexType(const Vertex &v) const = 0;

        const std::string& getEdgeTypeStr(const Edge &e) const {
            EDGE_TYPE et = this->getEdgeType(e);
            switch (et) {
                case VIRTUAL_EDGE : return "VIRTUAL_EDGE";
                case NORMAL_EDGE : return "NORMAL_EDGE";
                case  BUFFERLESS_EDGE : return "BUFFERLESS_EDGE";
                default : return "UNKNOWN";
            }
        }





        /*
         * Rwentrancy
         */

        virtual void setReentrancyFactor(const Vertex &t, const reentrancy_count_t r) = 0;
        virtual reentrancy_count_t getReentrancyFactor(const Vertex &t) const = 0;

        /*
         * Marking
         */

        virtual token_quantity_t getPreload(const Edge &c) const = 0;
        virtual void setPreload(const Edge &c, const token_quantity_t p) = 0;
        virtual token_size_t getTokenSize(const Edge &c) const = 0;
        virtual void setTokenSize(const Edge &c, const token_size_t ts) = 0;


        /*
         * Prod, Cons, and Phases
         */


        virtual void setPhasesQuantity(const Vertex &t, const phase_quantity_t phi) = 0;
        virtual void setInitPhasesQuantity(const Vertex &t, const phase_quantity_t phi) = 0;

        virtual phase_quantity_t getPhasesQuantity(const Vertex &t) const = 0;
        virtual phase_quantity_t getInitPhasesQuantity(const Vertex &t) const = 0;

        virtual phase_quantity_t getEdgeOutPhasesCount(const Edge &c) const = 0;
        virtual phase_quantity_t getEdgeOutInitPhasesCount(const Edge &c) const = 0;

        virtual phase_quantity_t getEdgeInPhasesCount(const Edge &c) const = 0;
        virtual phase_quantity_t getEdgeInInitPhasesCount(const Edge &c) const = 0;


        virtual token_quantity_t getEdgeOut(const Edge c) const = 0;
        virtual const std::vector<token_quantity_t> &getEdgeOutVector(const Edge &c) const = 0;
        virtual void setEdgeOutVector(const Edge &c, const std::vector<token_quantity_t> &l) = 0;
        virtual const std::vector<token_quantity_t> &getEdgeOutInitVector(const Edge &c) const = 0;
        virtual void setEdgeOutInitVector(const Edge &c, std::vector<token_quantity_t> &l) = 0;
        virtual token_quantity_t getEdgeOutPhase(const Edge &c, phase_index_t k) const = 0;
        virtual token_quantity_t getEdgeOutInitPhase(const Edge &c, phase_index_t k) const = 0;

        virtual token_quantity_t getEdgeIn(const Edge c) const = 0;
        virtual const std::vector<token_quantity_t> &getEdgeInVector(const Edge &c) const = 0;
        virtual void setEdgeInVector(const Edge &c, const std::vector<token_quantity_t> &l) = 0;
        virtual const std::vector<token_quantity_t> &getEdgeInInitVector(const Edge &c) const = 0;
        virtual void setEdgeInInitVector(const Edge &c, std::vector<token_quantity_t> &l) = 0;
        virtual token_quantity_t getEdgeInPhase(const Edge &c, phase_index_t k) const = 0;
        virtual token_quantity_t getEdgeInInitPhase(const Edge &c, phase_index_t k) const = 0;


        /*
         * Ports
         */
        virtual const std::string &getEdgeInputPortName(const Edge &c) const = 0;
        virtual void setEdgeInputPortName(const Edge &c,const std::string &name) = 0;
        virtual const std::string &getEdgeOutputPortName(const Edge &c) const = 0;
        virtual void setEdgeOutputPortName(const Edge &c,const std::string &name) = 0;
        virtual const Edge &getInputEdgeByPortName(const Vertex &t, const std::string &name) const = 0;
        virtual const Edge &getOutputEdgeByPortName(const Vertex &t, const std::string &name) const = 0;



        /*
         * Durations
         */

        virtual void setVertexVectorDuration(const Vertex &t, const std::vector<time_unit_t> &l)= 0;
        virtual void setVertexInitVectorDuration(const Vertex &t, const std::vector<time_unit_t> &l)= 0;

        virtual const std::vector<time_unit_t> &getVertexVectorDuration(const Vertex &t) const= 0;
        virtual const std::vector<time_unit_t> &getVertexInitVectorDuration(const Vertex &t) const= 0;

        virtual time_unit_t getVertexTotalDuration(const Vertex &t) const= 0;
        virtual time_unit_t getVertexDuration(const Vertex &t, phase_index_t k) const= 0;
        virtual time_unit_t getVertexInitDuration(const Vertex &t, phase_index_t k) const= 0;

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

        /*
         *
         * Repetition vector / consistency
         *
         */

//        void set_repetition_vector();
//        bool has_repetition_vector() const;
//        bool is_consistent();
//
//        void setNi(const Vertex t,
//                   const EXEC_COUNT Ni);
//
//        EXEC_COUNT getNi(const Vertex t) const;

        /*
         * Mapping related
         */


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
