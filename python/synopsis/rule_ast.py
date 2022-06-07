"""
Abstract Syntax Tree representation for SYNOPSIS rules
"""

class Rule:

    pass


class OneVarRule(Rule):


    def __init__(self, variable1, application, adjustment, max_applications):
        self.variable1 = variable1

        variables = (variable1,)

        application.validate(variables)
        self.application = application

        adjustment.validate(variables)
        self.adjustment = adjustment


    def __repr__(self):
        app_repr = repr(self.application)
        adj_repr = repr(self.adjustment)
        return f'RULE({self.variable1}, {app_repr}, {adj_repr})'


class TwoVarRule(Rule):


    def __init__(self, variable1, variable2, application, adjustment, max_applications):
        self.variable1 = variable1
        self.variable2 = variable2

        variables = (variable1, variable2)

        application.validate(variables)
        self.application = application

        adjustment.validate(variables)
        self.adjustment = adjustment


    def __repr__(self):
        app_repr = repr(self.application)
        adj_repr = repr(self.adjustment)
        return f'RULE({self.variable1}, {self.variable2}, {app_repr}, {adj_repr})'


class ValueExpression:

    def validate(self, scope):
        pass


class LogicalConstant(ValueExpression):


    def __init__(self, value):
        vtype = type(value)

        if vtype == str:
            if value.lower() == 'true':
                self.value = True
            elif value.lower() == 'false':
                self.value = False
            else:
                raise ValueError(f'Unexpected logical constant value "{value}"')

        elif vtype == bool:
            self.value = value

        else:
            raise ValueError(f'Unexpected value type "{vtype}"')


    def get_value(self):
        return self.value


    def __str__(self):
        return f'{self.value}'


    def __repr__(self):
        return f'LogicalConstant({self.value})'


class StringConstant(ValueExpression):


    def __init__(self, value):
        self.value = value


    def get_value(self):
        return self.value


    def __str__(self):
        return f'"{self.value}"'


    def __repr__(self):
        return f'StringConstant({self.value})'


class ComparatorExpression(ValueExpression):


    def __init__(self, comparator, left_expression, right_expression):
        self.comparator = comparator
        self.left_expression = left_expression
        self.right_expression = right_expression


    @staticmethod
    def evaluate(comparator, left_value, right_value):
        if comparator == '<':
            return (left_value < right_value)
        elif comparator == '<=':
            return (left_value <= right_value)
        elif comparator == '>':
            return (left_value > right_value)
        elif comparator == '>=':
            return (left_value >= right_value)
        elif comparator == '==':
            return (left_value == right_value)
        elif comparator == '!=':
            return (left_value != right_value)
        else:
            raise ValueError(f'Unknown comparator "{comparator}"')


    def get_value(self):
        return ComparatorExpression.evaluate(
            self.comparator,
            self.left_expression.get_value(),
            self.right_expression.get_value(),
        )


    def validate(self, scope):
        self.left_expression.validate(scope)
        self.right_expression.validate(scope)


    def __str__(self):
        return f'({self.left_expression} {self.comparator} {self.right_expression})'


    def __repr__(self):
        lrepr = repr(self.left_expression)
        rrepr = repr(self.right_expression)
        return f'BinaryExpression({self.comparator}, {lrepr}, {rrepr})'


class ArithmeticExpression(ValueExpression): pass


class ConstExpression(ArithmeticExpression):


    def __init__(self, value):
        self.value = float(value)


    def get_value(self):
        return self.value


    def __str__(self):
        return f'{self.value}'


    def __repr__(self):
        return f'ConstExpression({self.value})'


class BinaryExpression(ArithmeticExpression):


    def __init__(self, operator, left_expression, right_expression):
        self.operator = operator
        self.left_expression = left_expression
        self.right_expression = right_expression


    def validate(self, scope):
        self.left_expression.validate(scope)
        self.right_expression.validate(scope)


    @staticmethod
    def evaluate(operator, left_value, right_value):
        if operator == '*':
            return (left_value * right_value)
        elif operator == '+':
            return (left_value + right_value)
        elif operator == '-':
            return (left_value - right_value)
        else:
            raise ValueError(f'Unknown operator "{operator}"')


    def get_value(self):
        return BinaryExpression.evaluate(
            self.operator,
            self.left_expression.get_value(),
            self.right_expression.get_value(),
        )


    def __str__(self):
        return f'({self.left_expression} {self.operator} {self.right_expression})'


    def __repr__(self):
        lrepr = repr(self.left_expression)
        rrepr = repr(self.right_expression)
        return f'BinaryExpression({self.operator}, {lrepr}, {rrepr})'


class MinusExpression(ArithmeticExpression):


    def __init__(self, expression):
        self.expression = expression


    def validate(self, scope):
        self.expression.validate(scope)


    def get_value(self):
        return -self.expression.get_value()


    def __str__(self):
        return f'-{self.expression}'


    def __repr__(self):
        return f'MinusExpression({repr(self.expression)})'


class Field(ArithmeticExpression):


    def __init__(self, variable_name, field_name, lineno):
        self.variable_name = variable_name
        self.field_name = field_name
        self.lineno = lineno


    def validate(self, scope):
        if self.variable_name not in scope:
            raise ValueError(f'Variable "{self.variable_name}" not in scope (line {self.lineno})')


    def __str__(self):
        return f'{self.variable_name}.{self.field_name}'


    def __repr__(self):
        return f'Field({self.variable_name}, {self.field_name}, {self.lineno})'
