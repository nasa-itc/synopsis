/*
 * Public SYNOPSIS interface
 */
#ifndef JPL_SYNOPSIS_PUBLIC
#define JPL_SYNOPSIS_PUBLIC
#include <string>

namespace Synopsis {

    class DpMsg {

        public:

            /*
             * Default Constructor
             */
            DpMsg();

            DpMsg(std::string instrument_name, std::string dp_type,
                std::string dp_uri, std::string meta_uri, bool meta_usage);

            std::string get_instrument_name(void);
            std::string get_type(void);
            std::string get_uri(void);
            std::string get_metadata_uri(void);
            bool get_metadata_usage(void);

        private:
            std::string instrument_name;
            std::string dp_type;
            std::string dp_uri;
            std::string meta_uri;
            bool meta_usage;
    };


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

    class ApplicationModule {

        public:

            // Constructor
            ApplicationModule();

            /*
             * Performs module initialization
             */
            Application::Status init(void);

            /*
             * Returns the required memory for the module
             */
            size_t memory_requirement(void);
    };

    /*
     * Base class for an Autonomous Science Data System (ASDS)
     */
    class ASDS : public ApplicationModule {

        public:
            virtual Application::Status process_data_product(DpMsg msg) = 0;

        private:
            Application::Status submit_data_product(DpMsg msg);
    };

};

#endif
