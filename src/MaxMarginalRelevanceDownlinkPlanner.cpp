#include "MaxMarginalRelevanceDownlinkPlanner.hpp"


namespace Synopsis {


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
        std::vector<int> prioritized_list
    ) {
        /*
         * TODO:
         * 1. Load configuration
         * 2. Invoke planner with configuration and max_processing_time_sec
         * 3. Populate vector with sorted product IDs (do not exceed capacity)
         */
        return SUCCESS;
    }


};
