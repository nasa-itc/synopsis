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
        std::map<int, double> alpha,
        std::map<int, std::map<
            std::pair<std::string, std::string>, SimilarityFunction
        >> functions
    ) :
        _alpha(alpha),
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


    double Similarity::get_discount_factor(
        int bin,
        std::vector<std::map<std::string, DpMetadataValue>> queue,
        std::map<std::string, DpMetadataValue> asdp
    ) {
        double max_similarity = this->get_max_similarity(bin, queue, asdp);
        double alpha = 1.0;
        if (this->_alpha.count(bin)) {
            // If alpha specified, overwrite default 1.0 value
            alpha = this->_alpha[bin];
        }
        return (1.0 - alpha) + (alpha * (1.0 - max_similarity));
    }

};
