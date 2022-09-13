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


    Constraint::Constraint(
        std::vector<std::string> variables,
        BoolValueExpression *application_expression,
        ValueExpression *sum_field,
        double constraint_value
    ) :
        _variables(variables),
        _application_expression(application_expression),
        _sum_field(sum_field),
        _constraint_value(constraint_value)
    {

    }


    bool Constraint::apply(std::vector<std::map<std::string, DpMetadataValue>> asdps) {

        double aggregate = 0.0;
        DpMetadataValue value;

        if (_variables.size() == 1) {
            for (auto a : asdps) {
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments = {
                    {_variables[0], a}
                };
                if (_application_expression->get_value(assignments, asdps)) {
                    if (_sum_field) {
                        value = _sum_field->get_value(assignments, asdps);
                        if (value.is_numeric()) {
                            aggregate += value.get_numeric();
                        } else {
                            // TODO: Log error, value not aggregated
                        }
                    } else {
                        aggregate += 1;
                    }
                }
            }
            return aggregate < _constraint_value;

        } else {
            // TODO: Case not handled
            return true;
        }

        /* UNREACHABLE */

    }


    RuleSet::RuleSet() {}


    std::vector<Rule> RuleSet::get_rules(int bin) {
        if (this->_rule_map.count(bin)) {
            return this->_rule_map[bin];
        } else {
            return this->_default_rules;
        }
    }


    std::vector<Constraint> RuleSet::get_constraints(int bin) {
        if (this->_constraint_map.count(bin)) {
            return this->_constraint_map[bin];
        } else {
            return this->_default_constraints;
        }
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


    ComparatorExpression::ComparatorExpression(
        std::string comp,
        ValueExpression *left_expr,
        ValueExpression *right_expr
    ) :
        _comp(comp),
        _left_expr(left_expr),
        _right_expr(right_expr)
    {

    }


    bool ComparatorExpression::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        DpMetadataValue left_value = this->_left_expr->get_value(assignments, asdps);
        DpMetadataValue right_value = this->_right_expr->get_value(assignments, asdps);
        if (left_value.is_numeric() ^ right_value.is_numeric()) {
            // TODO: Log error (type mis-match)
            return false;
        }
        if (left_value.is_numeric()) {
            double left_value_dbl = left_value.get_numeric();
            double right_value_dbl = right_value.get_numeric();
            if (this->_comp == "==") {
                return (left_value_dbl == right_value_dbl);
            } else if (this->_comp == "!=") {
                return (left_value_dbl != right_value_dbl);
            } else if (this->_comp == ">") {
                return (left_value_dbl > right_value_dbl);
            } else if (this->_comp == ">=") {
                return (left_value_dbl >= right_value_dbl);
            } else if (this->_comp == "<") {
                return (left_value_dbl < right_value_dbl);
            } else if (this->_comp == "<=") {
                return (left_value_dbl <= right_value_dbl);
            } else {
                // TODO: Log error (unknown string comparision)
                return false;
            }
        } else {
            std::string left_value_str = left_value.get_string_value();
            std::string right_value_str = right_value.get_string_value();
            if (this->_comp == "==") {
                return (left_value_str == right_value_str);
            } else if (this->_comp == "!=") {
                return (left_value_str != right_value_str);
            } else {
                // TODO: Log error (unknown string comparision)
                return false;
            }
        }
    }


    StringConstant::StringConstant(std::string value) :
        _value(DpMetadataValue(value))
    {

    }

    DpMetadataValue StringConstant::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        return this->_value;
    }


    MinusExpression::MinusExpression(ValueExpression *expr) :
        _expr(expr)
    {

    }

    DpMetadataValue MinusExpression::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        DpMetadataValue value = this->_expr->get_value(assignments, asdps);
        if (value.is_numeric()) {
            return DpMetadataValue(-value.get_numeric());
        } else {
            // TODO: Warn not a number
            return DpMetadataValue(std::numeric_limits<double>::quiet_NaN());
        }
    }


    BinaryExpression::BinaryExpression(
        std::string op,
        ValueExpression *left_expr,
        ValueExpression *right_expr
    ) :
        _op(op),
        _left_expr(left_expr),
        _right_expr(right_expr)
    {

    }


    DpMetadataValue BinaryExpression::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        DpMetadataValue left_value = this->_left_expr->get_value(assignments, asdps);
        DpMetadataValue right_value = this->_right_expr->get_value(assignments, asdps);
        if (left_value.is_numeric() && right_value.is_numeric()) {
            double left_value_dbl = left_value.get_numeric();
            double right_value_dbl = right_value.get_numeric();
            if (this->_op == "*") {
                return DpMetadataValue(left_value_dbl * right_value_dbl);
            } else if (this->_op == "+") {
                return DpMetadataValue(left_value_dbl + right_value_dbl);
            } else if (this->_op == "-") {
                return DpMetadataValue(left_value_dbl - right_value_dbl);
            } else {
                // TODO: Warn operator not supported
                return DpMetadataValue(std::numeric_limits<double>::quiet_NaN());
            }
        } else {
            // TODO: Warn not a number
            return DpMetadataValue(std::numeric_limits<double>::quiet_NaN());
        }
    }


    Field::Field(
        std::string var_name,
        std::string field_name
    ) :
        _var_name(var_name),
        _field_name(field_name)
    {

    }


    DpMetadataValue Field::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        if (assignments.count(this->_var_name) > 0) {
            auto fields = assignments[this->_var_name];
            if (fields.count(this->_field_name) > 0) {
                return fields[this->_field_name];
            } else {
                // TODO: Field not found
                return DpMetadataValue(
                    std::numeric_limits<double>::quiet_NaN()
                );
            }

        } else {
            // TODO: Variable not found
            return DpMetadataValue(std::numeric_limits<double>::quiet_NaN());
        }
    }


    ExistentialExpression::ExistentialExpression(
        std::string variable,
        BoolValueExpression *expr
    ) :
        _var(variable),
        _expr(expr)
    {

    }


    bool ExistentialExpression::get_value(
            std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
            std::vector<std::map<std::string, DpMetadataValue>> asdps
        ) {
        for (auto asdp : asdps) {

            // Assign asdp to variable
            std::map<std::string, std::map<std::string, DpMetadataValue>> \
                new_assignments(assignments);
            new_assignments[this->_var] = asdp;

            // Evaluate expression and return if true
            bool evaluation = this->_expr->get_value(new_assignments, asdps);
            if (evaluation) { return true; }

        }
        return false;
    }


};
