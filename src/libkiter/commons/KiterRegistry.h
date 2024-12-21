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

/**
 * This part defines KiterRegistry and the different type of actions it can support
 */

enum class ActionType {
    GENERATOR,
    ANALYSIS,
    TRANSFORMATION,
    SCHEDULING,
    THROUGHPUT,
    BUFFER_SIZING,
    THROUGHPUT_BUFFERING_DSE,
    PRINTER,
    COUNT // keep this last
};
// Define the action type mappings
static const std::unordered_map<ActionType, std::pair<std::string, std::string>> ActionTypeMappings = {
    {ActionType::GENERATOR, {"generator", "-g"}},
    {ActionType::ANALYSIS, {"analysis", "-a"}},
    {ActionType::TRANSFORMATION, {"transformation", "-a"}},
    {ActionType::SCHEDULING, {"scheduling", "-a"}},
    {ActionType::THROUGHPUT, {"throughput", "-a"}},
    {ActionType::BUFFER_SIZING, {"buffer sizing", "-a"}},
    {ActionType::THROUGHPUT_BUFFERING_DSE, {"throughput buffering DSE", "-a"}},
    {ActionType::PRINTER, {"printer", "-a"}}
};

// Base struct for demonstration
template<typename  FUNC_T>
struct BaseAction {
    std::string name;
    std::string desc;
    FUNC_T fun;
};

struct generator_action_t : BaseAction<models::Dataflow* (*)(parameters_list_t)> {};
struct transformation_action_t : BaseAction<void (*)(models::Dataflow*, parameters_list_t)> {};
struct buffer_sizing_action_t : BaseAction<models::BufferSizingResult (*)(models::Dataflow*, parameters_list_t)> {};
struct printer_action_t : BaseAction<void (*)(models::Dataflow*, parameters_list_t)> {};
struct scheduling_action_t  : BaseAction<models::Scheduling  (*)(models::Dataflow*, parameters_list_t)> {};
struct analysis_action_t  : BaseAction<void (*)(models::Dataflow*, parameters_list_t)> {};
struct throughput_action_t  : BaseAction<TIME_UNIT  (*)(models::Dataflow*, parameters_list_t)> {};
struct throughput_buffering_dse_action_t  : BaseAction<StorageDistributionSet (*)(models::Dataflow*, parameters_list_t)> {};


#define ADD_KITER_TOOL(n,t,d,f)  static auto n##unique = KiterRegistry<t>::add(t({ #n , d, f}))

#define ADD_BUFFER_SIZING(n,d,f)    ADD_KITER_TOOL(n,buffer_sizing_action_t, d, f)
#define ADD_TRANSFORMATION(n,d,f)   ADD_KITER_TOOL(n,transformation_action_t, d, f)
#define ADD_THROUGHPUT_BUFFERING_DSE(n,d,f)  ADD_KITER_TOOL(n,throughput_buffering_dse_action_t, d, f)
#define ADD_ANALYSIS(n,d,f)  ADD_KITER_TOOL(n,analysis_action_t, d, f)
#define ADD_SCHEDULING(n,d,f)  ADD_KITER_TOOL(n,scheduling_action_t, d, f)
#define ADD_THROUGHPUT(n,d,f)  ADD_KITER_TOOL(n,throughput_action_t, d, f)
#define ADD_GENERATOR(n,d,f)   ADD_KITER_TOOL(n,generator_action_t, d, f)
#define ADD_PRINTER(n,d,f)     ADD_KITER_TOOL(n,printer_action_t, d, f)

template <typename T = transformation_action_t>
class KiterRegistry {

  public:
    using tools_map = std::map<std::string, T>;

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
        for (const auto &i : KiterRegistry::getSingletonMap()) {
            out << " - " << i.second.name << " : " << i.second.desc << std::endl;
        }
    }

    // Print all registries dynamically
    static void print_all(std::ostream& out) {
        for (int i = 0; i < static_cast<int>(ActionType::COUNT); ++i) {
            ActionType actionType = static_cast<ActionType>(i);
            const auto& [actionName, actionArgument] = ActionTypeMappings.at(actionType);

            out << " List of supported " << actionName << " (" << actionArgument << ") is:" << std::endl;

            // Dynamically dispatch the correct registry's print method
            switch (actionType) {
                case ActionType::GENERATOR:
                    KiterRegistry<generator_action_t>::print(out);
                break;
                case ActionType::ANALYSIS:
                    KiterRegistry<analysis_action_t>::print(out);
                break;
                case ActionType::TRANSFORMATION:
                    KiterRegistry<transformation_action_t>::print(out);
                break;
                case ActionType::SCHEDULING:
                    KiterRegistry<scheduling_action_t>::print(out);
                break;
                case ActionType::THROUGHPUT:
                    KiterRegistry<throughput_action_t>::print(out);
                break;
                case ActionType::BUFFER_SIZING:
                    KiterRegistry<buffer_sizing_action_t>::print(out);
                break;
                case ActionType::THROUGHPUT_BUFFERING_DSE:
                    KiterRegistry<throughput_buffering_dse_action_t>::print(out);
                break;
                case ActionType::PRINTER:
                    KiterRegistry<printer_action_t>::print(out);
                break;
                default:
                    break;
            }
            out << std::endl;
        }
    }

private:

    static tools_map& getSingletonMap() {
      static tools_map map;
      return map;
    }
};


#endif /* SRC_KITER_TASKS_KiterRegistry_H_ */
