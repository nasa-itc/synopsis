/*
 * Public SYNOPSIS interface
 */
#ifndef JPL_SYNOPSIS_PUBLIC
#define JPL_SYNOPSIS_PUBLIC

namespace Synopsis {

    class Application {

        public:

            typedef enum {
                SUCCESS = 0,
                FAILURE = 1
            } Status;

            /*
             * Constructor
             */
            Application();

            /*
             * Performs library initialization
             */
            Status init(void);

        private:

            /*
             * Holds reference to memory buffer
             */
            void *memory_buffer;

    };

};

int synopsis_hello_world(void);

#endif
