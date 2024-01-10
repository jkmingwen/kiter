/*
 * TransformationsRegistry.h
 *
 *  Created on: Apr 23, 2020
 *      Author: toky
 */

#ifndef SRC_KITER_TASKS_KiterRegistry_H_
#define SRC_KITER_TASKS_KiterRegistry_H_

#include <commons/basic_types.h>
#include <string>
#include <map>
#include <iostream>
#include <functional>

typedef std::map<std::string,std::string> parameters_list_t;

namespace models {
class Dataflow;
}

/**
 * This part is about buffer sizing
 */

class BufferSizingResult {

    bool       _valid;
    TOKEN_UNIT _total_size;
    std::map<ARRAY_INDEX, TOKEN_UNIT> _idv_size;
public:
    BufferSizingResult(TOKEN_UNIT t) : _valid (true), _total_size(t) {}
    BufferSizingResult() : _valid (false), _total_size(0) {}
    bool is_valid() const {return _valid;}
    TOKEN_UNIT total_size() const {return _total_size;}
    void set_validity(bool validity){_valid = validity;}
    void set_total_size(TOKEN_UNIT token){_total_size = token;}
    void add_edge_size(ARRAY_INDEX edge, TOKEN_UNIT size) {_idv_size.insert({edge, size});}
    TOKEN_UNIT get_edge_size (ARRAY_INDEX edge) {return _idv_size[edge];}
};

typedef std::function< BufferSizingResult(models::Dataflow* const, TIME_UNIT) > BufferSizingFun;



/**
 * This part defines KiterRegistry and the different type of actions it can support
 */

struct generator_t {
	std::string name;
	std::string desc;
	models::Dataflow* (*fun)(parameters_list_t);
};

struct transformation_t {
	std::string name;
	std::string desc;
	void (*fun)(models::Dataflow*, parameters_list_t);
};

struct buffer_sizing_t {
    std::string name;
    std::string desc;
    BufferSizingResult (*fun)(models::Dataflow*, parameters_list_t);
};

struct printer_t {
    std::string name;
    std::string desc;
    void (*fun)(models::Dataflow*, parameters_list_t);
};


#define ADD_KITER_TOOL(name,type,t)  static auto name##unique = KiterRegistry<type>::add(t)
#define ADD_BUFFER_SIZING(name,t)    ADD_KITER_TOOL(name,buffer_sizing_t,t)
#define ADD_TRANSFORMATION(name,t)   ADD_KITER_TOOL(name,transformation_t,t)
#define ADD_GENERATOR(name,t)        ADD_KITER_TOOL(name,generator_t,t)
#define ADD_PRINTER(name,t)          ADD_KITER_TOOL(name,printer_t,t)

template <typename T = transformation_t>
class KiterRegistry {

  public:
    typedef std::map<std::string, T> tools_map;

    static bool add(const T& t) {
        tools_map& map = getSingletonMap();
    	if (map.find(t.name) != map.end()) return false;
    	map[t.name] = t;
    	return true;
    }

    static const T* get(const std::string& name) {
        tools_map& map = getSingletonMap();
    	if (map.find(name) == map.end()) return nullptr;
    	return &(map[name]);
    }
    static void print (std::ostream &out) {
        for (auto i : KiterRegistry<T>::getSingletonMap()) {
            out << " - " << i.second.name << " : " << i.second.desc << std::endl;
        }
    }
    static void print_all (std::ostream &out) {
        out << " List of supported generator (-g) is " << std::endl;
        KiterRegistry<generator_t>::print(out);
        out << "" << std::endl;

        out << " List of supported buffer sizing algorithms (-a) is " << std::endl;
        KiterRegistry<buffer_sizing_t>::print(out);
        out << "" << std::endl;

        out << " List of supported algorithms (-a) is " << std::endl;
        KiterRegistry<transformation_t>::print(out);
        out << "" << std::endl;

        out << " List of supported printers (-a) is " << std::endl;
        KiterRegistry<printer_t>::print(out);
        out << "" << std::endl;

    }

private:

    static tools_map& getSingletonMap() {
      static tools_map map;
      return map;
    }
};


#endif /* SRC_KITER_TASKS_KiterRegistry_H_ */
