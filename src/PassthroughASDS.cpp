#include "PassthroughASDS.hpp"


namespace Synopsis {


    /*
     * Implement ASDS initialization
     */
    Status PassthroughASDS::init(void) {
        return SUCCESS;
    }


    size_t PassthroughASDS::memory_requirement(void) {
        return 0;
    }


    Status PassthroughASDS::process_data_product(DpMsg msg) {
        return this->submit_data_product(msg);
    }


};
