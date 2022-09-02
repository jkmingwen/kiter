/*
 * stdout.h
 *
 *  Created on: 16 avr. 2013
 *      Author: toky
 */

#ifndef STDOUT_H_
#define STDOUT_H_


#include <commons/KiterRegistry.h>
#include <commons/commons.h>
#include <string>
#include <models/Dataflow.h>
#include <models/Scheduling.h>


namespace printers {



std::string Scheduling2Tikz    (const models::Scheduling& periodic_scheduling);

std::string generate_kiter (const models::Dataflow* const  dataflow  , bool verbose = false);
std::string GenerateGraphDOT       (models::Dataflow* const  dataflow  , bool simple = true);
std::string GenerateNoCDOT       (models::Dataflow* const  dataflow  , bool connect_tasks = true , bool color_routes = true);
std::string PeriodicScheduling2DOT    (models::Dataflow* const  dataflow, models::Scheduling& periodic_scheduling,   TIME_UNIT last_execution_end_at,  bool full , double xscale , double yscale );

void printGraph              (models::Dataflow* const  dataflow, parameters_list_t = std::map<std::string,std::string>());
void printMapping            (models::Dataflow* const  dataflow, parameters_list_t = std::map<std::string,std::string>());
void printGraphAsKiterScript (models::Dataflow* const  dataflow, parameters_list_t = std::map<std::string,std::string>());
void printInfos              (models::Dataflow* const  dataflow, parameters_list_t = std::map<std::string,std::string>());
void printXML                (models::Dataflow* const  dataflow, parameters_list_t = std::map<std::string,std::string>());
}

ADD_PRINTER(PrintXML,
		printer_t({ "PrintXML" , "Print XML file", printers::printXML})
);

ADD_PRINTER(PrintKiter,
            printer_t({ "PrintKiter" , "Generate C++ code to internally generate the graph inside Kiter.", printers::printGraphAsKiterScript} )
);
ADD_PRINTER(PrintInfos,
            printer_t({ "PrintInfos" , "Just print some graph informations.", printers::printInfos})
);
ADD_PRINTER(PrintGraph,
            printer_t({ "PrintGraph" , "Print DOT file", printers::printGraph})
);
ADD_PRINTER(PrintMapping,
            printer_t({ "PrintMapping" , "Print DOT file", printers::printMapping})
);
#endif /* STDOUT_H_ */
