/*
 * Public SYNOPSIS interface
 */
#ifndef JPL_SYNOPSIS_PUBLIC
#define JPL_SYNOPSIS_PUBLIC
#include <string>

#define MAX_SYNOPSIS_APP_MODULES 32

namespace Synopsis {


    typedef enum {
        SUCCESS = 0,
        FAILURE = 1
    } Status;

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


    class ApplicationModule {

        public:

            // Constructor
            ApplicationModule();

            /*
             * Performs module initialization
             */
            virtual Status init(void) = 0;

            /*
             * Returns the required memory for the module
             */
            virtual size_t memory_requirement(void) = 0;
    };


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


    /*
     * Base class for an Autonomous Science Data System (ASDS)
     */
    class ASDS : public ApplicationModule {

        public:
            virtual Status process_data_product(DpMsg msg) = 0;

        protected:
            Status submit_data_product(DpMsg msg);
    };


    class PassthroughASDS : public ASDS {

        public:
            Status init(void);
            size_t memory_requirement(void);
            Status process_data_product(DpMsg msg);

    };

};

#endif
