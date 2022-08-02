#include "RuleAST.hpp"

namespace Synopsis {


    Rule::Rule(
        std::vector<std::string> variables,
        BoolValueExpression *application_expression,
        ValueExpression *adjustment_expression,
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
        DpMetadataValue adj;

        if (_variables.size() == 1) {
            for (auto a : asdps) {
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments = {
                    {_variables[0], a}
                };
                if (_application_expression->get_value(assignments, asdps)) {
                    adj = _adjustment_expression->get_value(assignments, asdps);
                    if (adj.is_numeric()) {
                        total_adj_value += adj.get_numeric();
                        n_applications += 1;
                    } else {
                        // TODO: Log error, no application/adjustment
                    }
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
                        adj = _adjustment_expression->get_value(assignments, asdps);
                        if (adj.is_numeric()) {
                            total_adj_value += adj.get_numeric();
                            n_applications += 1;
                        } else {
                            // TODO: Log error, no application/adjustment
                        }
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
        _value(DpMetadataValue(value))
    {

    }

    DpMetadataValue ConstExpression::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        return this->_value;
    }


    LogicalNot::LogicalNot(BoolValueExpression *expr) :
        _expr(expr)
    {

    }


    bool LogicalNot::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        return !(this->_expr->get_value(assignments, asdps));
    }


    BinaryLogicalExpression::BinaryLogicalExpression(
        std::string op,
        BoolValueExpression *left_expr,
        BoolValueExpression *right_expr
    ) :
        _op(op),
        _left_expr(left_expr),
        _right_expr(right_expr)
    {

    }


    bool BinaryLogicalExpression::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        bool left_value = this->_left_expr->get_value(assignments, asdps);
        bool right_value = this->_right_expr->get_value(assignments, asdps);
        if (this->_op == "AND") {
            return left_value && right_value;
        } else if (this->_op == "OR") {
            return left_value || right_value;
        } else {
            // TODO: Log error
            return false;
        }
    }

};
