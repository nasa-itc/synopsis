#ifndef JPL_SYNOPSIS_MaxMarginalRelevanceDownlinkPlanner
#define JPL_SYNOPSIS_MaxMarginalRelevanceDownlinkPlanner

#include "DownlinkPlanner.hpp"


namespace Synopsis {


    class MaxMarginalRelevanceDownlinkPlanner : public DownlinkPlanner {

        public:
            virtual ~MaxMarginalRelevanceDownlinkPlanner() = default;
            Status init(size_t bytes, void* memory, Logger *logger);
            Status deinit(void);
            size_t memory_requirement(void);
            Status prioritize(
                std::string rule_configuration_id,
                std::string similarity_configuration_id,
                double max_processing_time_sec,
                std::vector<int> prioritized_list
            );

    };


};


#endif
