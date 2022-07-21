#ifndef JPL_SYNOPSIS_LinuxClock
#define JPL_SYNOPSIS_LinuxClock

#include "Clock.hpp"


namespace Synopsis {


    class LinuxClock : public Clock {

        public:
            virtual ~LinuxClock() = default;


            /*
             * Returns time in seconds
             */
            double get_time(void);


    };


};


#endif
