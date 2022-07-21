#include <ctime>

#include "LinuxClock.hpp"

namespace Synopsis {

    double LinuxClock::get_time(void) {
        std::time_t now;
        std::time(&now);
        return std::difftime(now, 0);
    }

};
