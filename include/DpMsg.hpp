#ifndef JPL_SYNOPSIS_DpMsg
#define JPL_SYNOPSIS_DpMsg
#include <string>


namespace Synopsis {


    class DpMsg {

        public:

            /*
             * Default Constructor
             */
            DpMsg();

            DpMsg(std::string instrument_name, std::string dp_type,
                std::string dp_uri, std::string meta_uri, bool meta_usage);

            std::string get_instrument_name(void);
            std::string get_type(void);
            std::string get_uri(void);
            std::string get_metadata_uri(void);
            bool get_metadata_usage(void);

        private:
            std::string instrument_name;
            std::string dp_type;
            std::string dp_uri;
            std::string meta_uri;
            bool meta_usage;
    };


};


#endif
