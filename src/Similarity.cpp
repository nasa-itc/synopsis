#include "Similarity.hpp"

namespace Synopsis {

    SimilarityFunction::SimilarityFunction(
        std::vector<std::string> diversity_descriptors,
        std::vector<double> dd_factors,
        std::string similarity_type,
        std::map<std::string, double> similarity_params
    ) :
        _diversity_descriptors(diversity_descriptors),
        _dd_factors(dd_factors),
        _similarity_type(similarity_type),
        _similarity_params(similarity_params)
    {

    }

    double SimilarityFunction::get_similarity(
        std::map<std::string, DpMetadataValue> asdp1,
        std::map<std::string, DpMetadataValue> asdp2
    ) {
        // TODO: Implement
        return 0.0;
    }

    Similarity::Similarity(
        std::map<int, std::map<
            std::pair<std::string, std::string>, SimilarityFunction
        >> functions
    ) :
        _functions(functions)
    {

    }


    double Similarity::get_max_similarity(
        int bin,
        std::vector<std::map<std::string, DpMetadataValue>> queue,
        std::map<std::string, DpMetadataValue> asdp
    ) {
        // TODO: Implement
        return 0.0;
    }

};
