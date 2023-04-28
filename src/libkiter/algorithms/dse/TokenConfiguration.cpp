//
// Created by toky on 24/4/23.
//

#include "TokenConfiguration.h"

namespace algorithms {
    namespace dse {
        std::vector<std::string> parseCsvLine(const std::string& line) {
            VERBOSE_DEBUG("Read a CSV line: " << line);
            std::vector<std::string> cells;
            std::string cell;
            std::istringstream lineStream(line);
            bool inQuotes = false;

            for(char c : line) {
                switch(c) {
                    case ',':
                        if(inQuotes) {
                            cell += c;
                        } else {
                            cells.push_back(cell);
                            cell = "";
                        }
                        break;

                    case '\"':
                        inQuotes = !inQuotes;
                        break;

                    default:
                        cell += c;
                        break;
                }
            }
            // Add the last cell
            cells.push_back(cell);
            VERBOSE_DEBUG("Obtain a cell list: " << commons::toString<>(cells));
            return cells;
        }

        bool TokenConfiguration::dominates(const TokenConfiguration& other) const {
            for ( auto edge_token_item : configuration) {
                if (edge_token_item.second < other.configuration.at(edge_token_item.first)) {
                    return false;
                }
            }

            return true;
        }

        const std::map<ARRAY_INDEX, TOKEN_UNIT>&  TokenConfiguration::getConfiguration() const {
            return this->configuration;
        }

        std::string TokenConfiguration::csv_header() {
            return "storage distribution size,throughput,channel quantities,critical channels,execution time,cumulative time";
        }
        TokenConfiguration TokenConfiguration::from_csv_line(const models::Dataflow* dataflow, const std::string &line)  {
            std::vector<std::string> cells = parseCsvLine(line);

            if (cells.size() != 6) {
                throw std::runtime_error("Wrong number of cell in this line.");
            }

            std::vector<TOKEN_UNIT> configVec = commons::split<TOKEN_UNIT>(cells[2], ',');
            std::vector<TOKEN_UNIT> criticalVec = cells[3] == "-" ?std::vector<TOKEN_UNIT>() : commons::split<TOKEN_UNIT>(cells[3], ',');
            std::map<ARRAY_INDEX,TOKEN_UNIT> config;
            std::set<ARRAY_INDEX> criticals;

                {
                    size_t index = 0;
                    ForEachEdge(dataflow, e) {
                        if (dataflow->getEdgeType(e) == EDGE_TYPE::FEEDBACK_EDGE) {
                            ARRAY_INDEX edge_id = dataflow->getEdgeId(e);
                            config[edge_id] = configVec[index];
                            if (std::find(criticalVec.begin(), criticalVec.end(), index) != criticalVec.end()) {
                                criticals.insert(edge_id);
                            }
                            index++;
                        }
                    }
                }

            if (cells[1] != "-") {
                TIME_UNIT throughput = std::stod(cells[1]);
                TIME_UNIT execTime = cells[4] != "-" ? std::stod(cells[4]) : 0;
                TIME_UNIT cumulTime =  cells[5] != "-" ? std::stod(cells[5]) : 0;
                TokenConfiguration::PerformanceResult performance(throughput, criticals);
                TokenConfiguration res(dataflow, config, performance, execTime, cumulTime);
                return res;
            } else {
                TokenConfiguration res(dataflow, config);
                return res;
            }
        }
        std::string TokenConfiguration::to_csv_line(bool no_timing) const{

            std::string configuration_string  = "";
            std::string configuration_delim("");
            std::string critical_edges_string = "";
            std::string critical_edges_delim("");

            std::set<ARRAY_INDEX> critical_edges = this->performance_computed? this->getPerformance().critical_edges : std::set<ARRAY_INDEX >();

            configuration_string += "\"";
            critical_edges_string += "\"";

            int index = 0;
            for (auto item: this->configuration) {
                if (critical_edges.find(item.first) != critical_edges.end()) {
                    critical_edges_string += critical_edges_delim;
                    critical_edges_string += std::to_string(index);
                    critical_edges_delim = ",";
                }
                configuration_string += configuration_delim;
                configuration_string += std::to_string(item.second);
                configuration_delim = ",";
                index++;
            }
            configuration_string += "\"";
            critical_edges_string += "\"";

            std::string output("");
            output += commons::toString(this->getCost());
            output += ",";
            output += this->performance_computed? commons::toString(this->getPerformance().throughput) : "-";
            output += ",";
            output += configuration_string;
            output += ",";
            output += this->performance_computed? critical_edges_string: "-";
            output += ",";
            output += this->performance_computed and !no_timing? commons::toString(this->executionTime): "-";
            output += ",";
            output += this->performance_computed and !no_timing? commons::toString(this->cumulativeTime): "-";
            return output;
        }

        const models::Dataflow *TokenConfiguration::getDataflow() const {
            return dataflow;
        }

        bool TokenConfiguration::hasPerformance()  const {
            return this->performance_computed;
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



    } // dse
} // algorithms
