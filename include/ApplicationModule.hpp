#ifndef JPL_SYNOPSIS_ApplicationModule
#define JPL_SYNOPSIS_ApplicationModule

#include "synopsis_types.hpp"


namespace Synopsis {


    class ApplicationModule {

        public:

            /*
             * Performs module initialization
             */
            virtual Status init(size_t bytes, void* memory) = 0;
            virtual Status deinit() = 0;

            /*
             * Returns the required memory for the module
             */
            virtual size_t memory_requirement(void) = 0;
    };


};


#endif
