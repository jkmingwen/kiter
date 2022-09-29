//
// Created by toky on 23/9/22.
//

#ifndef KITER_NAIVEDATAFLOW_H
#define KITER_NAIVEDATAFLOW_H

#include "AbstractDataflow.h"
#include "commons/verbose.h"
#include "commons/commons.h"
#include <map>
#include <ostream>


namespace models {

    typedef long edge_vector_index_t;
    typedef long vertex_vector_index_t;

    class NaiveDataflow;

    class NaiveVertex {
        friend NaiveDataflow;

    private:
        vertex_vector_index_t vector_index;
        vertex_id_t  id;
        std:: string name;
        vertex_degree_t in_degree, out_degree;

        explicit NaiveVertex(vertex_vector_index_t vi) : vector_index (vi), id(0), in_degree(0), out_degree(0) {
        }

    public:
        NaiveVertex() = default;

        friend std::ostream &operator<<(std::ostream &os, const NaiveVertex &vertex) {
            os << "vector_index: " << vertex.vector_index << " id: " << vertex.id << " name: " << vertex.name
               << " in_degree: " << vertex.in_degree << " out_degree: " << vertex.out_degree;
            return os;
        }

        bool operator==(const NaiveVertex &rhs) const {
            return vector_index == rhs.vector_index;
        }

        bool operator!=(const NaiveVertex &rhs) const {
            return !(rhs == *this);
        }
    };

    class NaiveEdge {
        friend NaiveDataflow;
    private:
        edge_vector_index_t vector_index;
        vertex_id_t  id;
        std:: string name;
        const NaiveVertex *from, *to;
        explicit NaiveEdge(const NaiveVertex& from, const NaiveVertex& to, edge_vector_index_t ei) : vector_index (ei), from(&from), to(&to) {
        }
    public:
        NaiveEdge() = default;

        bool operator==(const NaiveEdge &rhs) const {
            return vector_index == rhs.vector_index;
        }

        bool operator!=(const NaiveEdge &rhs) const {
            return !(rhs == *this);
        }

        friend std::ostream &operator<<(std::ostream &os, const NaiveEdge &edge) {
            os << "vector_index: " << edge.vector_index << " id: " << edge.id << " name: " << edge.name << " from: "
               << edge.from << " to: " << edge.to;
            return os;
        }
    };

    class NaiveDataflow : public AbstractDataflow <NaiveVertex, NaiveEdge> {

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
        std::vector<NaiveVertex> vertex_ordered_list;
        std::vector<NaiveEdge> edge_ordered_list;

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
        const NaiveVertex &addVertex() override {
            return addVertex(this->pickRandomVertexId());
        }

        const NaiveVertex &addVertex(const vertex_id_t id) override {
            return addVertex(id, this->pickRandomVertexName(id));
        }

        const NaiveVertex &addVertex(const std::string &name) override {
            return addVertex(this->pickRandomVertexId(), name);
        }

        const NaiveVertex &addVertex(const vertex_id_t id, const std::string &name) override {
            VERBOSE_DEBUG("Add a new vertex with id " << id << " and name " << name);
            VERBOSE_DEBUG("vertex_ordered_list size is " << this->vertex_ordered_list.size());
            this->vertex_ordered_list.emplace_back(NaiveVertex(this->vertex_ordered_list.size()));
            this->vertex_input_ordered_list.emplace_back();
            this->vertex_output_ordered_list.emplace_back();
            VERBOSE_DEBUG("after emplace vertex_ordered_list size is " << this->vertex_ordered_list.size());
            NaiveVertex& new_vertex = this->vertex_ordered_list.back();
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
        void removeVertex(const NaiveVertex &t) override {

            VERBOSE_DEBUG("start to remove a vertex.");

            // remove edges first.
            auto old_vector_index = t.vector_index;


            VERBOSE_DEBUG("remove all the edges connected to it");

            VERBOSE_DEBUG("Get references to inputs and outputs");
            const auto &inputs = vertex_input_ordered_list.at(t.vector_index);
            const auto &outputs = vertex_output_ordered_list.at(t.vector_index);

            VERBOSE_DEBUG("iterates inputs");
            while (inputs.size()) {
                edge_vector_index_t idx = *inputs.begin();
                const NaiveEdge &e = edge_ordered_list.at(idx);
                this->removeEdge(e);
            }

            VERBOSE_DEBUG("iterates outputs");
            while (outputs.size()) {
                edge_vector_index_t idx = *outputs.begin();
                const NaiveEdge &e = edge_ordered_list.at(idx);
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
            }
        }

    private:
        vertex_id_t max_edge_id{};
        vertex_id_t  pickRandomEdgeId() {return ++max_edge_id;}
        std::string  pickRandomEdgeName(edge_id_t id) {return "edge" + id;}
    public:

        const NaiveEdge &addEdge(const NaiveVertex &from, const NaiveVertex &to) override {
            return addEdge(from, to , this->pickRandomEdgeId());
        }

        const NaiveEdge &addEdge(const NaiveVertex &from, const NaiveVertex &to, const edge_id_t id) override {
            return addEdge(from, to , id, pickRandomEdgeName(id));
        }

        const NaiveEdge &addEdge(const NaiveVertex &from, const NaiveVertex &to, const std::string &name) override {
            return addEdge(from, to , this->pickRandomEdgeId(), name);
        }
        const NaiveEdge &
        addEdge(const NaiveVertex &from, const NaiveVertex &to, const edge_id_t id, const std::string &name) override {

            NaiveEdge& new_edge = edge_ordered_list.emplace_back(NaiveEdge(from, to, edge_ordered_list.size()));
            this->setEdgeName(new_edge, name);
            this->setEdgeId(new_edge, id);
            max_edge_id = std::max(max_edge_id,id);
            vertex_ordered_list[new_edge.to->vector_index].in_degree++;
            vertex_ordered_list[new_edge.from->vector_index].out_degree++;
            vertex_input_ordered_list[new_edge.to->vector_index].insert(new_edge.vector_index);
            vertex_output_ordered_list[new_edge.from->vector_index].insert(new_edge.vector_index);
            return new_edge;
        }


        void removeEdge(const NaiveEdge &e) override {
            edge_vector_index_t old_edge_index = e.vector_index;

            vertex_ordered_list[e.to->vector_index].in_degree--;
            vertex_ordered_list[e.from->vector_index].out_degree--;

            vertex_input_ordered_list.at(e.to->vector_index).erase(e.vector_index);
            vertex_output_ordered_list.at(e.from->vector_index).erase(e.vector_index);


            edge_id_to_index.erase(e.id);
            edge_name_to_index.erase(e.name);
            edge_ordered_list.erase(edge_ordered_list.begin() + e.vector_index);

            for (auto idx = edge_ordered_list.begin() + old_edge_index ; idx < edge_ordered_list.end(); idx ++ ) {
                vertex_input_ordered_list[idx->to->vector_index].erase(idx->vector_index);
                vertex_output_ordered_list[idx->from->vector_index].erase(idx->vector_index);

                idx->vector_index -- ;

                vertex_input_ordered_list[idx->to->vector_index].insert(idx->vector_index);
                vertex_output_ordered_list[idx->from->vector_index].insert(idx->vector_index);

                edge_id_to_index.at(idx->id) = idx->vector_index;
                edge_name_to_index.at(idx->name) = idx->vector_index;
            }
        }

        std::pair<edge_iterator, edge_iterator> getEdges() const override {
            return {edge_ordered_list.begin(), edge_ordered_list.end()};
        }

        std::pair<vertex_iterator, vertex_iterator> getVertices() const override {
            return {vertex_ordered_list.begin(), vertex_ordered_list.end()};
        }

        std::pair<in_edge_iterator, in_edge_iterator> getInputEdges(const NaiveVertex &t) const override {
            static std::vector<NaiveEdge> cache;
            cache.clear();
            for (auto e_idx : vertex_input_ordered_list.at(t.vector_index)) {
                cache.emplace_back(edge_ordered_list.at(e_idx));
            }
            return {cache.begin(),cache.end()};
        }

        std::pair<out_edge_iterator, out_edge_iterator> getOutputEdges(const NaiveVertex &t) const override {
            static std::vector<NaiveEdge> cache;
            cache.clear();
            for (auto e_idx : vertex_output_ordered_list.at(t.vector_index)) {
                cache.emplace_back(edge_ordered_list.at(e_idx));
            }
            return {cache.begin(),cache.end()};
        }

        const NaiveVertex &getFirstVertex() const override {
            return vertex_ordered_list.front();
        }

        const NaiveEdge &getFirstEdge() const override {
            return edge_ordered_list.front();
        }

        const NaiveVertex &getEdgeSource(const NaiveEdge &c) const override {
            return *c.from;
        }

        const NaiveVertex &getEdgeTarget(const NaiveEdge &c) const override {
            return *c.to;
        }

        size_t getVerticesCount() const override {
            return vertex_ordered_list.size();
        }

        size_t getEdgesCount() const override {
            return edge_ordered_list.size();
        }

        vertex_degree_t getVertexDegree(const NaiveVertex &t) const override {
            return  t.in_degree + t.out_degree;
        }

        vertex_degree_t getVertexInDegree(const NaiveVertex &t) const override {
            return t.in_degree;
        }

        vertex_degree_t getVertexOutDegree(const NaiveVertex &t) const override {
            return t.out_degree;
        }


        vertex_id_t getVertexId(const NaiveVertex &t) const override {
            return t.id;
        }

        edge_id_t getEdgeId(const NaiveEdge &c) const override {
            return c.id;
        }

        const std::string &getVertexName(const NaiveVertex &t) const override {
            return t.name;
        }

        const std::string &getEdgeName(const NaiveEdge &c) const override {
            return c.name;
        }


        void setVertexId(const NaiveVertex &t, const vertex_id_t id) override {
            if (vertex_id_to_index.count(id)) {
                VERBOSE_ASSERT(t.vector_index == vertex_id_to_index.at(id), "Cannot have two vertex with the same id");
            }
            vertex_id_to_index[id] = t.vector_index;
            vertex_ordered_list[t.vector_index].id = id;
        }

        const NaiveVertex &getVertexById(const vertex_id_t id) const override {
            return vertex_ordered_list[vertex_id_to_index.at(id)];
        }

        void setEdgeId(const NaiveEdge &c, const edge_id_t id) override {
            if (edge_id_to_index.count(id)) {
                VERBOSE_ASSERT(c.vector_index == vertex_id_to_index.at(id), "Cannot have two edges with the same id");
            }
            edge_id_to_index[id] = c.vector_index;
            edge_ordered_list[c.vector_index].id = id;
        }

        const NaiveEdge &getEdgeById(const edge_id_t id) const override {
            return edge_ordered_list[edge_id_to_index.at(id)];
        }



        void setVertexName(const NaiveVertex &t, const std::string &name) override {
            if (vertex_name_to_index.count(name)) {
                VERBOSE_ASSERT(t.vector_index == vertex_name_to_index.at(name), "Cannot have two vertex with the same name");
            }
            vertex_name_to_index[name] = t.vector_index;
            vertex_ordered_list[t.vector_index].name = name;
        }

        void setEdgeName(const NaiveEdge &c, const std::string &name) override {
            if (edge_name_to_index.count(name)) {
                VERBOSE_ASSERT(c.vector_index == edge_name_to_index.at(name), "Cannot have two edges with the same name");
            }
            edge_name_to_index[name] = c.vector_index;
            edge_ordered_list[c.vector_index].name = name;

        }


        const NaiveVertex &getVertexByName(const std::string &name) const override {
            return vertex_ordered_list[vertex_name_to_index.at(name)];
        }


        const NaiveEdge &getEdgeByName(const std::string &name) const override {
            return edge_ordered_list[edge_name_to_index.at(name)];
        }


        bool edgeExist(const NaiveVertex &from, const NaiveVertex &to) const override {
            for (auto idx : vertex_output_ordered_list.at(from.vector_index)) {
                if (edge_ordered_list.at(idx).to->vector_index == to.vector_index) return true;
            }
            return false;
        }



    };

} // end of namespace models


#endif //KITER_NAIVEDATAFLOW_H
