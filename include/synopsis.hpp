/*
 * Public SYNOPSIS interface
 */
#ifndef JPL_SYNOPSIS_PUBLIC
#define JPL_SYNOPSIS_PUBLIC

#include "synopsis_types.hpp"

#include "DpMsg.hpp"

#include "ApplicationModule.hpp"
#include "ASDS.hpp"
#include "PassthroughASDS.hpp"

#define MAX_SYNOPSIS_APP_MODULES 32

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
            Status init(void);

            Status add_module(ApplicationModule *module);

            /*
             * Returns the required memory for the application
             */
            size_t memory_requirement(void);

        private:

            /*
             * Holds reference to memory buffer
             */
            void *memory_buffer;

            int n_modules;
            ApplicationModule *modules[MAX_SYNOPSIS_APP_MODULES];

    };


};


#endif
