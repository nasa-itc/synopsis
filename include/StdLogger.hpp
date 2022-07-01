#ifndef JPL_SYNOPSIS_StdLogger
#define JPL_SYNOPSIS_StdLogger

#include "Logger.hpp"


namespace Synopsis {


    class StdLogger : public Logger {

        public:
            virtual ~StdLogger() = default;


            /*
             * Returns the required memory for the module
             */
            void log(LogType type, const char* fmt, ...);


    };


};


#endif
