
#ifndef JPL_SYNOPSIS_TYPES
#define JPL_SYNOPSIS_TYPES

#include <cstddef>


namespace Synopsis {


    typedef enum {
        SUCCESS = 0,
        FAILURE = 1,
        TIMEOUT = 2
    } Status;


    typedef enum {
        UNTRANSMITTED = 0,
        TRANSMITTED = 1,
        DOWNLINKED = 2
    } DownlinkState;


    typedef enum {
        INT = 0,
        FLOAT = 1,
        STRING = 2
    } MetadataType;


    typedef enum {
        INFO = 0,
        WARN = 1,
        ERROR = 2
    } LogType;


};

#endif
