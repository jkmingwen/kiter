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

        VERTEX_TYPE _type = NORMAL_VERTEX;
        reentrancy_count_t _reentrancy = 0;
        phase_quantity_t _phase_quantity = 0;
        phase_quantity_t _initial_phase_quantity = 0;
        std::vector<time_unit_t> _init_phase_durations;
        std::vector<time_unit_t> _phase_durations;
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
        explicit NaiveEdgeData(const NaiveVertexData& from, const NaiveVertexData& to, edge_vector_index_t ei) : vector_index (ei), from(from.vector_index), to(to.vector_index) {
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
        EDGE_TYPE _type = NORMAL_EDGE;
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
    public: // TODO: turn private later
        edge_vector_index_t vector_index;
        NaiveEdgeRef(const NaiveEdgeData& data) : vector_index(data.vector_index) {};
    };

    class NaiveVertexRef {
    public: // TODO: turn private later
        vertex_vector_index_t vector_index;
        NaiveVertexRef(vertex_vector_index_t idx) : vector_index(idx) {};
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

    };

    class NaiveDataflow : public AbstractDataflow <NaiveVertexData, NaiveEdgeData,
            std::vector<NaiveVertexData>::const_iterator,
            std::vector<NaiveEdgeData>::const_iterator > {

    public:
        NaiveDataflow () : AbstractDataflow() , vertex_ordered_list(0), edge_ordered_list(0)
        , vertex_input_ordered_list(0), vertex_output_ordered_list(0)  {
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
        const NaiveVertexData &addVertex() override {
            return addVertex(this->pickRandomVertexId());
        }

        const NaiveVertexData &addVertex(const vertex_id_t id) override {
            return addVertex(id, this->pickRandomVertexName(id));
        }

        const NaiveVertexData &addVertex(const std::string &name) override {
            return addVertex(this->pickRandomVertexId(), name);
        }

        const NaiveVertexData &addVertex(const vertex_id_t id, const std::string &name) override {
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
        void removeVertex(const NaiveVertexData &t) override {

            VERBOSE_DEBUG("start to remove a vertex.");

            // remove edges first.
            auto old_vector_index = t.vector_index;


            VERBOSE_DEBUG("remove all the edges connected to it");

            VERBOSE_DEBUG("Get references to inputs and outputs");
            const auto &inputs = vertex_input_ordered_list.at(t.vector_index);
            const auto &outputs = vertex_output_ordered_list.at(t.vector_index);

            VERBOSE_DEBUG("iterates inputs: " << commons::toString(inputs));
            while (inputs.size()) {
                edge_vector_index_t idx = *inputs.begin();
                VERBOSE_DEBUG("remove edge with index " << idx << " from edge list of size " << edge_ordered_list.size());
                const NaiveEdgeData &e = edge_ordered_list.at(idx);
                VERBOSE_DEBUG("We found the edge, ready to remove");
                this->removeEdge(e);
            }

            VERBOSE_DEBUG("iterates outputs: " << commons::toString(outputs));
            while (outputs.size()) {
                edge_vector_index_t idx = *outputs.begin();
                const NaiveEdgeData &e = edge_ordered_list.at(idx);
                this->removeEdge(e);
            }



            VERBOSE_DEBUG("remove the vertex from the database");

            vertex_id_to_index.erase(t.id);
            vertex_name_to_index.erase(t.name);
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

        const NaiveEdgeData &addEdge(const NaiveVertexData &from, const NaiveVertexData &to) override {
            return addEdge(from, to , this->pickRandomEdgeId());
        }

        const NaiveEdgeData &addEdge(const NaiveVertexData &from, const NaiveVertexData &to, const edge_id_t id) override {
            return addEdge(from, to , id, pickRandomEdgeName(id));
        }

        const NaiveEdgeData &addEdge(const NaiveVertexData &from, const NaiveVertexData &to, const std::string &name) override {
            return addEdge(from, to , this->pickRandomEdgeId(), name);
        }
        const NaiveEdgeData &
        addEdge(const NaiveVertexData &from, const NaiveVertexData &to, const edge_id_t id, const std::string &name) override {

            NaiveEdgeData& new_edge = edge_ordered_list.emplace_back(NaiveEdgeData(from, to, edge_ordered_list.size()));
            this->setEdgeName(new_edge, name);
            this->setEdgeId(new_edge, id);
            max_edge_id = std::max(max_edge_id,id);
            vertex_ordered_list[new_edge.to].in_degree++;
            vertex_ordered_list[new_edge.from].out_degree++;
            vertex_input_ordered_list[new_edge.to].insert(new_edge.vector_index);
            vertex_output_ordered_list[new_edge.from].insert(new_edge.vector_index);
            return new_edge;
        }


        void removeEdge(const NaiveEdgeData &e) override {
            VERBOSE_DEBUG("An edge is going to be removed, index = " << e.vector_index);

            edge_vector_index_t old_edge_index = e.vector_index;

            vertex_ordered_list[e.to].in_degree--;
            vertex_ordered_list[e.from].out_degree--;

            VERBOSE_DEBUG("Source index = " << e.from);
            VERBOSE_DEBUG("Target index = " << e.to);
            VERBOSE_DEBUG("vertex ordered list size = " << vertex_ordered_list.size());
            VERBOSE_DEBUG("vertex input ordered list size = " << vertex_input_ordered_list.size());
            VERBOSE_DEBUG("vertex output ordered list size = " << vertex_output_ordered_list.size());

            vertex_input_ordered_list.at(e.to).erase(e.vector_index);
            vertex_output_ordered_list.at(e.from).erase(e.vector_index);


            edge_id_to_index.erase(e.id);
            edge_name_to_index.erase(e.name);
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
            return {edge_ordered_list.begin(), edge_ordered_list.end()};
        }

        range_t<vertex_iterable> getVertices() const override {
            return {vertex_ordered_list.begin(), vertex_ordered_list.end()};
        }

        range_t<in_edge_iterable> getInputEdges(const NaiveVertexData &t) const override {
            static std::vector<NaiveEdgeData> cache;
            cache.clear();
            for (auto e_idx : vertex_input_ordered_list.at(t.vector_index)) {
                cache.emplace_back(edge_ordered_list.at(e_idx));
            }
            return {cache.begin(), cache.end()};;
        }

        range_t<out_edge_iterable> getOutputEdges(const NaiveVertexData &t) const override {
            static std::vector<NaiveEdgeData> cache;
            cache.clear();
            for (auto e_idx : vertex_output_ordered_list.at(t.vector_index)) {
                cache.emplace_back(edge_ordered_list.at(e_idx));
            }
            return {cache.begin(), cache.end()};;
        }

        const NaiveVertexData &getFirstVertex() const override {
            return vertex_ordered_list.front();
        }

        const NaiveEdgeData &getFirstEdge() const override {
            return edge_ordered_list.front();
        }

        const NaiveVertexData &getEdgeSource(const NaiveEdgeData &c) const override {
            return vertex_ordered_list.at(c.from);
        }

        const NaiveVertexData &getEdgeTarget(const NaiveEdgeData &c) const override {
            return vertex_ordered_list.at(c.to);
        }

        size_t getVerticesCount() const override {
            return vertex_ordered_list.size();
        }

        size_t getEdgesCount() const override {
            return edge_ordered_list.size();
        }

        vertex_degree_t getVertexDegree(const NaiveVertexData &t) const override {
            return  t.in_degree + t.out_degree;
        }

        vertex_degree_t getVertexInDegree(const NaiveVertexData &t) const override {
            return t.in_degree;
        }

        vertex_degree_t getVertexOutDegree(const NaiveVertexData &t) const override {
            return t.out_degree;
        }


        vertex_id_t getVertexId(const NaiveVertexData &t) const override {
            return t.id;
        }

        edge_id_t getEdgeId(const NaiveEdgeData &c) const override {
            return c.id;
        }

        const std::string &getVertexName(const NaiveVertexData &t) const override {
            return t.name;
        }

        const std::string &getEdgeName(const NaiveEdgeData &c) const override {
            return c.name;
        }


        void setVertexId(const NaiveVertexData &t, const vertex_id_t id) override {
            if (vertex_id_to_index.count(id)) {
                VERBOSE_ASSERT(t.vector_index == vertex_id_to_index.at(id), "Cannot have two vertex with the same id");
            }
            vertex_id_to_index[id] = t.vector_index;
            vertex_ordered_list[t.vector_index].id = id;
        }

        const NaiveVertexData &getVertexById(const vertex_id_t id) const override {
            return vertex_ordered_list[vertex_id_to_index.at(id)];
        }

        void setEdgeId(const NaiveEdgeData &c, const edge_id_t id) override {
            if (edge_id_to_index.count(id)) {
                VERBOSE_ASSERT(c.vector_index == vertex_id_to_index.at(id), "Cannot have two edges with the same id");
            }
            edge_id_to_index[id] = c.vector_index;
            edge_ordered_list[c.vector_index].id = id;
        }

        const NaiveEdgeData &getEdgeById(const edge_id_t id) const override {
            return edge_ordered_list[edge_id_to_index.at(id)];
        }



        void setVertexName(const NaiveVertexData &t, const std::string &name) override {
            if (vertex_name_to_index.count(name)) {
                VERBOSE_ASSERT(t.vector_index == vertex_name_to_index.at(name), "Cannot have two vertex with the same name");
            }
            vertex_name_to_index[name] = t.vector_index;
            vertex_ordered_list[t.vector_index].name = name;
        }

        void setEdgeName(const NaiveEdgeData &c, const std::string &name) override {
            if (edge_name_to_index.count(name)) {
                VERBOSE_ASSERT(c.vector_index == edge_name_to_index.at(name), "Cannot have two edges with the same name");
            }
            edge_name_to_index[name] = c.vector_index;
            edge_ordered_list[c.vector_index].name = name;

        }


        const NaiveVertexData &getVertexByName(const std::string &name) const override {
            return vertex_ordered_list[vertex_name_to_index.at(name)];
        }


        const NaiveEdgeData &getEdgeByName(const std::string &name) const override {
            return edge_ordered_list[edge_name_to_index.at(name)];
        }


        bool edgeExist(const NaiveVertexData &from, const NaiveVertexData &to) const override {
            for (auto idx : vertex_output_ordered_list.at(from.vector_index)) {
                if (edge_ordered_list.at(idx).to == to.vector_index) return true;
            }
            return false;
        }


#define add_edge_property(function_name,data_type,property_name) \
        data_type get##function_name(const  NaiveEdgeData &e) const override { return  edge_ordered_list[e.vector_index]._##property_name; } \
        void set##function_name(const NaiveEdgeData &e, const data_type v) override { edge_ordered_list[e.vector_index]._##property_name = v; }
#define add_vertex_property(function_name,data_type,property_name) \
        data_type get##function_name(const  NaiveVertexData &e) const override { return  vertex_ordered_list[e.vector_index]._##property_name; } \
        void set##function_name(const NaiveVertexData &e, const data_type v) override { vertex_ordered_list[e.vector_index]._##property_name = v; }


        add_edge_property(EdgeType, EDGE_TYPE, type)
        add_vertex_property(VertexType, VERTEX_TYPE, type)
        add_vertex_property(ReentrancyFactor, reentrancy_count_t, reentrancy)
        add_edge_property(Preload, token_quantity_t, preload)
        add_edge_property(TokenSize, token_size_t, token_size)
        add_vertex_property(PhasesQuantity, phase_quantity_t, phase_quantity)
        add_vertex_property(InitPhasesQuantity, phase_quantity_t, initial_phase_quantity)

        phase_quantity_t getEdgeOutPhasesCount(const NaiveEdgeData &c) const override {
            return getPhasesQuantity(vertex_ordered_list.at(c.to));
        }

        phase_quantity_t getEdgeOutInitPhasesCount(const NaiveEdgeData &c) const override {
            return getInitPhasesQuantity(vertex_ordered_list.at(c.to));
        }

        phase_quantity_t getEdgeInPhasesCount(const NaiveEdgeData &c) const override {
            return getPhasesQuantity(vertex_ordered_list.at(c.from));
        }

        phase_quantity_t getEdgeInInitPhasesCount(const NaiveEdgeData &c) const override {
            return getInitPhasesQuantity(vertex_ordered_list.at(c.from));
        }


        const std::vector<token_quantity_t> &getEdgeOutVector(const NaiveEdgeData &c) const override {
            return edge_ordered_list.at(c.vector_index).out_vector;
        }

        void setEdgeOutVector(const NaiveEdgeData &c, const std::vector<token_quantity_t> &l) override {
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

        const std::vector<token_quantity_t> &getEdgeOutInitVector(const NaiveEdgeData &c) const override {
            return edge_ordered_list.at(c.vector_index).out_init_vector;
        }

        void setEdgeOutInitVector(const NaiveEdgeData &c, std::vector<token_quantity_t> &l) override {
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




        token_quantity_t getEdgeIn(const NaiveEdgeData c) const override {
            return edge_ordered_list[c.vector_index].in_total;
        }

        token_quantity_t getEdgeOut(const NaiveEdgeData c) const override {
            return edge_ordered_list[c.vector_index].out_total;
        }


        const std::vector<token_quantity_t> &getEdgeInVector(const NaiveEdgeData &c) const override {
            return edge_ordered_list[c.vector_index].in_vector;
        }

        void setEdgeInVector(const NaiveEdgeData &c, const std::vector<token_quantity_t> &l) override {
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

        const std::vector<token_quantity_t> &getEdgeInInitVector(const NaiveEdgeData &c) const override {
            return edge_ordered_list.at(c.vector_index).in_init_vector;
        }

        void setEdgeInInitVector(const NaiveEdgeData &c, std::vector<token_quantity_t> &l) override {
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

        token_quantity_t getEdgeOutPhase(const NaiveEdgeData &c, phase_index_t k) const override {
            if (k <= 0 ) {
                return getEdgeOutInitPhase(c, k + getEdgeOutInitVector(c).size());
            }

            return edge_ordered_list.at(c.vector_index).out_vector[k-1];


        }


        token_quantity_t getEdgeInPhase(const NaiveEdgeData &c, phase_index_t k) const override {
            if (k <= 0 ) {
                return getEdgeInInitPhase(c, k + getEdgeInInitVector(c).size());
            }
            return edge_ordered_list.at(c.vector_index).in_vector[k-1];
        }

        token_quantity_t getEdgeInInitPhase(const NaiveEdgeData &c, phase_index_t k) const override {
            return getEdgeInInitVector(c).at(k-1);
        }
        token_quantity_t getEdgeOutInitPhase(const NaiveEdgeData &c, phase_index_t k) const override {
            return getEdgeOutInitVector(c).at(k-1);
        }


        const NaiveEdgeData &getInputEdgeByPortName(const NaiveVertexData &t, const std::string &name) const override {
            for (const NaiveEdgeData &e : getInputEdges(t)) {
                if (e._output_port_name == name) {
                    return edge_ordered_list[e.vector_index];
                }
            }
            VERBOSE_FAILURE();
        }

        const NaiveEdgeData &getOutputEdgeByPortName(const NaiveVertexData &t, const std::string &name) const override {
            for (const NaiveEdgeData &e : getOutputEdges(t)) {
                if (e._input_port_name == name) {
                    return edge_ordered_list[e.vector_index];
                }
            }
            VERBOSE_FAILURE();
        }

        const std::string& getEdgeOutputPortName(const NaiveEdgeData &e) const override{
            return edge_ordered_list[e.vector_index].
                    _output_port_name;
        }
        void setEdgeOutputPortName(const NaiveEdgeData &e, const std::string& v) override{
            edge_ordered_list[e.vector_index].
                    _output_port_name = v;
        }
        const std::string& getEdgeInputPortName(const NaiveEdgeData &e) const override{
            return edge_ordered_list[e.vector_index].
                    _input_port_name;
        }
        void setEdgeInputPortName(const NaiveEdgeData &e, const std::string& v) override{
            edge_ordered_list[e.vector_index].
                    _input_port_name = v;
        }

        time_unit_t getVertexTotalDuration(const NaiveVertexData &t) const override {
            auto l = getVertexVectorDuration(t);
            return std::accumulate(l.begin(),l.end(),0); // TODO: cache me
        }



        time_unit_t getVertexDuration(const NaiveVertexData &t, phase_index_t k) const override {
            return getVertexVectorDuration(t).at(k);
        }

        time_unit_t getVertexInitDuration(const NaiveVertexData &t, phase_index_t k) const override {
            return getVertexInitVectorDuration(t).at(k);
        }

        const std::vector<time_unit_t>& getVertexVectorDuration(const NaiveVertexData &e) const override {
            return vertex_ordered_list.at(e.vector_index)._phase_durations;
        }

        void setVertexVectorDuration(const NaiveVertexData &e, const std::vector<time_unit_t>& v) override {
            vertex_ordered_list[e.vector_index]._phase_durations = v;
        }

        const std::vector<time_unit_t>& getVertexInitVectorDuration(const NaiveVertexData &e) const override {
            return vertex_ordered_list.at(e.vector_index)._init_phase_durations;
        }

        void setVertexInitVectorDuration(const NaiveVertexData &e, const std::vector<time_unit_t>& v) override {
            vertex_ordered_list[e.vector_index]._init_phase_durations = v;
        }





    };

} // end of namespace models


#endif //KITER_NAIVEDATAFLOW_H
