#ifndef JPL_SYNOPSIS_PassthroughASDS
#define JPL_SYNOPSIS_PassthroughASDS

#include "ASDS.hpp"


namespace Synopsis {


    class PassthroughASDS : public ASDS {

        public:
            virtual ~PassthroughASDS() = default;
            Status init(size_t bytes, void* memory);
            Status deinit(void);
            size_t memory_requirement(void);
            Status process_data_product(DpMsg msg);

    };


};


#endif
