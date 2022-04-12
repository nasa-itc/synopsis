#include "SqliteASDPDB.hpp"
#include "Sqlite3Statement.hpp"


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

        // Open database
        status= sqlite3_open(this->asdpdb_file.c_str(), &this->_db);
        if (status != SQLITE_OK) {
            // TODO: Log error
            return FAILURE;
        }

        // Initialize schema
        status = sqlite3_exec(this->_db, SQL_SCHEMA, NULL, NULL, NULL);
        if (status != SQLITE_OK) {
            // TODO: Log error
            return FAILURE;
        }

        return SUCCESS;
    }


    /*
     * Implement ASDS de-initialization
     */
    Status SqliteASDPDB::deinit() {
        // Close database connection
        sqlite3_close(this->_db);
        return SUCCESS;
    }


    size_t SqliteASDPDB::memory_requirement(void) {
        return 0;
    }


    Status SqliteASDPDB::insert_data_product(DpDbMsg& msg) {
        int dp_id;

        // Begin Transaction
        sqlite3_exec(this->_db, "BEGIN", NULL, NULL, NULL);

        try {

            // Insert ASDP
            Sqlite3Statement stmt(this->_db, std::string(R"(
                INSERT INTO ASDP (
                    instrument_name, type, uri, size,
                    science_utility_estimate, priority_bin, downlink_state)
                VALUES (?, ?, ?, ?, ?, ?, ?);
            )"));
            stmt.bind(0, msg.get_instrument_name());
            stmt.bind(1, msg.get_type());
            stmt.bind(2, msg.get_uri());
            stmt.bind(3, msg.get_dp_size());
            stmt.bind(4, msg.get_science_utility_estimate());
            stmt.bind(5, msg.get_priority_bin());
            stmt.bind(6, (int) msg.get_downlink_state());
            stmt.step();

            // Get ID of inserted ASDP
            dp_id = sqlite3_last_insert_rowid(this->_db);
            msg.set_dp_id(dp_id);

            // Insert Metadata
            for (auto const& pair : msg.get_metadata()) {
                std::string key = pair.first;
                DpMetadataValue value = pair.second;
                Sqlite3Statement stmt2(this->_db, std::string(R"(
                    INSERT INTO METADATA (
                        asdp_id, fieldname, type, value_int,
                        value_float, value_string)
                    VALUES (?, ?, ?, ?, ?, ?);
                )"));
                stmt2.bind(0, dp_id);
                stmt2.bind(1, key);
                stmt2.bind(2, (int) value.get_type());
                stmt2.bind(3, value.get_int_value());
                stmt2.bind(4, value.get_float_value());
                stmt2.bind(5, value.get_string_value());
                stmt2.step();
            }

            // Commit transaction
            sqlite3_exec(this->_db, "COMMIT", NULL, NULL, NULL);

        } catch (...) {

            // Rollback transaction and return error
            sqlite3_exec(this->_db, "ROLLBACK", NULL, NULL, NULL);

            // TODO: Log Error
            return FAILURE;
        }

        return SUCCESS;
    }


};
