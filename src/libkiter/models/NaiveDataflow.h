//
// Created by toky on 23/9/22.
//

#ifndef KITER_NAIVEDATAFLOW_H
#define KITER_NAIVEDATAFLOW_H

#include "AbstractDataflow.h"
#include "commons/verbose.h"
#include "commons/commons.h"
#include "commons/iterators.h"
#include <map>
#include <ostream>

#include <models/Dataflow.h>

/**
 *
 * New data structure project
 *  - any modification to the dataflow cancelled the validity of the Edge and Vertex objects (boost was only remove op).
 *    * TODO: possible fix, don't give ref to Vertex/Edges, but a proper index.
 *
 */

namespace models {

    typedef size_t edge_vector_index_t;
    typedef size_t vertex_vector_index_t;

    class NaiveDataflow;
    class NaiveEdgeRef;
    class NaiveVertexRef;
    class NaiveEdgeData;


    class NaiveVertexData {
        friend NaiveDataflow;
        friend NaiveVertexRef;
        friend NaiveEdgeData;

    private:
        vertex_vector_index_t vector_index;
        vertex_id_t  id;
        std:: string name;
        vertex_degree_t in_degree, out_degree;

        explicit NaiveVertexData(vertex_vector_index_t vi) : vector_index (vi), id(0), in_degree(0), out_degree(0) {
        }

    public:
        NaiveVertexData() = default;

        friend std::ostream &operator<<(std::ostream &os, const NaiveVertexData &vertex) {
            os << "vector_index: " << vertex.vector_index << " id: " << vertex.id << " name: " << vertex.name
               << " in_degree: " << vertex.in_degree << " out_degree: " << vertex.out_degree;
            return os;
        }

        bool operator==(const NaiveVertexData &rhs) const {
            return vector_index == rhs.vector_index;
        }

        bool operator!=(const NaiveVertexData &rhs) const {
            return !(rhs == *this);
        }

        DATAFLOW_VERTEX_TYPE _type = NORMAL_VERTEX;
        reentrancy_count_t _reentrancy = 0;
        phase_quantity_t _phase_quantity = 0;
        phase_quantity_t _initial_phase_quantity = 0;
        std::vector<time_unit_t> _init_phase_durations;
        std::vector<time_unit_t> _phase_durations;
        execution_count_t _repetition_vector;
    };



    class NaiveEdgeData {
        friend NaiveDataflow;
        friend NaiveEdgeRef;
    private:
        edge_vector_index_t vector_index;
        vertex_id_t  id;
        std:: string name;
        vertex_vector_index_t from;
        vertex_vector_index_t to;
        explicit NaiveEdgeData(const vertex_vector_index_t& from, const vertex_vector_index_t& to, edge_vector_index_t ei) : vector_index (ei), from(from), to(to) {
        }
    public:
        NaiveEdgeData() = default;

        bool operator==(const NaiveEdgeData &rhs) const {
            return vector_index == rhs.vector_index;
        }

        bool operator!=(const NaiveEdgeData &rhs) const {
            return !(rhs == *this);
        }

        friend std::ostream &operator<<(std::ostream &os, const NaiveEdgeData &edge) {
            os << "vector_index: " << edge.vector_index << " id: " << edge.id << " name: " << edge.name << " from: "
               << edge.from << " to: " << edge.to;
            return os;
        }
    private: // data for edge
        DATAFLOW_EDGE_TYPE _type = NORMAL_EDGE;
        token_quantity_t _preload = 0;
        token_size_t _token_size = 1;
        std::vector<token_quantity_t> out_vector;
        std::vector<token_quantity_t> in_vector;
        std::vector<token_quantity_t> out_init_vector;
        std::vector<token_quantity_t> in_init_vector;
        TOKEN_UNIT out_total;
        TOKEN_UNIT in_total;

        std::string _output_port_name;
        std::string _input_port_name;

    };



    class NaiveEdgeRef {
        friend NaiveDataflow;
        edge_vector_index_t vector_index;
    public: // TODO: turn private later
        NaiveEdgeRef(const NaiveEdgeData& data) : vector_index(data.vector_index) {};
        NaiveEdgeRef(const edge_vector_index_t& ref) : vector_index(ref) {};


        NaiveEdgeRef& operator++() {
            vector_index ++ ;
            return *this;
        }
        NaiveEdgeRef operator++(int) {
            auto r = *this;
            ++(*this);
            return r;
        }

        friend std::ostream &operator<<(std::ostream &os, const NaiveEdgeRef &edge) {
            os << "vector_index: " << edge.vector_index;
            return os;
        }


    };

    class NaiveVertexRef {
        friend NaiveDataflow;
        vertex_vector_index_t vector_index;
    public: // TODO: turn private later
        //NaiveVertexRef(vertex_vector_index_t idx) : vector_index(idx) {};
        NaiveVertexRef(const NaiveVertexData& data) : vector_index(data.vector_index) {};

        NaiveVertexRef& operator++() {
            vector_index ++ ;
            return *this;
        }
        NaiveVertexRef operator++(int) {
            auto r = *this;
            ++(*this);
            return r;
        }

        bool operator>(NaiveVertexRef& other) {return this->vector_index > other.vector_index;}

        friend bool operator==( NaiveVertexRef const& lhs, NaiveVertexRef const& rhs ) {
            return lhs.vector_index  == rhs.vector_index;
        }
        friend bool operator!=( NaiveVertexRef const& lhs, NaiveVertexRef const& rhs ) {
            return !(lhs==rhs);
        }
        friend bool operator<( NaiveVertexRef const& lhs, NaiveVertexRef const& rhs ) {
            return (lhs.vector_index<rhs.vector_index);
        }

        friend std::ostream &operator<<(std::ostream &os, const NaiveVertexRef &vertex) {
            os << "vector_index: " << vertex.vector_index;
            return os;
        }

    };

    class NaiveDataflow : public AbstractDataflow <NaiveVertexRef, NaiveEdgeRef,
            incremental_iterator<NaiveVertexRef>,
            incremental_iterator<NaiveEdgeRef>,
            safe_iterator<edge_vector_index_t , NaiveEdgeRef>,
            safe_iterator<edge_vector_index_t , NaiveEdgeRef> > {

    public:
        NaiveDataflow () : AbstractDataflow() , vertex_ordered_list(0), edge_ordered_list(0)
                , vertex_input_ordered_list(0), vertex_output_ordered_list(0)  {
        }

        NaiveDataflow (models::Dataflow& g) : AbstractDataflow() , vertex_ordered_list(0), edge_ordered_list(0)
                , vertex_input_ordered_list(0), vertex_output_ordered_list(0)  {
            for (auto v : g.vertices()) {
                auto new_vertex = this->addVertex(g.getVertexId(v), g.getVertexName(v));
                this->setPhasesQuantity(new_vertex, g.getPhasesQuantity(v));
                this->setInitPhasesQuantity(new_vertex, g.getInitPhasesQuantity(v));
                this->setVertexVectorDuration(new_vertex, (const std::vector<double> &) g.getVertexPhaseDuration(v));
                this->setVertexInitVectorDuration(new_vertex,
                                                  (const std::vector<double> &) g.getVertexInitPhaseDuration(v));
                this->setReentrancyFactor(new_vertex, g.getReentrancyFactor(v));
                this->setVertexType(new_vertex, DATAFLOW_VERTEX_TYPE::NORMAL_VERTEX);

            }

            {ForEachEdge (&g, e) {
                    auto src = this->getVertexById(g.getVertexId(g.getEdgeSource(e)));
                    auto tgt = this->getVertexById(g.getVertexId(g.getEdgeTarget(e)));
                auto new_edge = this->addEdge(src,tgt, g.getEdgeId(e), g.getEdgeName(e));
                    this->setEdgeInputPortName(new_edge,g.getEdgeInputPortName(e));
                    this->setEdgeOutputPortName(new_edge,g.getEdgeOutputPortName(e));
                    this->setPreload(new_edge,g.getPreload(e));
                    this->setTokenSize(new_edge,g.getTokenSize(e));
                    if (g.getEdgeType(e) == EDGE_TYPE::NORMAL_EDGE) {
                        this->setEdgeType(new_edge, DATAFLOW_EDGE_TYPE::NORMAL_EDGE);
                    } else {
                        VERBOSE_FAILURE();
                    }
                    this->setEdgeInInitVector(new_edge,const_cast<std::vector<token_quantity_t> &>(g.getEdgeInitInVector(e)));
                    this->setEdgeInVector(new_edge,const_cast<std::vector<token_quantity_t> &>(g.getEdgeInVector(e)));
                    this->setEdgeOutInitVector(new_edge,const_cast<std::vector<token_quantity_t> &>(g.getEdgeInitOutVector(e)));
                    this->setEdgeOutVector(new_edge,const_cast<std::vector<token_quantity_t> &>(g.getEdgeOutVector(e)));
            }}


        }


        virtual ~NaiveDataflow() = default ;

    private:
        void when_read_only_reset() override {
           // here we reset any cache
        }

    private:
        std::vector<NaiveVertexData> vertex_ordered_list;
        std::vector<NaiveEdgeData> edge_ordered_list;

        std::vector<std::set<edge_vector_index_t>> vertex_input_ordered_list;
        std::vector<std::set<edge_vector_index_t>> vertex_output_ordered_list;

        std::map<vertex_id_t, vertex_vector_index_t> vertex_id_to_index;
        std::map<std::string, vertex_vector_index_t> vertex_name_to_index;

        std::map<edge_id_t, edge_vector_index_t> edge_id_to_index;
        std::map<std::string, edge_vector_index_t> edge_name_to_index;


        vertex_id_t max_vertex_id{};
        vertex_id_t  pickRandomVertexId() {return ++max_vertex_id;}
        std::string  pickRandomVertexName(vertex_id_t id) {return "vertex" + commons::toString(id);}


    public:
        const NaiveVertexRef addVertex() override {
            return addVertex(this->pickRandomVertexId());
        }

        const NaiveVertexRef addVertex(const vertex_id_t id) override {
            return addVertex(id, this->pickRandomVertexName(id));
        }

        const NaiveVertexRef addVertex(const std::string &name) override {
            return addVertex(this->pickRandomVertexId(), name);
        }

        const NaiveVertexRef addVertex(const vertex_id_t id, const std::string &name) override {
            VERBOSE_DEBUG("Add a new vertex with id " << id << " and name " << name);
            VERBOSE_DEBUG("vertex_ordered_list size is " << this->vertex_ordered_list.size());
            this->vertex_ordered_list.emplace_back(NaiveVertexData(this->vertex_ordered_list.size()));
            this->vertex_input_ordered_list.emplace_back();
            this->vertex_output_ordered_list.emplace_back();
            VERBOSE_DEBUG("after emplace vertex_ordered_list size is " << this->vertex_ordered_list.size());
            NaiveVertexData& new_vertex = this->vertex_ordered_list.back();
            VERBOSE_DEBUG("Get new_vertex from the back index " << new_vertex.vector_index);
            this->setVertexName(new_vertex, name);
            VERBOSE_DEBUG("set new_vertex name " << new_vertex.name);
            this->setVertexId(new_vertex, id);
            VERBOSE_DEBUG("set new_vertex id " << new_vertex.id);
            this->max_vertex_id = std::max(this->max_vertex_id,id) + 1;
            VERBOSE_DEBUG("End of add vertex, update max_vertex_id " << this->max_vertex_id);
            return new_vertex;
        }




        // The remove operator is expensive O(n).
        void removeVertex(const NaiveVertexRef &t) override {

            VERBOSE_DEBUG("start to remove a vertex.");

            // remove edges first.
            auto old_vector_index = t.vector_index;


            VERBOSE_DEBUG("remove all the edges connected to it");

            VERBOSE_DEBUG("Get references to inputs and outputs");
            const auto &inputs = vertex_input_ordered_list.at(t.vector_index);
            const auto &outputs = vertex_output_ordered_list.at(t.vector_index);

            // VERBOSE_DEBUG("iterates inputs: " << commons::toString(inputs));
            while (inputs.size()) {
                edge_vector_index_t idx = *inputs.begin();
                VERBOSE_DEBUG("remove edge with index " << idx << " from edge list of size " << edge_ordered_list.size());
                const NaiveEdgeData &e = edge_ordered_list.at(idx);
                VERBOSE_DEBUG("We found the edge, ready to remove");
                this->removeEdge(e);
            }

            // VERBOSE_DEBUG("iterates outputs: " << commons::toString(outputs));
            while (outputs.size()) {
                edge_vector_index_t idx = *outputs.begin();
                const NaiveEdgeData &e = edge_ordered_list.at(idx);
                this->removeEdge(e);
            }



            VERBOSE_DEBUG("remove the vertex from the database");
            NaiveVertexData & data = vertex_ordered_list[t.vector_index];
            vertex_id_to_index.erase(data.id);
            vertex_name_to_index.erase(data.name);
            vertex_ordered_list.erase(vertex_ordered_list.begin() + t.vector_index);
            vertex_output_ordered_list.erase(vertex_output_ordered_list.begin() + t.vector_index);
            vertex_input_ordered_list.erase(vertex_input_ordered_list.begin() + t.vector_index);


            VERBOSE_DEBUG("by removing a vertex, we changed the index of all vertices, ");
            VERBOSE_DEBUG("update everything that point to vertices by index");

            for (auto idx = vertex_ordered_list.begin() + old_vector_index ; idx < vertex_ordered_list.end(); idx ++ ) {
                idx->vector_index -- ;
                vertex_id_to_index.at(idx->id) = idx->vector_index;
                vertex_name_to_index.at(idx->name) = idx->vector_index;

                for (auto edge_id : vertex_output_ordered_list.at(idx->vector_index) ) {
                    edge_ordered_list.at(edge_id).from = idx->vector_index;
                }

                for (auto edge_id : vertex_input_ordered_list.at(idx->vector_index) ) {
                    edge_ordered_list.at(edge_id).to = idx->vector_index;
                }
            }
        }

    private:
        vertex_id_t max_edge_id{};
        vertex_id_t  pickRandomEdgeId() {return ++max_edge_id;}
        std::string  pickRandomEdgeName(edge_id_t id) {return "edge" + id;}
    public:

        const NaiveEdgeRef addEdge(const NaiveVertexRef &from, const NaiveVertexRef &to) override {
            return addEdge(from, to , this->pickRandomEdgeId());
        }

        const NaiveEdgeRef addEdge(const NaiveVertexRef &from, const NaiveVertexRef &to, const edge_id_t id) override {
            return addEdge(from, to , id, pickRandomEdgeName(id));
        }

        const NaiveEdgeRef addEdge(const NaiveVertexRef &from, const NaiveVertexRef &to, const std::string &name) override {
            return addEdge(from, to , this->pickRandomEdgeId(), name);
        }

        const NaiveEdgeRef
        addEdge(const NaiveVertexRef &from, const NaiveVertexRef &to, const edge_id_t id, const std::string &name) override {

            NaiveEdgeData& new_edge = edge_ordered_list.emplace_back(NaiveEdgeData(from.vector_index, to.vector_index, edge_ordered_list.size()));
            this->setEdgeName(new_edge, name);
            this->setEdgeId(new_edge, id);
            max_edge_id = std::max(max_edge_id,id);
            vertex_ordered_list[new_edge.to].in_degree++;
            vertex_ordered_list[new_edge.from].out_degree++;
            vertex_output_ordered_list[new_edge.from].insert(new_edge.vector_index);
            return new_edge;
        }


        void removeEdge(const NaiveEdgeRef &e) override {
            VERBOSE_DEBUG("An edge is going to be removed, index = " << e.vector_index);

            edge_vector_index_t old_edge_index = e.vector_index;
            NaiveEdgeData & data = edge_ordered_list[e.vector_index];
            vertex_ordered_list[data.to].in_degree--;
            vertex_ordered_list[data.from].out_degree--;

            VERBOSE_DEBUG("Source index = " << data.from);
            VERBOSE_DEBUG("Target index = " << data.to);
            VERBOSE_DEBUG("vertex ordered list size = " << vertex_ordered_list.size());
            VERBOSE_DEBUG("vertex input ordered list size = " << vertex_input_ordered_list.size());
            VERBOSE_DEBUG("vertex output ordered list size = " << vertex_output_ordered_list.size());

            vertex_input_ordered_list.at(data.to).erase(e.vector_index);
            vertex_output_ordered_list.at(data.from).erase(e.vector_index);


            edge_id_to_index.erase(data.id);
            edge_name_to_index.erase(data.name);
            edge_ordered_list.erase(edge_ordered_list.begin() + e.vector_index);

            VERBOSE_DEBUG("Reordering stage..");

            for (auto idx = edge_ordered_list.begin() + old_edge_index ; idx < edge_ordered_list.end(); idx ++ ) {
                vertex_input_ordered_list[idx->to].erase(idx->vector_index);
                vertex_output_ordered_list[idx->from].erase(idx->vector_index);

                idx->vector_index -- ;

                vertex_input_ordered_list[idx->to].insert(idx->vector_index);
                vertex_output_ordered_list[idx->from].insert(idx->vector_index);

                edge_id_to_index.at(idx->id) = idx->vector_index;
                edge_name_to_index.at(idx->name) = idx->vector_index;
            }
        }

        range_t<edge_iterable> getEdges() const override {
            return {incremental_iterator<NaiveEdgeRef>(edge_ordered_list.at(0),edge_ordered_list.size()), edge_iterable()};
        }

        range_t<vertex_iterable> getVertices() const override {
            return {incremental_iterator<NaiveVertexRef>(vertex_ordered_list.at(0),vertex_ordered_list.size()), vertex_iterable()};

        }

        range_t<in_edge_iterable> getInputEdges(const NaiveVertexRef &t) const override {
            const std::set<edge_vector_index_t> & tmp = vertex_input_ordered_list.at(t.vector_index);
            return {safe_iterator<edge_vector_index_t, NaiveEdgeRef>(tmp), safe_iterator<edge_vector_index_t, NaiveEdgeRef>()};
        }

        range_t<out_edge_iterable> getOutputEdges(const NaiveVertexRef &t) const override {
            const std::set<edge_vector_index_t> & tmp = vertex_output_ordered_list.at(t.vector_index);
            return {safe_iterator<edge_vector_index_t, NaiveEdgeRef>(tmp), safe_iterator<edge_vector_index_t, NaiveEdgeRef>()};
        }

        range_t<inout_edge_iterable> getConnectedEdges(const NaiveVertexRef &t) const override { // TODO: this is ugly
            static std::set<edge_vector_index_t>  tmp = vertex_input_ordered_list.at(t.vector_index);
            tmp.insert(vertex_output_ordered_list.at(t.vector_index).begin(),vertex_output_ordered_list.at(t.vector_index).end());
            return {safe_iterator<edge_vector_index_t, NaiveEdgeRef>(tmp), safe_iterator<edge_vector_index_t, NaiveEdgeRef>()};
        }

        const NaiveVertexRef getFirstVertex() const override {
            return vertex_ordered_list.front();
        }

        const NaiveEdgeRef getFirstEdge() const override {
            return edge_ordered_list.front();
        }

        const NaiveVertexRef getEdgeSource(const NaiveEdgeRef &c) const override {
            return vertex_ordered_list.at(edge_ordered_list.at(c.vector_index).from);
        }

        const NaiveVertexRef getEdgeTarget(const NaiveEdgeRef &c) const override {
            return vertex_ordered_list.at(edge_ordered_list.at(c.vector_index).to);
        }

        size_t getVerticesCount() const override {
            return vertex_ordered_list.size();
        }

        size_t getEdgesCount() const override {
            return edge_ordered_list.size();
        }

        vertex_degree_t getVertexDegree(const NaiveVertexRef &t) const override {
            return  vertex_ordered_list.at(t.vector_index).in_degree + vertex_ordered_list.at(t.vector_index).out_degree;
        }

        vertex_degree_t getVertexInDegree(const NaiveVertexRef &t) const override {
            return vertex_ordered_list.at(t.vector_index).in_degree;
        }

        vertex_degree_t getVertexOutDegree(const NaiveVertexRef &t) const override {
            return vertex_ordered_list.at(t.vector_index).out_degree;
        }


        vertex_id_t getVertexId(const NaiveVertexRef &t) const override {
            return vertex_ordered_list.at(t.vector_index).id;
        }

        edge_id_t getEdgeId(const NaiveEdgeRef &c) const override {
            return edge_ordered_list.at(c.vector_index).id;
        }

        const std::string &getVertexName(const NaiveVertexRef &t) const override {
            return vertex_ordered_list.at(t.vector_index).name;
        }

        const std::string &getEdgeName(const NaiveEdgeRef &c) const override {
            return edge_ordered_list.at(c.vector_index).name;
        }


        void setVertexId(const NaiveVertexRef &t, const vertex_id_t id) override {
            if (vertex_id_to_index.count(id)) {
                VERBOSE_ASSERT(t.vector_index == vertex_id_to_index.at(id), "Cannot have two vertex with the same id");
            }
            vertex_id_to_index[id] = t.vector_index;
            vertex_ordered_list[t.vector_index].id = id;
        }

        const NaiveVertexRef getVertexById(const vertex_id_t id) const override {
            return vertex_ordered_list[vertex_id_to_index.at(id)];
        }

        void setEdgeId(const NaiveEdgeRef &c, const edge_id_t id) override {
            if (edge_id_to_index.count(id)) {
                VERBOSE_ASSERT(c.vector_index == vertex_id_to_index.at(id), "Cannot have two edges with the same id");
            }
            edge_id_to_index[id] = c.vector_index;
            edge_ordered_list[c.vector_index].id = id;
        }

        const NaiveEdgeRef getEdgeById(const edge_id_t id) const override {
            return edge_ordered_list[edge_id_to_index.at(id)];
        }



        void setVertexName(const NaiveVertexRef &t, const std::string &name) override {
            if (vertex_name_to_index.count(name)) {
                VERBOSE_ASSERT(t.vector_index == vertex_name_to_index.at(name), "Cannot have two vertex with the same name");
            }
            vertex_name_to_index[name] = t.vector_index;
            vertex_ordered_list[t.vector_index].name = name;
        }

        void setEdgeName(const NaiveEdgeRef &c, const std::string &name) override {
            if (edge_name_to_index.count(name)) {
                VERBOSE_ASSERT(c.vector_index == edge_name_to_index.at(name), "Cannot have two edges with the same name");
            }
            edge_name_to_index[name] = c.vector_index;
            edge_ordered_list[c.vector_index].name = name;

        }


        const NaiveVertexRef getVertexByName(const std::string &name) const override {
            return vertex_ordered_list[vertex_name_to_index.at(name)];
        }


        const NaiveEdgeRef getEdgeByName(const std::string &name) const override {
            return edge_ordered_list[edge_name_to_index.at(name)];
        }


        bool edgeExist(const NaiveVertexRef &from, const NaiveVertexRef &to) const override {
            for (auto idx : vertex_output_ordered_list.at(from.vector_index)) {
                if (edge_ordered_list.at(idx).to == to.vector_index) return true;
            }
            return false;
        }


#define add_edge_property(function_name,data_type,property_name) \
        data_type get##function_name(const  NaiveEdgeRef &e) const override { return  edge_ordered_list[e.vector_index]._##property_name; } \
        void set##function_name(const NaiveEdgeRef &e, const data_type v) override { edge_ordered_list[e.vector_index]._##property_name = v; }
#define add_vertex_property(function_name,data_type,property_name) \
        data_type get##function_name(const  NaiveVertexRef &e) const override { return  vertex_ordered_list[e.vector_index]._##property_name; } \
        void set##function_name(const NaiveVertexRef &e, const data_type v) override { vertex_ordered_list[e.vector_index]._##property_name = v; }


        add_edge_property(EdgeType, DATAFLOW_EDGE_TYPE, type)
        add_vertex_property(VertexType, DATAFLOW_VERTEX_TYPE, type)
        add_vertex_property(ReentrancyFactor, reentrancy_count_t, reentrancy)
        add_edge_property(Preload, token_quantity_t, preload)
        add_edge_property(TokenSize, token_size_t, token_size)
        add_vertex_property(PhasesQuantity, phase_quantity_t, phase_quantity)
        add_vertex_property(InitPhasesQuantity, phase_quantity_t, initial_phase_quantity)

        phase_quantity_t getEdgeOutPhasesCount(const NaiveEdgeRef &c) const override {
            return getPhasesQuantity(vertex_ordered_list.at(edge_ordered_list.at(c.vector_index).to));
        }

        phase_quantity_t getEdgeOutInitPhasesCount(const NaiveEdgeRef &c) const override {
            return getInitPhasesQuantity(vertex_ordered_list.at(edge_ordered_list.at(c.vector_index).to));
        }

        phase_quantity_t getEdgeInPhasesCount(const NaiveEdgeRef &c) const override {
            return getPhasesQuantity(vertex_ordered_list.at(edge_ordered_list.at(c.vector_index).from));
        }

        phase_quantity_t getEdgeInInitPhasesCount(const NaiveEdgeRef &c) const override {
            return getInitPhasesQuantity(vertex_ordered_list.at(edge_ordered_list.at(c.vector_index).from));
        }


        const std::vector<token_quantity_t> &getEdgeOutVector(const NaiveEdgeRef &c) const override {
            return edge_ordered_list.at(c.vector_index).out_vector;
        }

        void setEdgeOutVector(const NaiveEdgeRef &c, const std::vector<token_quantity_t> &l) override {
            NaiveEdgeData& current_edge  = edge_ordered_list.at(c.vector_index);
            phase_quantity_t q = this->getPhasesQuantity(this->getEdgeTarget(c));
            TOKEN_UNIT total =std::accumulate(l.begin(),l.end(),0);
            current_edge.out_vector = l;
            current_edge.out_total = total;
            if (q > 0) {
                VERBOSE_ASSERT_EQUALS(q, l.size());
            } else {
                this->setPhasesQuantity(this->getEdgeTarget(c),l.size());
            }
        }

        const std::vector<token_quantity_t> &getEdgeOutInitVector(const NaiveEdgeRef &c) const override {
            return edge_ordered_list.at(c.vector_index).out_init_vector;
        }

        void setEdgeOutInitVector(const NaiveEdgeRef &c, std::vector<token_quantity_t> &l) override {
            NaiveEdgeData& current_edge  = edge_ordered_list.at(c.vector_index);
            phase_quantity_t q = this->getInitPhasesQuantity(this->getEdgeTarget(c));
            TOKEN_UNIT total =std::accumulate(l.begin(),l.end(),0);
            current_edge.out_vector = l;
            current_edge.out_total = total;
            if (q > 0) {
                VERBOSE_ASSERT_EQUALS(q, l.size());
            } else {
                this->setPhasesQuantity(this->getEdgeTarget(c),l.size());
            }
        }




        token_quantity_t getEdgeIn(const NaiveEdgeRef c) const override {
            return edge_ordered_list[c.vector_index].in_total;
        }

        token_quantity_t getEdgeOut(const NaiveEdgeRef c) const override {
            return edge_ordered_list[c.vector_index].out_total;
        }


        const std::vector<token_quantity_t> &getEdgeInVector(const NaiveEdgeRef &c) const override {
            return edge_ordered_list[c.vector_index].in_vector;
        }

        void setEdgeInVector(const NaiveEdgeRef &c, const std::vector<token_quantity_t> &l) override {
            NaiveEdgeData& current_edge  = edge_ordered_list.at(c.vector_index);
            phase_quantity_t q = this->getPhasesQuantity(this->getEdgeSource(c));
            TOKEN_UNIT total =std::accumulate(l.begin(),l.end(),0);
            current_edge.in_vector = l;
            current_edge.in_total = total;
            if (q > 0) {
                VERBOSE_ASSERT_EQUALS(q, l.size());
            } else {
                this->setPhasesQuantity(this->getEdgeSource(c),l.size());
            }
        }

        const std::vector<token_quantity_t> &getEdgeInInitVector(const NaiveEdgeRef &c) const override {
            return edge_ordered_list.at(c.vector_index).in_init_vector;
        }

        void setEdgeInInitVector(const NaiveEdgeRef &c, std::vector<token_quantity_t> &l) override {
            NaiveEdgeData& current_edge  = edge_ordered_list.at(c.vector_index);
            phase_quantity_t task_phase_quantity = this->getInitPhasesQuantity(this->getEdgeTarget(c));
            TOKEN_UNIT total =std::accumulate(l.begin(),l.end(),0);
            current_edge.out_vector = l;
            current_edge.out_total = total;
            if (task_phase_quantity > 0) {
                VERBOSE_ASSERT_EQUALS(task_phase_quantity, l.size());
            } else {
                this->setPhasesQuantity(this->getEdgeTarget(c),l.size());
            }
        }

        token_quantity_t getEdgeOutPhase(const NaiveEdgeRef &c, phase_index_t k) const override {
            if (k <= 0 ) {
                return getEdgeOutInitPhase(c, k + getEdgeOutInitVector(c).size());
            }

            return edge_ordered_list.at(c.vector_index).out_vector[k-1];


        }


        token_quantity_t getEdgeInPhase(const NaiveEdgeRef &c, phase_index_t k) const override {
            if (k <= 0 ) {
                return getEdgeInInitPhase(c, k + getEdgeInInitVector(c).size());
            }
            return edge_ordered_list.at(c.vector_index).in_vector[k-1];
        }

        token_quantity_t getEdgeInInitPhase(const NaiveEdgeRef &c, phase_index_t k) const override {
            return getEdgeInInitVector(c).at(k-1);
        }
        token_quantity_t getEdgeOutInitPhase(const NaiveEdgeRef &c, phase_index_t k) const override {
            return getEdgeOutInitVector(c).at(k-1);
        }


        const NaiveEdgeRef getInputEdgeByPortName(const NaiveVertexRef &t, const std::string &name) const override {
            for (const NaiveEdgeRef &e : getInputEdges(t)) {
                if (edge_ordered_list.at(e.vector_index)._output_port_name == name) {
                    return edge_ordered_list[e.vector_index];
                }
            }
            VERBOSE_FAILURE();
        }

        const NaiveEdgeRef getOutputEdgeByPortName(const NaiveVertexRef &t, const std::string &name) const override {
            for (const NaiveEdgeRef &e : getOutputEdges(t)) {
                if (edge_ordered_list.at(e.vector_index)._input_port_name == name) {
                    return edge_ordered_list[e.vector_index];
                }
            }
            VERBOSE_FAILURE();
        }

        const std::string& getEdgeOutputPortName(const NaiveEdgeRef &e) const override{
            return edge_ordered_list[e.vector_index].
                    _output_port_name;
        }
        void setEdgeOutputPortName(const NaiveEdgeRef &e, const std::string& v) override{
            edge_ordered_list[e.vector_index].
                    _output_port_name = v;
        }
        const std::string& getEdgeInputPortName(const NaiveEdgeRef &e) const override{
            return edge_ordered_list[e.vector_index].
                    _input_port_name;
        }
        void setEdgeInputPortName(const NaiveEdgeRef &e, const std::string& v) override{
            edge_ordered_list[e.vector_index].
                    _input_port_name = v;
        }

        time_unit_t getVertexTotalDuration(const NaiveVertexRef &t) const override {
            auto l = getVertexVectorDuration(t);
            return std::accumulate(l.begin(),l.end(),0); // TODO: cache me
        }



        time_unit_t getVertexDuration(const NaiveVertexRef &t, phase_index_t k) const override {
            return getVertexVectorDuration(t).at(k);
        }

        time_unit_t getVertexInitDuration(const NaiveVertexRef &t, phase_index_t k) const override {
            return getVertexInitVectorDuration(t).at(k);
        }

        const std::vector<time_unit_t>& getVertexVectorDuration(const NaiveVertexRef &e) const override {
            return vertex_ordered_list.at(e.vector_index)._phase_durations;
        }

        void setVertexVectorDuration(const NaiveVertexRef &e, const std::vector<time_unit_t>& v) override {
            vertex_ordered_list[e.vector_index]._phase_durations = v;
        }

        const std::vector<time_unit_t>& getVertexInitVectorDuration(const NaiveVertexRef &e) const override {
            return vertex_ordered_list.at(e.vector_index)._init_phase_durations;
        }

        void setVertexInitVectorDuration(const NaiveVertexRef &e, const std::vector<time_unit_t>& v) override {
            vertex_ordered_list[e.vector_index]._init_phase_durations = v;
        }


        void setNi(const NaiveVertexRef t, const execution_count_t Ni) override {
            vertex_ordered_list.at(t.vector_index)._repetition_vector = Ni;
        }



    };

} // end of namespace models


#endif //KITER_NAIVEDATAFLOW_H
