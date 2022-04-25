/*
 * Public SYNOPSIS functionality
 */
#include <cstddef>
#include <string>
#include <tuple>
#include <vector>

#include "synopsis.hpp"

namespace Synopsis {


    Application::Application(ASDPDB *db) :
        buffer_size(0),
        memory_buffer(NULL),
        _db(db),
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

        // Init ASDSs
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

        // Init ASDPDB
        mem = _db->memory_requirement();
        mem += Application::padding_nbytes(mem);
        status = _db->init(mem, (void*)((char*)memory + offset));
        if (status != SUCCESS) {
            return status;
        }
        offset += mem;
        if (offset > bytes) {
            return FAILURE;
        }

        return SUCCESS;
    }


    /*
     * Implement library deinitialization
     */
    Status Application::deinit(void) {
        Status status;

        // De-init ASDSs
        for (int i = 0; i < this->n_asds; i++) {
            ASDS *asds = std::get<2>(this->asds[i]);
            status = asds->deinit();
            if (status != SUCCESS) {
                return status;
            }
        }

        // De-init ASDPDB
        status = _db->deinit();
        if (status != SUCCESS) {
            return status;
        }

        return SUCCESS;
    }


    size_t Application::memory_requirement(void) {
        size_t base_memory_req = 0;
        size_t mem = 0;

        // Get ASDS memory requirement
        for (int i = 0; i < this->n_asds; i++) {
            ASDS *asds = std::get<2>(this->asds[i]);
            mem = asds->memory_requirement();
            mem += Application::padding_nbytes(mem);
            base_memory_req += mem;
        }

        // Get ASDPDB memory requirement
        mem = _db->memory_requirement();
        mem += Application::padding_nbytes(mem);
        base_memory_req += mem;

        return base_memory_req;
    }


    size_t Application::padding_nbytes(size_t block_size) {
        return (MEM_ALIGN_SIZE - (block_size % MEM_ALIGN_SIZE)) % MEM_ALIGN_SIZE;
    }

    Status Application::accept_dp(DpMsg msg) {

        std::string iname = msg.get_instrument_name();
        std::string dp_type = msg.get_type();

        Status status = SUCCESS;
        Status status_i;
        for (int i = 0; i < this->n_asds; i++) {

            std::string iname_i = std::get<0>(this->asds[i]);
            if (iname == iname_i) {

                std::string dp_type_i = std::get<1>(this->asds[i]);
                if ((dp_type_i == "") || (dp_type_i == dp_type)) {

                    ASDS *asds = std::get<2>(this->asds[i]);
                    status_i = asds->process_data_product(msg);
                    if (status_i != SUCCESS) {
                        // TODO: log status?
                        status = status_i;
                    }

                }
            }
        }
        return status;
    }


    Status Application::update_sue(int asdp_id, double sue) {
        // TODO: set SUE in ASDP DB
        return SUCCESS;
    }


    Status Application::update_priority_bin(int asdp_id, int bin) {
        // TODO: set bin in ASDP DB
        return SUCCESS;
    }


    Status Application::update_downlink_state(int asdp_id, DownlinkState state) {
        // TODO: set state in ASDP DB
        return SUCCESS;
    }


    Status Application::prioritize(
        std::string configuration_id, double max_processing_time_sec,
        std::vector<int> prioritized_list
    ) {
        /*
         * TODO:
         * 1. Load configuration
         * 2. Invoke planner with configuration and max_processing_time_sec
         * 3. Populate vector with sorted product IDs (do not exceed capacity)
         */
        return SUCCESS;
    }


};
