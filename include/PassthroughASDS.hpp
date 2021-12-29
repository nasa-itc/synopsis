#ifndef JPL_SYNOPSIS_PassthroughASDS
#define JPL_SYNOPSIS_PassthroughASDS

#include "ASDS.hpp"


namespace Synopsis {


    class PassthroughASDS : public ASDS {

        public:
            Status init(void);
            size_t memory_requirement(void);
            Status process_data_product(DpMsg msg);

    };


};


#endif
