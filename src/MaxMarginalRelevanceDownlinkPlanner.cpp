#include "MaxMarginalRelevanceDownlinkPlanner.hpp"
#include "Timer.hpp"


namespace Synopsis {


    Status _populate_asdp(DpDbMsg msg, std::map<std::string, DpMetadataValue> &asdp) {

        // Initialize with existing metadata
        asdp = msg.get_metadata();

        // Add "first class" metadata fields
        asdp["id"] = DpMetadataValue(msg.get_dp_id());
        asdp["instrument_name"] = DpMetadataValue(msg.get_instrument_name());
        asdp["type"] = DpMetadataValue(msg.get_type());
        asdp["size"] = DpMetadataValue((int)msg.get_dp_size());
        asdp["science_utility_estimate"] = DpMetadataValue(msg.get_science_utility_estimate());
        asdp["priority_bin"] = DpMetadataValue(msg.get_priority_bin());

        return SUCCESS;
    }


    std::vector<int> _prioritize_bin(
        int bin,
        std::vector<std::map<std::string, DpMetadataValue>> asdps,
        RuleSet ruleset,
        Similarity similarity
    ) {
        std::vector<std::map<std::string, DpMetadataValue>> prioritized;
        int maxiter = asdps.size();

        int cumulative_size = 0;
        double cumulative_sue = 0.0;

        for (int i = 0; i < maxiter; i++) {

            int idx = -1;
            int best_idx = -1;
            double best_value = 0.0;
            for (auto &asdp : asdps) {
                idx += 1;
                std::vector<std::map<std::string, DpMetadataValue>> candidate(prioritized);
                candidate.push_back(asdp);

                // Compute similarity discount factor
                double discount_factor = similarity.get_discount_factor(
                    bin, prioritized, asdp
                );

                // Compute final SUE value
                double final_sue = (
                    discount_factor *
                    asdp["science_utility_estimate"].get_float_value()
                );
                asdp["final_science_utility_estimate"] = DpMetadataValue(final_sue);

                // Compute candidate cumulative utility and size
                double candidate_utility = cumulative_sue + final_sue;
                int candidate_size = cumulative_size + asdp["size"].get_int_value();

                auto applied = ruleset.apply(bin, candidate);
                if (!applied.first) {
                    // Constraints violated
                    continue;
                }

                // Apply rule adjustement
                candidate_utility += applied.second;

                double relative_utility = candidate_utility / candidate_size;
                if ((best_idx < 0) || (relative_utility > best_value)) {
                    best_idx = idx;
                    best_value = relative_utility;
                }

            }

            // No valid successor was found
            if (best_idx < 0) {
                break;
            }

            // Push best ASDP onto prioritized list
            auto best_asdp = asdps[best_idx];
            prioritized.push_back(best_asdp);
            asdps.erase(asdps.begin() + best_idx);
            cumulative_size += best_asdp["size"].get_int_value();
            cumulative_sue += best_asdp["final_science_utility_estimate"].get_float_value();

        }

        std::vector<int> prioritized_ids;
        for (auto &asdp : prioritized) {
            prioritized_ids.push_back(asdp["id"].get_int_value());
        }

        return prioritized_ids;
    }


    /*
     * Implement DownlinkPlanner initialization
     */
    Status MaxMarginalRelevanceDownlinkPlanner::init(
        size_t bytes, void* memory, Logger *logger
    ) {
        this->_logger = logger;
        return SUCCESS;
    }


    /*
     * Implement DownlinkPlanner de-initialization
     */
    Status MaxMarginalRelevanceDownlinkPlanner::deinit() {
        return SUCCESS;
    }


    size_t MaxMarginalRelevanceDownlinkPlanner::memory_requirement(void) {
        return 0;
    }


    Status MaxMarginalRelevanceDownlinkPlanner::prioritize(
        std::string rule_configuration_id,
        std::string similarity_configuration_id,
        double max_processing_time_sec,
        std::vector<int> &prioritized_list
    ) {

        Status status;

        Timer timer(this->_clock, max_processing_time_sec);
        timer.start();

        // Parse/Load RuleSet
        RuleSet ruleset = parse_rule_config(rule_configuration_id);

        // Load similarity configuration
        Similarity similarity = \
            parse_similarity_config(similarity_configuration_id);

        // Load ASDPs
        std::vector<int> dp_ids = this->_db->list_data_product_ids();
        std::map<int, std::vector<std::map<std::string, DpMetadataValue>>> binned_asdps;
        std::vector<std::map<std::string, DpMetadataValue>> transmitted;
        DpDbMsg msg;
        for (int dp_id : dp_ids) {
            Status status = this->_db->get_data_product(dp_id, msg);
            if (status != SUCCESS) { return status; }

            DownlinkState dl_state = msg.get_downlink_state();
            if (dl_state == DOWNLINKED) { continue; }

            int bin = msg.get_priority_bin();

            std::map<std::string, DpMetadataValue> asdp;
            status = _populate_asdp(msg, asdp);
            if (status != SUCCESS) {
                // TODO: Log Error
                return status;
            }

            if (dl_state == TRANSMITTED) {
                transmitted.push_back(asdp);
            } else {
                if (binned_asdps.count(bin)) {
                    binned_asdps[bin].push_back(asdp);
                } else {
                    std::vector<std::map<std::string, DpMetadataValue>> binlist = { asdp };
                    binned_asdps[bin] = binlist;
                }
            }

        }

        if (timer.is_expired()) {
            return TIMEOUT;
        }

        // Prioritize each bin (assumes entries are traversed in bin order)
        for (auto &entry : binned_asdps) {
            int bin = entry.first;
            auto &asdps = entry.second;
            std::vector<int> prioritized_bin = _prioritize_bin(
                bin, asdps, ruleset, similarity
            );
            for (int asdp_id : prioritized_bin) {
                prioritized_list.push_back(asdp_id);
            }
        }

        return SUCCESS;
    }


};
