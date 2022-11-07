#ifndef JPL_SYNOPSIS_Similarity
#define JPL_SYNOPSIS_Similarity

#include <vector>
#include <map>
#include <utility>

#include "synopsis_types.hpp"
#include "DpDbMsg.hpp"


namespace Synopsis {


    double _sq_euclidean_dist(
        std::vector<double> dd1, std::vector<double> dd2
    );


    double _gaussian_similarity(
        double sigma,
        std::vector<double> dd1, std::vector<double> dd2
    );


    class SimilarityFunction {

        public:
            SimilarityFunction(
                std::vector<std::string> diversity_descriptors,
                std::vector<double> dd_factors,
                std::string similarity_type,
                std::map<std::string, double> similarity_params
            );
            ~SimilarityFunction() = default;

            std::vector<double> _extract_dd(
                AsdpEntry asdp
            );

            double get_similarity(
                AsdpEntry asdp1,
                AsdpEntry asdp2
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
                double default_alpha,
                std::map<int, std::map<
                    std::pair<std::string, std::string>, SimilarityFunction
                >> functions,
                std::map<
                    std::pair<std::string, std::string>, SimilarityFunction
                > default_functions
            );
            ~Similarity() = default;


            double _get_cached_similarity(
                SimilarityFunction &similarity_function,
                AsdpEntry asdp1,
                AsdpEntry asdp2
            );

            double get_max_similarity(
                int bin,
                AsdpList queue,
                AsdpEntry asdp
            );

            double get_discount_factor(
                int bin,
                AsdpList queue,
                AsdpEntry asdp
            );

        private:
            std::map<int, std::map<
                std::pair<std::string, std::string>, SimilarityFunction
            >> _functions;
            std::map<
                std::pair<std::string, std::string>, SimilarityFunction
            > _default_functions;
            std::map<int, double> _alpha;
            double _default_alpha;
            std::map<std::pair<int, int>, double> _cache;

    };


    Similarity parse_similarity_config(std::string config_file);


};


#endif
