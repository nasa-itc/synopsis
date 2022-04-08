#ifndef JPL_SYNOPSIS_DpDbMsg
#define JPL_SYNOPSIS_DpDbMsg
#include <string>
#include <map>

#include "synopsis_types.hpp"


namespace Synopsis {


    class DpMetadataValue {

        public:

            DpMetadataValue();

            DpMetadataValue(MetadataType type,
                int int_value, double float_value, std::string string_value);

            MetadataType get_type(void);
            int get_int_value(void);
            double get_float_value(void);
            std::string get_string_value(void);

        private:
            MetadataType type;
            int int_value;
            double float_value;
            std::string string_value;

    };


    class DpDbMsg {

        public:

            /*
             * Default Constructor
             */
            DpDbMsg();

            DpDbMsg(int dp_id, std::string instrument_name, std::string dp_type,
                std::string dp_uri, size_t dp_size, double science_utility_estimate,
                int prioity_bin, DownlinkState downlink_state,
                std::map<std::string, DpMetadataValue> metadata);

            int get_dp_id(void);
            std::string get_instrument_name(void);
            std::string get_type(void);
            std::string get_uri(void);
            size_t get_dp_size(void);
            double get_science_utility_estimate(void);
            int get_priority_bin(void);
            DownlinkState get_downlink_state(void);
            std::map<std::string, DpMetadataValue> get_metadata(void);

        private:
            int dp_id;
            std::string instrument_name;
            std::string dp_type;
            std::string dp_uri;
            size_t dp_size;
            double science_utility_estimate;
            int prioity_bin;
            DownlinkState downlink_state;
            std::map<std::string, DpMetadataValue> metadata;
    };


};


#endif
