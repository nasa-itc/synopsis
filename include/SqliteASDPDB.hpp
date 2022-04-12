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

        private:
            std::string asdpdb_file;
            sqlite3* _db;

    };


};


#endif
