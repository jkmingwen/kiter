//
// Created by toky on 27/4/23.
//

#include "TDMAWriter.h"
#include <libxml/tree.h>
#include <models/Dataflow.h>

#define GOOD_CAST (const xmlChar *)
void printers::write_TDMA_file (models::Dataflow* const  dataflow, std::string output_file) {

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
