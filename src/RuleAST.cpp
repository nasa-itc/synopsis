#include <limits>
#include <nlohmann/json.hpp>
#include <fstream>

#include "RuleAST.hpp"

namespace Synopsis {


    /*
     * PRIVATE FUNCTION PROTOTYPES
     */


    std::pair<RuleList, ConstraintList>
    _parse_bin(
            nlohmann::json &j_bin,
            std::vector<std::shared_ptr<RuleExpression>> &exprs
    );


    template<class T> T _parse_argument(
        nlohmann::json &j_obj, std::string arg,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    );


    Rule _parse_rule(
        nlohmann::json &j_rule,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    );


    Constraint _parse_constraint(
        nlohmann::json &j_constraint,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    );


    std::string _get_obj_type(nlohmann::json &j_obj);


    Status _get_argument_obj(
        nlohmann::json *result, nlohmann::json &j_obj, std::string arg
    );


    /*
     * IMPLEMENTATIONS
     */


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


    double Rule::apply(AsdpList asdps) {

        int n_applications = 0;
        double total_adj_value = 0.0;
        DpMetadataValue adj;

        if (_variables.size() == 1) {
            for (auto a : asdps) {
                AsdpAssignments assignments = {
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
                    AsdpAssignments assignments = {
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


    bool Constraint::apply(AsdpList asdps) {

        double aggregate = 0.0;
        DpMetadataValue value;

        if (_variables.size() == 1) {
            for (auto a : asdps) {
                AsdpAssignments assignments = {
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


    RuleSet::RuleSet():
        _rule_map({}),
        _constraint_map({}),
        _default_rules({}),
        _default_constraints({}),
        _expressions({})
    {

    }


    RuleSet::RuleSet(
        std::map<int, RuleList> rule_map,
        std::map<int, ConstraintList> constraint_map,
        RuleList default_rules,
        ConstraintList default_constraints
    ) :
        _rule_map(rule_map),
        _constraint_map(constraint_map),
        _default_rules(default_rules),
        _default_constraints(default_constraints),
        _expressions({})
    {

    }


    RuleSet::RuleSet(
        std::map<int, RuleList> rule_map,
        std::map<int, ConstraintList> constraint_map,
        RuleList default_rules,
        ConstraintList default_constraints,
        std::vector<std::shared_ptr<RuleExpression>> expressions
    ) :
        _rule_map(rule_map),
        _constraint_map(constraint_map),
        _default_rules(default_rules),
        _default_constraints(default_constraints),
        _expressions(expressions)
    {

    }


    RuleList RuleSet::get_rules(int bin) {
        if (this->_rule_map.count(bin)) {
            return this->_rule_map[bin];
        } else {
            return this->_default_rules;
        }
    }


    ConstraintList RuleSet::get_constraints(int bin) {
        if (this->_constraint_map.count(bin)) {
            return this->_constraint_map[bin];
        } else {
            return this->_default_constraints;
        }
    }


    std::pair<bool, double> RuleSet::apply(
        int bin,
        AsdpList queue
    ) {

        // Check constraints
        ConstraintList constraints = this->get_constraints(bin);
        bool violated = false;
        for (auto &constraint : constraints) {
            if (!constraint.apply(queue)) {
                violated = true;
                break;
            }
        }
        if (violated) {
            return std::make_pair(false, 0.0);
        }

        // Apply rules
        RuleList rules = this->get_rules(bin);
        double utility = 0.0;
        for (auto &rule : rules) {
            double adj = rule.apply(queue);
            utility += adj;
        }

        return std::make_pair(true, utility);
    }


    LogicalConstant::LogicalConstant(bool value) :
        _value(value)
    {

    }


    bool LogicalConstant::get_value(
            AsdpAssignments assignments,
            AsdpList asdps
        ) {
        return this->_value;
    }


    ConstExpression::ConstExpression(double value) :
        _value(DpMetadataValue(value))
    {

    }

    DpMetadataValue ConstExpression::get_value(
            AsdpAssignments assignments,
            AsdpList asdps
        ) {
        return this->_value;
    }


    LogicalNot::LogicalNot(BoolValueExpression *expr) :
        _expr(expr)
    {

    }


    bool LogicalNot::get_value(
            AsdpAssignments assignments,
            AsdpList asdps
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
            AsdpAssignments assignments,
            AsdpList asdps
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
            AsdpAssignments assignments,
            AsdpList asdps
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
            AsdpAssignments assignments,
            AsdpList asdps
        ) {
        return this->_value;
    }


    MinusExpression::MinusExpression(ValueExpression *expr) :
        _expr(expr)
    {

    }

    DpMetadataValue MinusExpression::get_value(
            AsdpAssignments assignments,
            AsdpList asdps
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
            AsdpAssignments assignments,
            AsdpList asdps
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
            AsdpAssignments assignments,
            AsdpList asdps
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
            AsdpAssignments assignments,
            AsdpList asdps
        ) {
        for (auto asdp : asdps) {

            // Assign asdp to variable
            AsdpAssignments new_assignments(assignments);
            new_assignments[this->_var] = asdp;

            // Evaluate expression and return if true
            bool evaluation = this->_expr->get_value(new_assignments, asdps);
            if (evaluation) { return true; }

        }
        return false;
    }


    Status _get_argument_obj(
        nlohmann::json *result, nlohmann::json &j_obj, std::string arg
    ) {
        if (!j_obj.is_object()) {
            return FAILURE;
        }

        // TODO: Check for key
        auto j_contents = j_obj["__contents__"];
        if (!j_contents.is_object()) {
            return FAILURE;
        }

        // TODO: Check for key
        *result = j_contents[arg];
        return SUCCESS;
    }


    std::string _get_obj_type(nlohmann::json &j_obj) {
        if (!j_obj.is_object()) {
            // TODO: log invalid object
            return "";
        }
        auto j_type = j_obj["__type__"];
        if (!j_type.is_string()) {
            // TODO: log invalid type
            return "";
        }
        return j_type.get<std::string>();
    }

    template<> std::vector<std::string> _parse_argument(
        nlohmann::json &j_obj, std::string arg,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        std::vector<std::string> bad = {};
        nlohmann::json j_arg;
        Status status = _get_argument_obj(&j_arg, j_obj, arg);
        if (status != SUCCESS) { return bad; }

        if (j_arg.is_array()) {
            std::vector<std::string> result;
            for (auto& j_v : j_arg) {
                if (j_v.is_string()) {
                    result.push_back(j_v.get<std::string>());
                }
            }
            return result;
        }

        return bad;
    }


    template<> std::string _parse_argument(
        nlohmann::json &j_obj, std::string arg,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        std::string bad = "";
        nlohmann::json j_arg;
        Status status = _get_argument_obj(&j_arg, j_obj, arg);
        if (status != SUCCESS) { return bad; }

        if (j_arg.is_string()) {
            return j_arg.get<std::string>();
        }

        return bad;
    }


    template<> int _parse_argument(
        nlohmann::json &j_obj, std::string arg,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        int bad = -1;
        nlohmann::json j_arg;
        Status status = _get_argument_obj(&j_arg, j_obj, arg);
        if (status != SUCCESS) { return bad; }

        if (j_arg.is_number_integer()) {
            return j_arg.get<int>();
        } else {
            // TODO: log error
            return bad;
        }

    }


    template<> double _parse_argument(
        nlohmann::json &j_obj, std::string arg,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        double bad = 0.0;
        nlohmann::json j_arg;
        Status status = _get_argument_obj(&j_arg, j_obj, arg);
        if (status != SUCCESS) { return bad; }

        if (j_arg.is_number()) {
            return j_arg.get<double>();
        } else {
            // TODO: log error
            return bad;
        }

    }


    template<> bool _parse_argument(
        nlohmann::json &j_obj, std::string arg,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        bool bad = false;
        nlohmann::json j_arg;
        Status status = _get_argument_obj(&j_arg, j_obj, arg);
        if (status != SUCCESS) { return bad; }

        if (j_arg.is_boolean()) {
            return j_arg.get<bool>();
        } else {
            // TODO: log error
            return bad;
        }

    }


    template<> ValueExpression* _parse_argument(
        nlohmann::json &j_obj, std::string arg,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        ValueExpression* bad = nullptr;
        nlohmann::json j_arg;
        Status status = _get_argument_obj(&j_arg, j_obj, arg);
        if (status != SUCCESS) { return bad; }

        std::string type = _get_obj_type(j_arg);

        std::shared_ptr<ValueExpression> ptr;

        if (type == "ConstExpression") {
            double value = _parse_argument<double>(j_arg, "value", exprs);
            ptr = std::make_shared<ConstExpression>(
                ConstExpression(value)
            );

        } else if (type == "StringConstant") {
            std::string value = _parse_argument<std::string>(
                j_arg, "value", exprs
            );
            ptr = std::make_shared<StringConstant>(
                StringConstant(value)
            );

        } else if (type == "MinusExpression") {
            ValueExpression *expr = _parse_argument<ValueExpression*>(
                j_arg, "expression", exprs
            );
            if (!expr) { return bad; }
            ptr = std::make_shared<MinusExpression>(
                MinusExpression(expr)
            );

        } else if (type == "BinaryExpression") {
            std::string op = _parse_argument<std::string>(
                j_arg, "operator", exprs
            );
            ValueExpression *left_expr = _parse_argument<ValueExpression*>(
                j_arg, "left_expression", exprs
            );
            if (!left_expr) { return bad; }
            ValueExpression *right_expr = _parse_argument<ValueExpression*>(
                j_arg, "right_expression", exprs
            );
            if (!right_expr) { return bad; }
            ptr = std::make_shared<BinaryExpression>(
                BinaryExpression(op, left_expr, right_expr)
            );

        } else if (type == "Field") {
            std::string var_name = _parse_argument<std::string>(
                j_arg, "variable_name", exprs
            );
            std::string field_name = _parse_argument<std::string>(
                j_arg, "field_name", exprs
            );
            ptr = std::make_shared<Field>(
                Field(var_name, field_name)
            );

        } else {
            return bad;
        }


        exprs.push_back(ptr);
        return ptr.get();
    }


    template<> BoolValueExpression* _parse_argument(
        nlohmann::json &j_obj, std::string arg,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        BoolValueExpression* bad = nullptr;
        nlohmann::json j_arg;
        Status status = _get_argument_obj(&j_arg, j_obj, arg);
        if (status != SUCCESS) { return bad; }

        std::string type = _get_obj_type(j_arg);

        std::shared_ptr<BoolValueExpression> ptr;

        if (type == "LogicalConstant") {
            bool value = _parse_argument<bool>(j_arg, "value", exprs);
            ptr = std::make_shared<LogicalConstant>(
                LogicalConstant(value)
            );

        } else if (type == "LogicalNot") {
            BoolValueExpression *expr = _parse_argument<BoolValueExpression*>(
                j_arg, "expression", exprs
            );
            if (!expr) { return bad; }
            ptr = std::make_shared<LogicalNot>(
                LogicalNot(expr)
            );

        } else if (type == "BinaryLogicalExpression") {
            std::string op = _parse_argument<std::string>(
                j_arg, "operator", exprs
            );
            BoolValueExpression *left_expr = _parse_argument<BoolValueExpression*>(
                j_arg, "left_expression", exprs
            );
            if (!left_expr) { return bad; }
            BoolValueExpression *right_expr = _parse_argument<BoolValueExpression*>(
                j_arg, "right_expression", exprs
            );
            if (!right_expr) { return bad; }
            ptr = std::make_shared<BinaryLogicalExpression>(
                BinaryLogicalExpression(op, left_expr, right_expr)
            );

        } else if (type == "ComparatorExpression") {
            std::string comp = _parse_argument<std::string>(
                j_arg, "comparator", exprs
            );
            ValueExpression *left_expr = _parse_argument<ValueExpression*>(
                j_arg, "left_expression", exprs
            );
            if (!left_expr) { return bad; }
            ValueExpression *right_expr = _parse_argument<ValueExpression*>(
                j_arg, "right_expression", exprs
            );
            if (!right_expr) { return bad; }
            ptr = std::make_shared<ComparatorExpression>(
                ComparatorExpression(comp, left_expr, right_expr)
            );

        } else if (type == "ExistentialExpression") {
            std::string variable = _parse_argument<std::string>(
                j_arg, "variable", exprs
            );
            BoolValueExpression *expr = _parse_argument<BoolValueExpression*>(
                j_arg, "expression", exprs
            );
            if (!expr) { return bad; }
            ptr = std::make_shared<ExistentialExpression>(
                ExistentialExpression(variable, expr)
            );

        } else {
            return bad;
        }

        exprs.push_back(ptr);
        return ptr.get();
    }


    Constraint _parse_constraint(
        nlohmann::json &j_constraint,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        Constraint bad = Constraint({}, nullptr, nullptr, 0.0);

        std::string type = _get_obj_type(j_constraint);
        if (type != "Constraint") {
            // TODO: log warning
            return bad;
        }

        auto variables = _parse_argument<std::vector<std::string>>(
            j_constraint, "variables", exprs
        );
        auto application = _parse_argument<BoolValueExpression*>(
            j_constraint, "application", exprs
        );
        auto sum_field = _parse_argument<ValueExpression*>(
            j_constraint, "sum_field", exprs
        );
        auto constraint_value = _parse_argument<double>(
            j_constraint, "constraint_value", exprs
        );

        if (!application) {
            // TODO: bad application expression
            return bad;
        }

        return Constraint(
            variables, application, sum_field, constraint_value
        );

    }


    Rule _parse_rule(
        nlohmann::json &j_rule,
        std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        Rule bad = Rule({}, nullptr, nullptr, 0);

        std::string type = _get_obj_type(j_rule);
        if (type != "Rule") {
            // TODO: log warning
            return bad;
        }

        auto variables = _parse_argument<std::vector<std::string>>(
            j_rule, "variables", exprs
        );
        auto application = _parse_argument<BoolValueExpression*>(
            j_rule, "application", exprs
        );
        auto adjustment = _parse_argument<ValueExpression*>(
            j_rule, "adjustment", exprs
        );
        auto max_applications = _parse_argument<int>(
            j_rule, "max_applications", exprs
        );

        if (!application) {
            // TODO: bad application expression
            return bad;
        }
        if (!adjustment) {
            // TODO: bad adjustment expression
            return bad;
        }

        return Rule(
            variables, application, adjustment, max_applications
        );

    }


    std::pair<RuleList, ConstraintList>
    _parse_bin(
            nlohmann::json &j_bin,
            std::vector<std::shared_ptr<RuleExpression>> &exprs
    ) {
        RuleList rules;
        ConstraintList constraints;

        // TODO: use zero variables to indicate invalid rules, and exclude
        // these from lists?

        // TODO: check for key
        auto j_rules = j_bin["rules"];
        if (j_rules.is_array()) {
            for (auto j_rule : j_rules) {
                rules.push_back(_parse_rule(j_rule, exprs));
            }
        }

        // TODO: check for key
        auto j_constraints = j_bin["constraints"];
        if (j_constraints.is_array()) {
            for (auto j_constraint : j_constraints) {
                constraints.push_back(_parse_constraint(j_constraint, exprs));
            }
        }

        return std::make_pair(rules, constraints);
    }


    RuleSet parse_rule_config(std::string config_file) {

        std::ifstream file_input(config_file);
        auto j = nlohmann::json::parse(file_input);

        if (!j.is_object()) {
            // TODO: log warning
            return RuleSet();
        }

        std::map<int, RuleList> rule_map;
        std::map<int, ConstraintList> constraint_map;
        RuleList default_rules;
        ConstraintList default_constraints;
        std::vector<std::shared_ptr<RuleExpression>> exprs;

        for (auto& el : j.items()) {
            std::string key = el.key();
            auto val = el.value();

            auto bin_rules = _parse_bin(val, exprs);

            if (key == "default") {
                default_rules = bin_rules.first;
                default_constraints = bin_rules.second;
            } else {
                try {
                    int ikey = std::stoi(key);
                    rule_map[ikey] = bin_rules.first;
                    constraint_map[ikey] = bin_rules.second;
                } catch ( std::invalid_argument & e ) {
                    // TODO: log bad alpha key type
                    continue;
                }
            }
        }

        return RuleSet(
            rule_map, constraint_map,
            default_rules, default_constraints,
            exprs
        );
    }


};
