/*
 * Public SYNOPSIS interface
 */
#ifndef JPL_SYNOPSIS_PUBLIC
#define JPL_SYNOPSIS_PUBLIC
#include <string>
#include <tuple>
#include <vector>

#include "synopsis_types.hpp"

#include "DpMsg.hpp"

#include "ApplicationModule.hpp"
#include "ASDS.hpp"
#include "ASDPDB.hpp"
#include "PassthroughASDS.hpp"
#include "Logger.hpp"
#include "Clock.hpp"
#include "DownlinkPlanner.hpp"

#define MAX_SYNOPSIS_APP_ASDS 32
#define MEM_ALIGN_SIZE 8

namespace Synopsis {


    class Application {

        public:

            /*
             * Constructor
             */
            Application(ASDPDB *db, DownlinkPlanner *planner, Logger *logger, Clock *clock);
            ~Application() = default;

            /*
             * Performs library initialization
             */
            Status init(size_t bytes, void* memory);
            Status deinit(void);

            Status add_asds(std::string instrument_name, ASDS *asds);
            Status add_asds(std::string instrument_name, std::string dp_type,
                ASDS *asds);

            /*
             * Returns the required memory for the application
             */
            size_t memory_requirement(void);

            Status accept_dp(DpMsg msg);

            Status update_science_utility(int asdp_id, double sue);
            Status update_priority_bin(int asdp_id, int bin);
            Status update_downlink_state(int asdp_id, DownlinkState state);
            template <typename T>
            Status update_asdp_metadata(int asdp_id, std::string fieldname, T value);

            Status prioritize(
                std::string rule_configuration_id,
                std::string similarity_configuration_id,
                double max_processing_time_sec,
                std::vector<int> prioritized_list
            );

        private:

            /*
             * Holds reference to memory buffer
             */
            size_t buffer_size;
            void *memory_buffer;

            ASDPDB *_db;
            DownlinkPlanner *_planner;
            Logger *_logger;
            Clock *_clock;

            int n_asds;
            std::tuple<std::string, std::string, ASDS*> asds[MAX_SYNOPSIS_APP_ASDS];

            static size_t padding_nbytes(size_t block_size);

    };


};


#endif
