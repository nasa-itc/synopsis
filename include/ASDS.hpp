#ifndef JPL_SYNOPSIS_ASDS
#define JPL_SYNOPSIS_ASDS

#include "synopsis_types.hpp"
#include "DpMsg.hpp"
#include "DpDbMsg.hpp"
#include "ApplicationModule.hpp"
#include "ASDPDB.hpp"


namespace Synopsis {


    // TODO: separate out reusable utility functions?
    size_t get_file_size(std::string filename);


    /*
     * Base class for an Autonomous Science Data System (ASDS)
     */
    class ASDS : public ApplicationModule {

        public:
            virtual ~ASDS() = default;
            virtual Status process_data_product(DpMsg msg) = 0;
            void set_database(ASDPDB *db);

        protected:
            Status submit_data_product(DpMsg msg);
            Status submit_data_product(DpDbMsg msg);

        private:
            ASDPDB *_db = nullptr;

    };


};


#endif
