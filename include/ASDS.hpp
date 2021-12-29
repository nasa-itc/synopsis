#ifndef JPL_SYNOPSIS_ASDS
#define JPL_SYNOPSIS_ASDS

#include "synopsis_types.hpp"
#include "DpMsg.hpp"
#include "ApplicationModule.hpp"


namespace Synopsis {


    /*
     * Base class for an Autonomous Science Data System (ASDS)
     */
    class ASDS : public ApplicationModule {

        public:
            virtual Status process_data_product(DpMsg msg) = 0;

        protected:
            Status submit_data_product(DpMsg msg);
    };


};


#endif
