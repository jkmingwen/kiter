//
// Created by toky on 24/4/23.
//

#include "TokenConfiguration.h"

namespace algorithms {
    namespace dse {

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
        std::string TokenConfiguration::to_csv_line() const{
            std::string output("");
            output += this->performance_computed? commons::toString(this->getCost()) : "-";
            output += ",";
            output += this->performance_computed? commons::toString(this->getPerformance()) : "-";
            output += ",";
            output += "\"";
            std::string delim("");
            for (auto item : this->configuration){
                output += delim;
                output += std::to_string(item.second);
                delim = ",";
            }
            output += "\"";
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



    } // dse
} // algorithms
