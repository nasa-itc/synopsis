/*
 * Public SYNOPSIS functionality
 */
#include <cstddef>

#include "synopsis.hpp"

namespace Synopsis {


    Application::Application() :
        memory_buffer(NULL),
        n_modules(0)
    {

    }


    Status Application::add_module(ApplicationModule *module) {
        if (this->n_modules >= MAX_SYNOPSIS_APP_MODULES) {
            return FAILURE;
        }
        this->modules[this->n_modules] = module;
        this->n_modules += 1;
        return SUCCESS;
    }


    /*
     * Implement library initialization
     */
    Status Application::init(void) {
        return SUCCESS;
    }


    size_t Application::memory_requirement(void) {
        size_t base_memory_req = 0;
        for (int i = 0; i < this->n_modules; i++) {
            base_memory_req += this->modules[i]->memory_requirement();
        }
        return base_memory_req;
    }


};
