#ifndef JPL_SYNOPSIS_RuleAST
#define JPL_SYNOPSIS_RuleAST

#include <vector>
#include <map>

#include "synopsis_types.hpp"
#include "DpDbMsg.hpp"


namespace Synopsis {


    class BoolValueExpression {

        public:

            virtual ~BoolValueExpression() = default;

            virtual bool get_value(
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
                std::vector<std::map<std::string, DpMetadataValue>> asdps
            ) = 0;

    };


    class ValueExpression {

        public:

            virtual ~ValueExpression() = default;

            virtual DpMetadataValue get_value(
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
                std::vector<std::map<std::string, DpMetadataValue>> asdps
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
            double apply(std::vector<std::map<std::string, DpMetadataValue>> asdps);

        private:

            std::vector<std::string> _variables;
            BoolValueExpression *_application_expression;
            ValueExpression *_adjustment_expression;
            int _max_applications;


    };


    class LogicalConstant : public BoolValueExpression {


        public:
            LogicalConstant(bool value);
            virtual ~LogicalConstant() = default;

            bool get_value(
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
                std::vector<std::map<std::string, DpMetadataValue>> asdps
            );

        private:
            bool _value;

    };


    class ConstExpression : public ValueExpression {


        public:
            ConstExpression(double value);
            virtual ~ConstExpression() = default;

            DpMetadataValue get_value(
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
                std::vector<std::map<std::string, DpMetadataValue>> asdps
            );

        private:
            DpMetadataValue _value;

    };


    class LogicalNot : public BoolValueExpression {


        public:
            LogicalNot(BoolValueExpression *expr);
            virtual ~LogicalNot() = default;

            bool get_value(
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
                std::vector<std::map<std::string, DpMetadataValue>> asdps
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
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
                std::vector<std::map<std::string, DpMetadataValue>> asdps
            );

        private:
            std::string _op;
            BoolValueExpression *_left_expr;
            BoolValueExpression *_right_expr;

    };


// Remaining types:
// [x] Rule
// [ ] Constraint
// [ ] ExistentialExpression   # bool
// [x] LogicalConstant         # bool  ()
// [x] BinaryLogicalExpression # bool  (bool, bool)
// [ ] ComparatorExpression    # bool  (VALUE VALUE)
// [ ] StringConstant          # VALUE ()
// [x] ConstExpression         # VALUE ()
// [ ] BinaryExpression        # VALUE (VALUE, VALUE)
// [ ] MinusExpression         # VALUE (VALUE)
// [ ] Field                   # VALUE


};


#endif
