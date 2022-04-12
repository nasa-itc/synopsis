#ifndef JPL_SYNOPSIS_ASDPDB
#define JPL_SYNOPSIS_ASDPDB

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

    };


};


#endif
