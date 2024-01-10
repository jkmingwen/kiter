//
// Created by toky on 24/4/23.
//

#include "TokenConfiguration.h"

namespace algorithms {
    namespace dse {

        TokenConfiguration::TokenConfiguration(const models::Dataflow *dataflow,
                                               const std::map<ARRAY_INDEX, TOKEN_UNIT> &configuration,
                                               const PerformanceResult &result,
                                               TIME_UNIT execTime,
                                               TIME_UNIT cumulTime) :
                TokenConfiguration(dataflow, configuration)
        {
            performance = result;
            executionTime = execTime;
            cumulativeTime = cumulTime;
            this->performance_computed = true;
        }



        bool TokenConfiguration::dominates(const TokenConfiguration& other) const {
            for ( auto edge_token_item : configuration) {
                if (edge_token_item.second < other.configuration.at(edge_token_item.first)) {
                    return false;
                }
            }

            return true;
        }


        void TokenConfiguration::refresh_cost(const models::Dataflow* dataflow) {
            cost = 0;
            for (auto edge_token_item : configuration) {
                cost += edge_token_item.second;
                Edge e = dataflow->getEdgeById(edge_token_item.first);
                VERBOSE_ASSERT_EQUALS(dataflow->getEdgeType(e), EDGE_TYPE::FEEDBACK_EDGE);
            }
        }

        std::map<ARRAY_INDEX, TOKEN_UNIT> TokenConfiguration::vectorToMap(const models::Dataflow* dataflow, const std::vector<TOKEN_UNIT>& config_vec) {
            std::map<ARRAY_INDEX, TOKEN_UNIT> temp_config;
            size_t i = 0;
            ForEachEdge(dataflow, c) {
                if (dataflow->getEdgeType(c) == EDGE_TYPE::FEEDBACK_EDGE) {
                    VERBOSE_ASSERT(i < config_vec.size(), "Configuration too small to be stored");
                    temp_config[dataflow->getEdgeId(c)] = config_vec[i];
                    i++;
                }
            }
            VERBOSE_ASSERT(i == config_vec.size(), "Configuration too big to be stored");
            return temp_config;
        }



        /*
         *
         *  CSV PART
         *
         */


        const std::string TokenConfiguration::csv_header() {
            return "storage distribution size,"
                   "throughput,"
                   "channel quantities,"
                   "feedback quantities,"
                   "critical channels,"
                   "critical feedback,"
                   "duration,"
                   "cumulative duration";
        }

        size_t TokenConfiguration::csv_header_size() {
            static auto size =  commons::split<std::string>(csv_header(), ',').size() ;
            return size;
        }

        TokenConfiguration TokenConfiguration::from_csv_line(const models::Dataflow* dataflow, const std::string &line)  {
            std::vector<std::string> cells = commons::parseCsvLine(line);

            if (cells.size() != csv_header_size() ) {
                throw std::runtime_error("Wrong number of cell in this line.");
            }


            std::map<ARRAY_INDEX,TOKEN_UNIT> config;
            std::set<ARRAY_INDEX> criticals;

            // cell[0] - Cost
            // cell[1] - Throughput
            bool has_performance =  (cells[1] != "-");
            TIME_UNIT throughput = cells[1] != "-" ? std::stod(cells[1]) : 0;
            // cell[2] - channel quantities
            std::vector<TOKEN_UNIT> edgeVec = commons::split<TOKEN_UNIT>(cells[2], ',');
            // cell[3] - feedback quantities
            std::vector<TOKEN_UNIT> feedbackVec = commons::split<TOKEN_UNIT>(cells[3], ',');
            // cell[4] - channel critical
            std::vector<TOKEN_UNIT> criticalEdgeVec = cells[4] == "-" ?std::vector<TOKEN_UNIT>() : commons::split<TOKEN_UNIT>(cells[4], ',');
            // cell[5] - feedback critical
            std::vector<TOKEN_UNIT> criticalFeedbackVec = cells[5] == "-" ?std::vector<TOKEN_UNIT>() : commons::split<TOKEN_UNIT>(cells[5], ',');
            // cell[6] - duration
            TIME_UNIT execTime = cells[6] != "-" ? std::stod(cells[6]) : 0;
            // cell[7] - cumulative duration
            TIME_UNIT cumulTime =  cells[7] != "-" ? std::stod(cells[7]) : 0;


            {
                size_t index = 0;
                ForEachEdge(dataflow, e) {
                    if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                        ARRAY_INDEX edge_id = dataflow->getEdgeId(e);
                        config[edge_id] = feedbackVec[index];
                        if (std::find(criticalEdgeVec.begin(), criticalEdgeVec.end(), index) != criticalEdgeVec.end()) {
                            criticals.insert(edge_id);
                        }
                        index++;
                    }
                }
            }

            if (has_performance) {
                TokenConfiguration::PerformanceResult performance(throughput, criticals);
                TokenConfiguration res(dataflow, config, performance, execTime, cumulTime);
                return res;
            } else {
                TokenConfiguration res(dataflow, config);
                return res;
            }
        }
        std::string TokenConfiguration::to_csv_line(bool no_timing) const{

            std::set<ARRAY_INDEX> critical_edges = this->performance_computed? this->getPerformance().critical_edges : std::set<ARRAY_INDEX >();

            std::string  normal_configuration_string;
            std::string  feedback_configuration_string;
            std::string  normal_critical_string;
            std::string  feedback_critical_string;

            int normal_index = 0;
            int feedback_index = 0;

            { ForEachEdge(this->getDataflow(),e){

                auto edge_id = this->getDataflow()->getEdgeId(e);

                // keep track of index and store their preload/configuration
                if (this->getDataflow()->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                    if (!feedback_configuration_string.empty()) feedback_configuration_string += ",";
                    feedback_configuration_string  += std::to_string(this->configuration.at(edge_id));
                    feedback_index++;
                } else {
                    if  (!normal_configuration_string.empty()) normal_configuration_string += ",";
                    normal_configuration_string  += std::to_string(this->getDataflow()->getPreload(e));
                    normal_index++;
                }

                // When one edge is critical, it's csv index is stored in the corresponding list.
                if (critical_edges.find(edge_id) != critical_edges.end()) {
                    if (this->getDataflow()->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                        if (!feedback_critical_string.empty()) feedback_critical_string += ",";
                        feedback_critical_string += std::to_string(feedback_index);
                    } else {
                        if (!normal_critical_string.empty()) normal_critical_string += ",";
                        normal_critical_string += std::to_string(normal_index);
                    }
                }

            }}

            std::string output("");
            // cell[0] - Cost
            output += commons::toString(this->getCost());
            output += ",";
            // cell[1] - Throughput
            output += this->performance_computed? commons::toString(this->getPerformance().throughput) : "-";
            output += ",";
            // cell[2] - channel quantities
            output += "\"" + normal_configuration_string + "\"" ;
            output += ",";
            // cell[3] - feedback quantities
            output += "\"" + feedback_configuration_string + "\"" ;
            output += ",";
            // cell[4] - channel critical
            output += this->performance_computed? "\"" + normal_critical_string + "\"" : "-";
                       output += ",";
            // cell[5] - feedback critical
            output += this->performance_computed? "\"" + feedback_critical_string + "\"" : "-";
            output += ",";
            // cell[6] - duration
            output += this->performance_computed and !no_timing? commons::toString(this->executionTime): "-";
            output += ",";
            // cell[7] - cumulative duration
            output += this->performance_computed and !no_timing? commons::toString(this->cumulativeTime): "-";


            return output;
        }





        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration& f) {
            return out << f.to_csv_line();
        }

        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration::PerformanceResult& pr) {
            out << pr.throughput << ",\"";
            for (auto it = pr.critical_edges.begin(); it != pr.critical_edges.end(); ++it) {
                if (it != pr.critical_edges.begin()) {
                    out << ", ";
                }
                out << *it;
            }
            out << "\"";
            return out;
        }

        std::istream& operator>>(std::istream& is, algorithms::dse::TokenConfiguration& dt) {
            const models::Dataflow *df = dt.getDataflow();
            std::string input_str;
            is >> input_str;
            std::vector<TOKEN_UNIT> vals = commons::split<TOKEN_UNIT>(input_str, ',');
            std::map<ARRAY_INDEX, TOKEN_UNIT> new_map;
            {   size_t i = 0;
                ForEachEdge(df,c){
                    if (df->getEdgeType(c) == EDGE_TYPE::FEEDBACK_EDGE) {
                        VERBOSE_ASSERT(i < vals.size(), "Configuration too small to be stored");
                        new_map[df->getEdgeId(c)] = vals[i];
                        i++;
                    }
                }
                VERBOSE_ASSERT(i == vals.size(), "Configuration too big to be stored");
            }
            dt = algorithms::dse::TokenConfiguration(dt.getDataflow(), new_map);

            return is;
        }




    } // dse
} // algorithms
