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
        >> functions,
        std::map<
            std::pair<std::string, std::string>, SimilarityFunction
        > default_functions
    ) :
        _alpha(alpha),
        _functions(functions),
        _default_functions(default_functions)
    {

    }


    double Similarity::_get_cached_similarity(
        SimilarityFunction &similarity_function,
        std::map<std::string, DpMetadataValue> asdp1,
        std::map<std::string, DpMetadataValue> asdp2
    ) {

        // Make cache key
        std::pair<int, int> cache_key;
        int aid1 = asdp1["id"].get_int_value();
        int aid2 = asdp2["id"].get_int_value();

        // Smaller ASDP ID is first element
        if (aid1 < aid2) {
            cache_key = std::make_pair(aid1, aid2);
        } else {
            cache_key = std::make_pair(aid2, aid1);
        }

        // Compute similarity if not in cache
        double similarity;
        if (!this->_cache.count(cache_key)) {
            similarity = similarity_function.get_similarity(asdp1, asdp2);
            this->_cache[cache_key] = similarity;
        } else {
            similarity = this->_cache[cache_key];
        }

        return similarity;
    }


    double Similarity::get_max_similarity(
        int bin,
        std::vector<std::map<std::string, DpMetadataValue>> queue,
        std::map<std::string, DpMetadataValue> asdp
    ) {

        if (queue.size() == 0) {
            // Special case: queue is empty
            return 0.0;
        }

        auto inst_type = std::make_pair(
            asdp["instrument_name"].get_string_value(),
            asdp["type"].get_string_value()
        );

        std::map<std::pair<std::string, std::string>, SimilarityFunction> sf;
        if (this->_functions.count(bin)) {
            sf = this->_functions[bin];
        } else {
            sf = this->_default_functions;
        }

        // Get similarity function
        if (!sf.count(inst_type)) {
            // No similarity function specified for this ASDP
            return 0.0;
        }
        SimilarityFunction &sim = sf.at(inst_type);

        // Compute max similarity
        int aid1 = asdp["id"].get_int_value();
        double max_similarity = 0.0;
        for (auto &asdp2 : queue) {

            // Check for type match
            auto inst_type2 = std::make_pair(
                asdp2["instrument_name"].get_string_value(),
                asdp2["type"].get_string_value()
            );
            if (inst_type != inst_type2) {
                continue;
            }

            double similarity = this->_get_cached_similarity(
                sim, asdp, asdp2
            );

            if (similarity > max_similarity) {
                max_similarity = similarity;
            }

        }

        return max_similarity;
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
