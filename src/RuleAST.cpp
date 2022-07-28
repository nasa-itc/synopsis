#include "RuleAST.hpp"

namespace Synopsis {


    Rule::Rule(
        std::vector<std::string> variables,
        BoolValueExpression *application_expression,
        NumericValueExpression *adjustment_expression,
        int max_applications
    ) :
        _variables(variables),
        _application_expression(application_expression),
        _adjustment_expression(adjustment_expression),
        _max_applications(max_applications)
    {

    }


    double Rule::apply(std::vector<std::map<std::string, DpMetadataValue>> asdps) {

        int n_applications = 0;
        double total_adj_value = 0.0;

        if (_variables.size() == 1) {
            for (auto a : asdps) {
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments = {
                    {_variables[0], a}
                };
                if (_application_expression->get_value(assignments, asdps)) {
                    n_applications += 1;
                    total_adj_value += _adjustment_expression->get_value(assignments, asdps);
                    // TODO: Log application of rule?
                    if ((_max_applications >= 0) && (n_applications >= _max_applications)) {
                        break;
                    }
                }
            }
            return total_adj_value;

        } else if (_variables.size() == 2) {
            for (auto a : asdps) {
                for (auto b : asdps) {
                    std::map<std::string, std::map<std::string, DpMetadataValue>> assignments = {
                        {_variables[0], a},
                        {_variables[1], b}
                    };
                    if (_application_expression->get_value(assignments, asdps)) {
                        n_applications += 1;
                        total_adj_value += _adjustment_expression->get_value(assignments, asdps);
                        // TODO: Log application of rule?
                        if ((_max_applications >= 0) && (n_applications >= _max_applications)) {
                            break;
                        }
                    }
                }
            }
            return total_adj_value;

        } else {
            // TODO: Case not handled
            return total_adj_value;
        }

        /* UNREACHABLE */

    }


    LogicalConstant::LogicalConstant(bool value) :
        _value(value)
    {

    }


    bool LogicalConstant::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        return this->_value;
    }


    ConstExpression::ConstExpression(double value) :
        _value(value)
    {

    }

    double ConstExpression::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        return this->_value;
    }

};
