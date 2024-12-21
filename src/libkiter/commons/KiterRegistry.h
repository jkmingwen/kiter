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
#include <algorithm>


typedef std::map<std::string,std::string> parameters_list_t;


// typedef std::function< BufferSizingResult(models::Dataflow* const, TIME_UNIT) > BufferSizingFun;



/**
 * This part defines KiterRegistry and the different type of actions it can support
 */

struct generator_action_t {
	std::string name;
	std::string desc;
	models::Dataflow* (*fun)(parameters_list_t);
};

struct transformation_action_t {
    std::string name;
    std::string desc;
    void (*fun)(models::Dataflow*, parameters_list_t);
};

struct buffer_sizing_action_t {
    std::string name;
    std::string desc;

    models::BufferSizingResult (*fun)(models::Dataflow*, parameters_list_t);
};
struct printer_action_t {
    std::string name;
    std::string desc;
    void (*fun)(models::Dataflow*, parameters_list_t);
};

struct scheduling_action_t {
    std::string name;
    std::string desc;
    models::Scheduling  (*fun)(models::Dataflow*, parameters_list_t);
};

struct analysis_action_t {
    std::string name;
    std::string desc;
    void (*fun)(models::Dataflow*, parameters_list_t);
};
struct throughput_action_t {
    std::string name;
    std::string desc;
    TIME_UNIT  (*fun)(models::Dataflow*, parameters_list_t);
};


struct throughput_buffering_dse_action_t {
    std::string name;
    std::string desc;

    StorageDistributionSet (*fun)(models::Dataflow*, parameters_list_t);
};


#define ADD_KITER_TOOL(name,type,t)  static auto name##unique = KiterRegistry<type>::add(t)

#define ADD_BUFFER_SIZING(name,t)    ADD_KITER_TOOL(name,buffer_sizing_action_t,t)
#define ADD_TRANSFORMATION(name,t)   ADD_KITER_TOOL(name,transformation_action_t,t)
#define ADD_THROUGHPUT_BUFFERING_DSE(name,t)   ADD_KITER_TOOL(name,throughput_buffering_dse_action_t,t)
#define ADD_ANALYSIS(name,t)   ADD_KITER_TOOL(name,analysis_action_t,t)
#define ADD_SCHEDULING(name,t)   ADD_KITER_TOOL(name,scheduling_action_t,t)
#define ADD_THROUGHPUT(name,t)   ADD_KITER_TOOL(name,throughput_action_t,t)
#define ADD_GENERATOR(name,t)        ADD_KITER_TOOL(name,generator_action_t,t)
#define ADD_PRINTER(name,t)          ADD_KITER_TOOL(name,printer_action_t,t)

template <typename T = transformation_action_t>
class KiterRegistry {

  public:
    typedef std::map<std::string, T> tools_map;

    static bool add(const T& t) {
        tools_map& map = getSingletonMap();
    	if (map.find(t.name) != map.end()) return false;
    	map[t.name] = t;
    	return true;
    }

    static const std::vector<std::string>  get_names() {
        tools_map& m = getSingletonMap();
        std::vector<std::string> keys;
        keys.reserve(m.size()); // Reserve memory to improve performance
        std::transform(m.begin(), m.end(), std::back_inserter(keys),
                       [](const auto& pair) {
                           return pair.first;
                       });
        return keys;

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
        KiterRegistry<generator_action_t>::print(out);
        out << "" << std::endl;

        out << " List of supported buffer sizing algorithms (-a) is " << std::endl;
        KiterRegistry<buffer_sizing_action_t>::print(out);
        out << "" << std::endl;

        out << " List of supported transformation algorithms (-a) is " << std::endl;
        KiterRegistry<transformation_action_t>::print(out);
        out << "" << std::endl;

        out << " List of supported scheduling algorithms (-a) is " << std::endl;
        KiterRegistry<scheduling_action_t>::print(out);
        out << "" << std::endl;

        out << " List of supported throughput algorithms (-a) is " << std::endl;
        KiterRegistry<throughput_action_t>::print(out);
        out << "" << std::endl;

        out << " List of supported DSE algorithms (-a) is " << std::endl;
        KiterRegistry<throughput_buffering_dse_action_t>::print(out);
        out << "" << std::endl;

        out << " List of supported analysis algorithms (-a) is " << std::endl;
        KiterRegistry<analysis_action_t>::print(out);
        out << "" << std::endl;

        out << " List of supported printers (-a) is " << std::endl;
        KiterRegistry<printer_action_t>::print(out);
        out << "" << std::endl;

    }

private:

    static tools_map& getSingletonMap() {
      static tools_map map;
      return map;
    }
};


#endif /* SRC_KITER_TASKS_KiterRegistry_H_ */
