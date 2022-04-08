
#ifndef JPL_SYNOPSIS_TYPES
#define JPL_SYNOPSIS_TYPES


namespace Synopsis {


    typedef enum {
        SUCCESS = 0,
        FAILURE = 1
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


};

#endif
