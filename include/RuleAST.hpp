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


    class NumericValueExpression {

        public:

            virtual ~NumericValueExpression() = default;

            virtual double get_value(
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
                NumericValueExpression *adjustment_expression,
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
            NumericValueExpression *_adjustment_expression;
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


    class ConstExpression : public NumericValueExpression {


        public:
            ConstExpression(double value);
            virtual ~ConstExpression() = default;

            double get_value(
                std::map<std::string, std::map<std::string, DpMetadataValue>> assignments,
                std::vector<std::map<std::string, DpMetadataValue>> asdps
            );

        private:
            double _value;

    };


};


#endif
