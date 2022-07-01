#include <iostream>

#include "StdLogger.hpp"

namespace Synopsis {

    void StdLogger::log(LogType type, const char* fmt, ...) {
        // TODO: Change output stream depending on type?

        va_list args;
        va_start(args, fmt);

        switch (type) {
            case LogType::INFO:
                printf("[INFO ] ");
                break;
            case LogType::WARN:
                printf("[WARN ] ");
                break;
            case LogType::ERROR:
            default:
                printf("[ERROR] ");
                break;
        }
        vprintf(fmt, args);
        printf("\n");

        va_end(args);
    }

};
