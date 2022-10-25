#ifndef JPL_SYNOPSIS_MaxMarginalRelevanceDownlinkPlanner
#define JPL_SYNOPSIS_MaxMarginalRelevanceDownlinkPlanner

#include "DownlinkPlanner.hpp"
#include "RuleAST.hpp"
#include "Similarity.hpp"


namespace Synopsis {

    Status _populate_asdp(DpDbMsg msg, std::map<std::string, DpMetadataValue> &asdp);

    std::vector<int> _prioritize_bin(
        int bin,
        std::vector<std::map<std::string, DpMetadataValue>> asdps,
        RuleSet ruleset, Similarity similarity
    );

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
                std::vector<int> &prioritized_list
            );

    };


};


#endif
