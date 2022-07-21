#ifndef JPL_SYNOPSIS_Timer
#define JPL_SYNOPSIS_Timer

#include "synopsis_types.hpp"
#include "Clock.hpp"


namespace Synopsis {


    class Timer {

        public:

            /*
             * Default Constructor
             */
            Timer(Clock *clock, double duration);
            ~Timer() = default;


            /*
             * Returns time in seconds
             */
            void start(void);
            bool is_expired(void);

        private:

            Clock *_clock;
            double duration;
            double start_time;


    };


};


#endif
