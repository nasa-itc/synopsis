#include <cmath>

#include "Similarity.hpp"


namespace Synopsis {


    double _sq_euclidean_dist(
        std::vector<double> dd1, std::vector<double> dd2
    ) {
        double acc = 0.0;
        int n1 = dd1.size();
        int n2 = dd2.size();
        int n = std::min(n1, n2);

        for (int i = 0; i < n; i++) {
            double diff = dd1[i] - dd2[i];
            acc += (diff * diff);
        }

        return acc;
    }


    double _gaussian_similarity(
        double sigma,
        std::vector<double> dd1, std::vector<double> dd2
    ) {
        double dist_sq = _sq_euclidean_dist(dd1, dd2);
        return exp(-(dist_sq / (sigma * sigma)));
    }


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

    std::vector<double> SimilarityFunction::_extract_dd(
        std::map<std::string, DpMetadataValue> asdp
    ) {
        std::vector<double> dd;
        int n_dd = this->_diversity_descriptors.size();

        for (int i = 0; i < n_dd; i++) {
            std::string key = this->_diversity_descriptors[i];

            // TODO: handle missing key
            double dd_i = asdp[key].get_float_value();

            // Multiply by factor (if provided)
            if (i < this->_dd_factors.size()) {
                dd_i *= this->_dd_factors[i];
            }

            dd.push_back(dd_i);

        }

        return dd;
    }


    double SimilarityFunction::get_similarity(
        std::map<std::string, DpMetadataValue> asdp1,
        std::map<std::string, DpMetadataValue> asdp2
    ) {

        double similarity = 0.0;
        auto dd1 = this->_extract_dd(asdp1);
        auto dd2 = this->_extract_dd(asdp2);

        if (this->_similarity_type == "gaussian") {
            double sigma = 1.0;
            if (this->_similarity_params.count("sigma")) {
                sigma = this->_similarity_params["sigma"];
            } else {
                // TODO: Log warning about missing parameter
            }
            similarity = _gaussian_similarity(sigma, dd1, dd2);
        } else {
            // TODO: Log unknown similarity type
        }

        // TODO: Implement
        return similarity;
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
