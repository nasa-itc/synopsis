#include "SqliteASDPDB.hpp"


namespace Synopsis {

    static constexpr const char* SQL_SCHEMA = R"(

    CREATE TABLE IF NOT EXISTS ASDP (
        asdp_id INTEGER PRIMARY KEY,
        instrument_name TEXT,
        type TEXT,
        uri TEXT,
        size INTEGER,
        science_utility_estimate REAL,
        priority_bin INTEGER,
        downlink_state INTEGER
    );

    CREATE TABLE IF NOT EXISTS METADATA (
        asdp_id INTEGER,
        fieldname TEXT NOT NULL,
        type INTEGER,
        value_int INTEGER,
        value_float REAL,
        value_string TEXT,
        FOREIGN KEY(asdp_id) REFERENCES ASDP(asdp_id)
    );

    )";


    SqliteASDPDB::SqliteASDPDB(std::string asdpdb_file) :
        asdpdb_file(asdpdb_file),
        _db(NULL)
    {

    }


    /*
     * Implement ASDS initialization
     */
    Status SqliteASDPDB::init(size_t bytes, void* memory) {
        int status;

        status= sqlite3_open(this->asdpdb_file.c_str(), &this->_db);
        if (status != SQLITE_OK) {
            // TODO: Log error
            return FAILURE;
        }

        char *errmsg;
        status = sqlite3_exec(this->_db, SQL_SCHEMA, NULL, NULL, &errmsg);
        if (status != SQLITE_OK) {
            // TODO: Log error
            printf("%s", errmsg);
            return FAILURE;
        }

        return SUCCESS;
    }


    /*
     * Implement ASDS de-initialization
     */
    Status SqliteASDPDB::deinit() {
        sqlite3_close(this->_db);
        return SUCCESS;
    }


    size_t SqliteASDPDB::memory_requirement(void) {
        return 0;
    }


    Status SqliteASDPDB::insert_data_product(DpDbMsg msg) {
        // TODO: Implement
        return SUCCESS;
    }


};
