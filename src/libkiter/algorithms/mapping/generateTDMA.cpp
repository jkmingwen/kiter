/*
 * generateTDMA.cpp
 *
 *  Created on: Jul 2021
 *      Author: katwinkl3
 */

#include <models/Dataflow.h>
#include <algorithms/mappings.h>
#include <libxml/tree.h>

#include <utility>
#define GOOD_CAST (const xmlChar *)
void algorithms::mapping::generateTDMA (models::Dataflow* const  dataflow, parameters_list_t params_list) {

    std::string output_file = commons::get_parameter(std::move(params_list), "output", "");
    xmlDocPtr doc = xmlNewDoc(GOOD_CAST"1.0");
	xmlNodePtr root = xmlNewNode(nullptr,GOOD_CAST"tdma");
    xmlNewProp(root,GOOD_CAST"slots",GOOD_CAST"1");
    xmlDocSetRootElement(doc,root);

    // TODO : This bit of code is strange, need to double check
    {ForEachEdge(dataflow,e){
        const edge_id_t * e_id = &dataflow->getRoute(e)[0];
        const auto* rid = (const unsigned char*)e_id;
        xmlNodePtr rule = xmlNewNode(nullptr,GOOD_CAST"rule");
        xmlNewProp(rule,GOOD_CAST"id",rid);
        xmlNewProp(rule,GOOD_CAST"slot",GOOD_CAST"0");
    }}

    if (!output_file.empty()) xmlSaveFile(output_file.c_str(),doc);
	xmlFreeDoc(doc);

}

