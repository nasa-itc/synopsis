/*
 * Public SYNOPSIS functionality
 */
#include <cstddef>

#include "synopsis.hpp"

namespace Synopsis {

    Application::Application() :
        memory_buffer(NULL)
    {

    }

    /*
     * Implement library initialization
     */
    Application::Status Application::init(void) {
        return SUCCESS;
    }

};
