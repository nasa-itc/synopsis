/*
 * Public SYNOPSIS functionality
 */
#include <cstddef>
#include <string>
#include <tuple>

#include "synopsis.hpp"

namespace Synopsis {


    Application::Application() :
        buffer_size(0),
        memory_buffer(NULL),
        n_asds(0)
    {

    }


    Status Application::add_asds(std::string instrument_name, ASDS *module) {
        return this->add_asds(instrument_name, "", module);
    }


    Status Application::add_asds(std::string instrument_name,
            std::string dp_type, ASDS *module) {
        if (this->n_asds >= MAX_SYNOPSIS_APP_ASDS) {
            return FAILURE;
        }
        this->asds[this->n_asds] =
            std::make_tuple(instrument_name, dp_type, module);
        this->n_asds += 1;
        return SUCCESS;
    }


    /*
     * Implement library initialization
     */
    Status Application::init(size_t bytes, void* memory) {
        Status status;
        void* pntr;
        size_t mem = 0;
        size_t offset = 0;
        for (int i = 0; i < this->n_asds; i++) {
            ASDS *asds = std::get<2>(this->asds[i]);
            mem = asds->memory_requirement();
            mem += Application::padding_nbytes(mem);

            status = asds->init(mem, (void*)((char*)memory + offset));
            if (status != SUCCESS) {
                return status;
            }

            offset += mem;
            if (offset > bytes) {
                return FAILURE;
            }
        }
        return SUCCESS;
    }


    size_t Application::memory_requirement(void) {
        size_t base_memory_req = 0;
        size_t mem = 0;
        for (int i = 0; i < this->n_asds; i++) {
            ASDS *asds = std::get<2>(this->asds[i]);
            mem = asds->memory_requirement();
            mem += Application::padding_nbytes(mem);
            base_memory_req += mem;
        }
        return base_memory_req;
    }


    size_t Application::padding_nbytes(size_t block_size) {
        return (MEM_ALIGN_SIZE - (block_size % MEM_ALIGN_SIZE)) % MEM_ALIGN_SIZE;
    }


};
