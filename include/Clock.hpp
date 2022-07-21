#ifndef JPL_SYNOPSIS_Clock
#define JPL_SYNOPSIS_Clock

#include "synopsis_types.hpp"


namespace Synopsis {


    class Clock {

        public:
            virtual ~Clock() = default;


            /*
             * Returns time in seconds
             */
            virtual double get_time(void) = 0;


    };


};


#endif
