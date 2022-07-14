#ifndef JPL_SYNOPSIS_StdLogger
#define JPL_SYNOPSIS_StdLogger

#include "Logger.hpp"


namespace Synopsis {


    class StdLogger : public Logger {

        public:
            virtual ~StdLogger() = default;


            /*
             * Logs a message of the specified type
             */
            void log(LogType type, const char* fmt, ...);


    };


};


#endif
