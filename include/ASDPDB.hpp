#ifndef JPL_SYNOPSIS_ASDPDB
#define JPL_SYNOPSIS_ASDPDB
#include <vector>

#include "synopsis_types.hpp"
#include "ApplicationModule.hpp"
#include "DpDbMsg.hpp"


namespace Synopsis {


    /*
     * Base class for an Autonomous Science Data Product Database (ASDPDB)
     */
    class ASDPDB : public ApplicationModule {

        public:
            virtual Status insert_data_product(DpDbMsg& msg) = 0;
            virtual Status get_data_product(int asdp_id, DpDbMsg& msg) = 0;
            virtual std::vector<int> list_data_product_ids(void) = 0;
            virtual Status update_science_utility(int asdp_id, double sue) = 0;
            virtual Status update_priority_bin(int asdp_id, int bin) = 0;
            virtual Status update_downlink_state(int asdp_id, DownlinkState state) = 0;

    };


};


#endif
