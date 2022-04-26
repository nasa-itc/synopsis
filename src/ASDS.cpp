#include <nlohmann/json.hpp>

#include "ASDS.hpp"

namespace Synopsis {

    void ASDS::set_database(ASDPDB *db) {
        this->_db = db;
    }

    Status ASDS::submit_data_product(DpMsg msg) {
        // TODO: Convert DpMsg to DpDbMsg
        return SUCCESS;
    }


    Status ASDS::submit_data_product(DpDbMsg msg) {
        if(this->_db == nullptr) {
            // TODO: Log no DB instance available
            return FAILURE;
        }
        return this->_db->insert_data_product(msg);
    }

};
