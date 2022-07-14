#ifndef JPL_SYNOPSIS_Logger
#define JPL_SYNOPSIS_Logger

#include "synopsis_types.hpp"


namespace Synopsis {


    class Logger {

        public:
            virtual ~Logger() = default;


            /*
             * Logs a message of the specified type
             */
            virtual void log(LogType type, const char* fmt, ...) = 0;


    };


};


#endif
