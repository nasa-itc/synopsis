#ifndef JPL_SYNOPSIS_SqliteASDPDB
#define JPL_SYNOPSIS_SqliteASDPDB
#include <string>
#include <sqlite3.h>

#include "ASDPDB.hpp"


namespace Synopsis {


    class SqliteASDPDB : public ASDPDB {

        public:

            SqliteASDPDB(std::string asdpdb_file);

            Status init(size_t bytes, void* memory);
            Status deinit(void);
            size_t memory_requirement(void);

            Status insert_data_product(DpDbMsg& msg);
            Status get_data_product(int asdp_id, DpDbMsg& msg);
            std::vector<int> list_data_product_ids(void);
            Status update_science_utility(int asdp_id, double sue);
            Status update_priority_bin(int asdp_id, int bin);
            Status update_downlink_state(int asdp_id, DownlinkState state);

        private:
            std::string asdpdb_file;
            sqlite3* _db;

    };


};


#endif
