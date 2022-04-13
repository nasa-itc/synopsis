
#ifndef JPL_SYNOPSIS_SQL
#define JPL_SYNOPSIS_SQL


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

    static constexpr const char* SQL_ASDP_INSERT = R"(

    INSERT INTO ASDP (
        instrument_name, type, uri, size,
        science_utility_estimate, priority_bin, downlink_state
    ) VALUES (?, ?, ?, ?, ?, ?, ?);

    )";

    static constexpr const char* SQL_ASDP_METADATA_INSERT = R"(

    INSERT INTO METADATA (
        asdp_id, fieldname, type, value_int, value_float, value_string
    ) VALUES (?, ?, ?, ?, ?, ?);

    )";

    static constexpr const char* SQL_ASDP_SELECT = R"(

    SELECT asdp_id FROM ASDP;

    )";

    static constexpr const char* SQL_ASDP_GET = R"(

    SELECT
        asdp_id, instrument_name, type, uri, size,
        science_utility_estimate, priority_bin, downlink_state
    FROM ASDP WHERE asdp_id=?;

    )";

    static constexpr const char* SQL_ASDP_METADATA_GET = R"(

    SELECT
        fieldname, type, value_int, value_float, value_string
    FROM METADATA WHERE asdp_id=?;

    )";


};

#endif
