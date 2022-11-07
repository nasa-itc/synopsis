#ifndef JPL_SYNOPSIS_RuleAST
#define JPL_SYNOPSIS_RuleAST

#include <vector>
#include <map>
#include <utility>

#include "synopsis_types.hpp"
#include "DpDbMsg.hpp"


namespace Synopsis {


    class RuleExpression {

        public:

            virtual ~RuleExpression() = default;

    };


    class BoolValueExpression : public RuleExpression {

        public:

            virtual ~BoolValueExpression() = default;

            virtual bool get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            ) = 0;

    };


    class ValueExpression : public RuleExpression {

        public:

            virtual ~ValueExpression() = default;

            virtual DpMetadataValue get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            ) = 0;

    };


    class Rule {

        public:

            /*
             * Default Constructor
             */
            Rule(
                std::vector<std::string> variables,
                BoolValueExpression *application_expression,
                ValueExpression *adjustment_expression,
                int max_applications
            );
            ~Rule() = default;


            /*
             * Returns total SUE adjustment due to application of rule
             */
            double apply(AsdpList asdps);

        private:

            std::vector<std::string> _variables;
            BoolValueExpression *_application_expression;
            ValueExpression *_adjustment_expression;
            int _max_applications;


    };


    class Constraint {

        public:

            /*
             * Default Constructor
             */
            Constraint(
                std::vector<std::string> variables,
                BoolValueExpression *application_expression,
                ValueExpression *sum_field,
                double constraint_value
            );
            ~Constraint() = default;


            /*
             * Returns whether the constraint is satisfied for the given ASDPs
             */
            bool apply(AsdpList asdps);

        private:

            std::vector<std::string> _variables;
            BoolValueExpression *_application_expression;
            ValueExpression *_sum_field;
            double _constraint_value;

    };


    using RuleList = std::vector<Rule>;
    using ConstraintList = std::vector<Constraint>;


    class RuleSet {

        public:
            RuleSet();
            RuleSet(
                std::map<int, RuleList> rule_map,
                std::map<int, ConstraintList> constraint_map,
                RuleList default_rules,
                ConstraintList default_constraints
            );
            RuleSet(
                std::map<int, RuleList> rule_map,
                std::map<int, ConstraintList> constraint_map,
                RuleList default_rules,
                ConstraintList default_constraints,
                std::vector<std::shared_ptr<RuleExpression>> expressions
            );
            ~RuleSet() = default;

            RuleList get_rules(int bin);
            ConstraintList get_constraints(int bin);

            /*
             * Returns a pair of values; the first entry indicates whether all
             * constraints were satisfied, and if true, the second entry
             * specifies the total utility adjustment to apply.
             */
            std::pair<bool, double> apply(
                int bin,
                AsdpList queue
            );


        private:

            std::map<int, RuleList> _rule_map;
            std::map<int, ConstraintList> _constraint_map;
            RuleList _default_rules;
            ConstraintList _default_constraints;
            std::vector<std::shared_ptr<RuleExpression>> _expressions;

    };


    class LogicalConstant : public BoolValueExpression {


        public:
            LogicalConstant(bool value);
            virtual ~LogicalConstant() = default;

            bool get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            bool _value;

    };


    class ConstExpression : public ValueExpression {


        public:
            ConstExpression(double value);
            virtual ~ConstExpression() = default;

            DpMetadataValue get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            DpMetadataValue _value;

    };


    class LogicalNot : public BoolValueExpression {


        public:
            LogicalNot(BoolValueExpression *expr);
            virtual ~LogicalNot() = default;

            bool get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            BoolValueExpression *_expr;

    };


    class BinaryLogicalExpression : public BoolValueExpression {


        public:
            BinaryLogicalExpression(
                std::string op,
                BoolValueExpression *left_expr,
                BoolValueExpression *right_expr
            );
            virtual ~BinaryLogicalExpression() = default;

            bool get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            std::string _op;
            BoolValueExpression *_left_expr;
            BoolValueExpression *_right_expr;

    };


    class ComparatorExpression : public BoolValueExpression {


        public:
            ComparatorExpression(
                std::string comp,
                ValueExpression *left_expr,
                ValueExpression *right_expr
            );
            virtual ~ComparatorExpression() = default;

            bool get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            std::string _comp;
            ValueExpression *_left_expr;
            ValueExpression *_right_expr;

    };


    class StringConstant : public ValueExpression {


        public:
            StringConstant(
                std::string value
            );
            virtual ~StringConstant() = default;

            DpMetadataValue get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            DpMetadataValue _value;

    };


    class MinusExpression : public ValueExpression {


        public:
            MinusExpression(
                ValueExpression *expr
            );
            virtual ~MinusExpression() = default;

            DpMetadataValue get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            ValueExpression *_expr;

    };


    class BinaryExpression : public ValueExpression {


        public:
            BinaryExpression(
                std::string op,
                ValueExpression *left_expr,
                ValueExpression *right_expr
            );
            virtual ~BinaryExpression() = default;

            DpMetadataValue get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            std::string _op;
            ValueExpression *_left_expr;
            ValueExpression *_right_expr;

    };


    class Field : public ValueExpression {


        public:
            Field(
                std::string var_name,
                std::string field_name
            );
            virtual ~Field() = default;

            DpMetadataValue get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            std::string _var_name;
            std::string _field_name;

    };


    class ExistentialExpression : public BoolValueExpression {


        public:
            ExistentialExpression(
                std::string variable,
                BoolValueExpression *expr
            );
            virtual ~ExistentialExpression() = default;

            bool get_value(
                AsdpAssignments assignments,
                AsdpList asdps
            );

        private:
            std::string _var;
            BoolValueExpression *_expr;

    };


    RuleSet parse_rule_config(std::string config_file);


};


#endif
