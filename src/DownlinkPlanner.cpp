#include "DownlinkPlanner.hpp"

namespace Synopsis {


    void DownlinkPlanner::set_database(ASDPDB *db) {
        this->_db = db;
    }


    void DownlinkPlanner::set_clock(Clock *clock) {
        this->_clock = clock;
    }


};
