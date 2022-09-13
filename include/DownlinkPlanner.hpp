#ifndef JPL_SYNOPSIS_DownlinkPlanner
#define JPL_SYNOPSIS_DownlinkPlanner

#include <vector>

#include "synopsis_types.hpp"
#include "ApplicationModule.hpp"
#include "ASDPDB.hpp"
#include "Clock.hpp"


namespace Synopsis {


    /*
     * Base class for an Autonomous Science Data System (ASDS)
     */
    class DownlinkPlanner : public ApplicationModule {

        public:
            virtual ~DownlinkPlanner() = default;

            virtual Status prioritize(
                std::string rule_configuration_id,
                std::string similarity_configuration_id,
                double max_processing_time_sec,
                std::vector<int> prioritized_list
            ) = 0;
            void set_database(ASDPDB *db);
            void set_clock(Clock *clock);

        private:
            ASDPDB *_db = nullptr;
            Clock *_clock = nullptr;

    };


};


#endif
