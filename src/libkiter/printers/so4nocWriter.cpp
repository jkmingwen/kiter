//
// Created by toky on 27/4/23.
//

#include "so4nocWriter.h"
#include "libxml/parser.h"

#include <models/Dataflow.h>
#include <tuple>
#include <map>
#include<filesystem>
#include <libxml/tree.h>

std::tuple<std::map<ARRAY_INDEX, long>, long> printers::generateConditions(const models::Dataflow* const dataflow, const std::string& filename, std::set<ARRAY_INDEX> ){
    std::map<ARRAY_INDEX, long> condition; // dataflow edge : [slot]
    long slots = 0;
    std::filesystem::path file(filename);
    if (file.extension() == ".xml"){
        std::map<edge_id_t, long> condition_param; //noc edge id : slot
        xmlDocPtr doc =  xmlParseFile(filename.c_str()); //TODO: error handling and shift into separate function
        xmlNodePtr tdma_node;
        for (xmlNodePtr cur_node = doc->children; cur_node; cur_node = cur_node->next) {
            if (cur_node->type == XML_ELEMENT_NODE) {
                if (std::string((const char*)cur_node->name) == std::string("tdma")) tdma_node = cur_node;
            }
        }
        const char* temp = (const char*) tdma_node->properties->children->content;
        slots = atoi(temp); //TODO: change to strtol
        for (xmlNodePtr cur_node = tdma_node->children; cur_node; cur_node = cur_node->next) {
            if (cur_node->type == XML_ELEMENT_NODE) {
                if (std::string((const char*)cur_node->name) == std::string("rule")) {
                    long eid;
                    long s;
                    char *ss1, *ss2;
                    for (xmlAttrPtr cur_attr = cur_node->properties; cur_attr; cur_attr = cur_attr->next){
                        if (strcmp((const char*)cur_attr->name,"slot") == 0){
                            temp = (const char*) cur_attr->children->content;
                            s = strtol(temp, &ss1, 10);
                        }
                        if (strcmp((const char*)cur_attr->name,"id") == 0){
                            temp = (const char*) cur_attr->children->content;
                            eid = strtol(temp, &ss2, 10);
                        }
                    }
                    condition_param.insert({eid,s});
                }
            }
        }
        xmlFreeDoc(doc);
        {ForEachEdge(dataflow,e){
                if (dataflow->getEdgeType(e) == VIRTUAL_EDGE){
                    condition.insert({dataflow->getEdgeId(e), -1});
                } else {
                    condition.insert({dataflow->getEdgeId(e), condition_param[dataflow->getRoute(e)[0]]});
                }
                // if (new_edges.find(dataflow->getEdgeId(e)) != new_edges.end()){
                //   condition.insert({dataflow->getEdgeId(e), -1});
                // } else {
                //   condition.insert({dataflow->getEdgeId(e), condition_param[dataflow->getRoute(e)[0]]});
                // }
            }}
    }
    if (file.extension() == ".txt"){
        std::map<std::pair<long, long>, long> condition_param; //noc edge id : slot
        std::string line;
        std::ifstream readfile(filename);
        while (getline (readfile, line)) {
            std::istringstream ss(line);
            std::string subline;
            std::vector<long> vals = {};
            while(std::getline(ss, subline, ',')) {
                if ((subline != "src") && (subline != "dest") && (subline != "slot")){
                    vals.push_back(std::stol(subline));
                }
            }
            if (vals[2] > slots){
                slots = vals[2];
            }
            condition_param.insert({{vals[0]+dataflow->getNoC().size(), vals[1]+dataflow->getNoC().size()}, vals[2]});
        }
        readfile.close();
        {ForEachEdge(dataflow,e){
                if (dataflow->getEdgeType(e) == VIRTUAL_EDGE){
                    condition.insert({dataflow->getEdgeId(e), -1}); //actually has no use
                } else {
                    if (condition_param.find({ dataflow->getMapping(dataflow->getEdgeSource(e)), dataflow->getMapping(dataflow->getEdgeTarget(e)) }) != condition_param.end()){
                        condition.insert({dataflow->getEdgeId(e), condition_param[{dataflow->getMapping(dataflow->getEdgeSource(e)), dataflow->getMapping(dataflow->getEdgeTarget(e))}] });
                    } else {
                        std::cout << "Missing TDMA rule for edge " << dataflow->getEdgeId(e) << "src=" << dataflow->getMapping(dataflow->getEdgeSource(e)) << " dest=" << dataflow->getMapping(dataflow->getEdgeTarget(e))<< std::endl;
                        condition.insert({dataflow->getEdgeId(e), 0});
                    }
                }
            }}
    }
    return {condition, slots+1};
}
