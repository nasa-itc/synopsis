#ifndef JPL_SYNOPSIS_Similarity
#define JPL_SYNOPSIS_Similarity

#include <vector>
#include <map>
#include <utility>

#include "synopsis_types.hpp"
#include "DpDbMsg.hpp"


namespace Synopsis {


    class SimilarityFunction {

        public:
            SimilarityFunction(
                std::vector<std::string> diversity_descriptors,
                std::vector<double> dd_factors,
                std::string similarity_type,
                std::map<std::string, double> similarity_params
            );
            ~SimilarityFunction() = default;

            double get_similarity(
                std::map<std::string, DpMetadataValue> asdp1,
                std::map<std::string, DpMetadataValue> asdp2
            );

        private:
            std::vector<std::string> _diversity_descriptors;
            std::vector<double> _dd_factors;
            std::string _similarity_type;
            std::map<std::string, double> _similarity_params;

    };


    class Similarity {

        public:
            Similarity(
                std::map<int, double> alpha,
                std::map<int, std::map<
                    std::pair<std::string, std::string>, SimilarityFunction
                >> functions
            );
            ~Similarity() = default;

            double get_max_similarity(
                int bin,
                std::vector<std::map<std::string, DpMetadataValue>> queue,
                std::map<std::string, DpMetadataValue> asdp
            );

            double get_discount_factor(
                int bin,
                std::vector<std::map<std::string, DpMetadataValue>> queue,
                std::map<std::string, DpMetadataValue> asdp
            );

        private:
            std::map<int, std::map<
                std::pair<std::string, std::string>, SimilarityFunction
            >> _functions;
            std::map<int, double> _alpha;
            std::map<std::pair<int, int>, double> _cache;

    };


};


#endif
