/*
 * Public SYNOPSIS interface
 */
#ifndef JPL_SYNOPSIS_PUBLIC
#define JPL_SYNOPSIS_PUBLIC
#include <string>
#include <tuple>

#include "synopsis_types.hpp"

#include "DpMsg.hpp"

#include "ApplicationModule.hpp"
#include "ASDS.hpp"
#include "PassthroughASDS.hpp"

#define MAX_SYNOPSIS_APP_ASDS 32
#define MEM_ALIGN_SIZE 8

namespace Synopsis {


    class Application {

        public:

            /*
             * Constructor
             */
            Application();

            /*
             * Performs library initialization
             */
            Status init(size_t bytes, void* memory);
            Status deinit(void);

            Status add_asds(std::string instrument_name, ASDS *asds);
            Status add_asds(std::string instrument_name, std::string dp_type,
                ASDS *asds);

            /*
             * Returns the required memory for the application
             */
            size_t memory_requirement(void);

            Status accept_dp(DpMsg msg);

        private:

            /*
             * Holds reference to memory buffer
             */
            size_t buffer_size;
            void *memory_buffer;

            int n_asds;
            std::tuple<std::string, std::string, ASDS*> asds[MAX_SYNOPSIS_APP_ASDS];

            static size_t padding_nbytes(size_t block_size);

    };


};


#endif
