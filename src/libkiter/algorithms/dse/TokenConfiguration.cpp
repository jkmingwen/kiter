//
// Created by toky on 24/4/23.
//

#include "TokenConfiguration.h"

namespace algorithms {
    namespace dse {
        TokenConfiguration::TokenConfiguration(const models::Dataflow* dataflow, std::map<ARRAY_INDEX, TOKEN_UNIT> configuration)
                :dataflow{dataflow}, configuration{configuration} {
            cost = 0;
            for (auto edge_token_item : configuration) {
                cost += edge_token_item.second;
                Edge e = dataflow->getEdgeById(edge_token_item.first);
                VERBOSE_ASSERT_EQUALS(dataflow->getEdgeType(e), EDGE_TYPE::FEEDBACK_EDGE);
            }
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
        std::string TokenConfiguration::to_csv_line() const{
            std::string output("");
            output += this->computed? commons::toString(this->getCost()) : "-";
            output += ",";
            output += this->computed? commons::toString(this->getThroughput()) : "-";
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
        std::ostream& operator<<(std::ostream& out, const algorithms::dse::TokenConfiguration& f)
        {
            return out << f.to_csv_line();
        }
    } // dse
} // algorithms
