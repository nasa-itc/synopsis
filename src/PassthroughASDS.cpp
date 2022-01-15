#include "PassthroughASDS.hpp"


namespace Synopsis {


    /*
     * Implement ASDS initialization
     */
    Status PassthroughASDS::init(size_t bytes, void* memory) {
        return SUCCESS;
    }


    /*
     * Implement ASDS de-initialization
     */
    Status PassthroughASDS::deinit() {
        return SUCCESS;
    }


    size_t PassthroughASDS::memory_requirement(void) {
        return 0;
    }


    Status PassthroughASDS::process_data_product(DpMsg msg) {
        return this->submit_data_product(msg);
    }


};
