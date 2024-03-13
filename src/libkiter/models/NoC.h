/*
 * NoC.h
 *
 *  Created on: Jul 24, 2019
 *      Author: toky
 */

#ifndef SRC_LIBKITER_MODELS_NOC_H_
#define SRC_LIBKITER_MODELS_NOC_H_


#include <commons/verbose.h>
#include <map>
#include <vector>
#include <iostream>

typedef int                    noc_size_t;
typedef unsigned long          bank_id_t;
typedef          long          noc_id_t;
typedef noc_id_t               node_id_t;
typedef noc_id_t               edge_id_t;

typedef std::vector<edge_id_t> route_t;

class NoCException: public std::exception {
    virtual const char* what() const throw() {
        return "Error happened in the NoC component";
    }
};

enum NetworkNodeType { Core, Router, Unknown };

struct NetworkComponent {
	noc_id_t id;
	NetworkComponent (noc_id_t id) : id(id) {};

};

struct NetworkNode : public NetworkComponent {
	//node_id_t id;
	NetworkNodeType type;
	double       x;
	double       y;
	NetworkNode (node_id_t id, NetworkNodeType type, double x , double y)  :  NetworkComponent(id),	 type(type), x(x),	 y (y) {};
};

inline std::ostream& operator<< (std::ostream &out,const NetworkNode& n) {
	if (n.type == NetworkNodeType::Router) {
		out << "Router(" << n.id << ", x=" << n.x << ", y=" << n.y << ")";
	} else if (n.type == NetworkNodeType::Core) {
		out << "Core(" << n.id << ", x=" << n.x  << ", y=" << n.y << ")";
	} else {
		out << "UnknownNetworkNode(" << n.id << ", x=" << n.x  << ", y=" << n.y << ")";
	}
	return out;
}

struct NetworkEdge : public NetworkComponent{
	//edge_id_t id;
	node_id_t src;
	node_id_t dst;
	NetworkEdge(edge_id_t id,
			noc_id_t src,
			noc_id_t dst) :  NetworkComponent(id),	 src(src),	 dst (dst) {};
};
inline std::ostream& operator<< (std::ostream &out,const NetworkEdge& e) {
	out << "NetworkEdge(" << e.id << ", from=" << e.src  << ", to=" << e.dst << ")";
	return out;
}


class NoC {

private :
	noc_size_t _XSIZE, _YSIZE;
private:
	std::map<std::pair<node_id_t,node_id_t>,NetworkEdge> _medges;
	std::map<edge_id_t,NetworkEdge> _mid2edges;
	std::map<node_id_t,NetworkNode> _mnodes;
	std::map<node_id_t,std::vector<node_id_t>> _adj;
	std::vector<NetworkEdge> _vedges;
	std::vector<NetworkNode> _vnodes;

	NoC () : _XSIZE(0), _YSIZE(0) {
	} ;
public:
	const std::vector<NetworkEdge>& getEdges() const {return this->_vedges;};
	const std::vector<NetworkNode>& getNodes() const {return this->_vnodes;};
	bool           hasNode(node_id_t id) const {return (this->_mnodes.count(id) > 0);};
	bool           hasEdge(edge_id_t id)  const {return (this->_mid2edges.count(id) > 0);};

	const NetworkNode&   getNode(node_id_t id) const {
		VERBOSE_ASSERT(this->_mnodes.count(id), "Impossible to find node " << id << " inside the NoC.");
		return this->_mnodes.at(id);
	};

	const NetworkEdge& getEdge(edge_id_t e) const {return this->_mid2edges.at(e);};
	const NetworkEdge& getEdge(node_id_t src,node_id_t dst) const {
		const std::pair<node_id_t,node_id_t> pair = {src,dst};
		VERBOSE_ASSERT(this->_medges.count(pair), "Impossible to find edge from " << src  << " to " << dst  << " inside the NoC.");
		return this->_medges.at(pair);
	};

	
	const NetworkEdge& getLeftNeighbour(node_id_t src) const {
		for (const auto &it : this->_vedges){
			if ((it.src == src) && (this->getNode(it.src).y == this->getNode(it.dst).y) 
				&& (this->getNode(it.src).x > this->getNode(it.dst).x)){
					return it;
			}
		}
        VERBOSE_ERROR("getLeftNeighbour: no edge from this node");
        throw NoCException();
	};
	const NetworkEdge& getRightNeighbour(node_id_t src) const {
		for (const auto &it : this->_vedges){
			if ((it.src == src) && (this->getNode(it.src).y == this->getNode(it.dst).y) 
				&& (this->getNode(it.src).x < this->getNode(it.dst).x)){
					return it;
			}
		}
        VERBOSE_ERROR("getRightNeighbour: no edge from this node");
        throw NoCException();
	};
	const NetworkEdge& getTopNeighbour(node_id_t src) const {
		for (const auto &it : this->_vedges){
			if ((it.src == src) && (this->getNode(it.src).x == this->getNode(it.dst).x) 
				&& (this->getNode(it.src).y < this->getNode(it.dst).y)){
					return it;
			}
		}
        VERBOSE_ERROR("getTopNeighbour: no edge from this node");
        throw NoCException();
	};
	const NetworkEdge& getBottomNeighbour(node_id_t src) const {
		for (const auto &it : this->_vedges){
			if ((it.src == src) && (this->getNode(it.src).x == this->getNode(it.dst).x) 
				&& (this->getNode(it.src).y > this->getNode(it.dst).y)){
					return it;
			}
		}

        VERBOSE_ERROR("getBottomNeighbour: no edge from this node");
        throw NoCException();
	};
	const NetworkEdge& getSrcNeighbour(node_id_t src) const {
		//Match associated router
		for (const auto &it : this->_vedges){
			if (it.src == src){
					return it;
			}
		}
        VERBOSE_ERROR("getSrcNeighbour: no edge to this node.");
        throw NoCException();
	};
	const NetworkEdge& getDstNeighbour(node_id_t dst) const {
		//Match associated router
		for (const auto &it : this->_vedges){
			if (it.dst == dst){
					return it;
			}
		}
        VERBOSE_ERROR("getSrcNeighbour: no edge from this node.");
        throw NoCException();
	};


	//the routers are from (0 to (NXN)-1)
	//while the cores are marked from (NXN) to 2*(NXN) - 1

	// TODO ; Hardware limit
	NoC (noc_size_t XSIZE, noc_size_t YSIZE) : _XSIZE(XSIZE), _YSIZE(YSIZE)  {
		this->generate();
		VERBOSE_ASSERT(check_ids() , "Generate failed.");
	};
private:
	void generate();
	bool check_ids();

	void addNetworkNode (const NetworkNode n) {
			_vnodes.push_back(n);
			_mnodes.insert({n.id,n});
	}
	void addNetworkEdge (const NetworkEdge e) {
		_mid2edges.insert({e.id,e});
		_medges.insert({std::pair<node_id_t,node_id_t>(e.src, e.dst),e});
		_vedges.push_back(e);
		_adj[e.src].push_back(e.dst);
	}


public :
	noc_size_t size      () const {return _XSIZE * _YSIZE;}
	noc_size_t getXSize  () const {return _XSIZE;}
	noc_size_t getYSize  () const {return _YSIZE;}




};


#endif /* SRC_LIBKITER_MODELS_NOC_H_ */
