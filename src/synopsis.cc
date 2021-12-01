/*
 * Public SYNOPSIS functionality
 */
#include <stdlib.h>
#include <synopsis.h>

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

int synopsis_hello_world(void) {
    return 12345;
}
