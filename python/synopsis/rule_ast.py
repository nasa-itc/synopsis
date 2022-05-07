"""
Abstract Syntax Tree representation for SYNOPSIS rules
"""

class Rule:

    pass


class OneVarRule(Rule):


    def __init__(self, variable1, application, adjustment, max_applications):
        self.variable1 = variable1

        adjustment.validate((variable1,))
        self.adjustment = adjustment


    def __repr__(self):
        adj_repr = repr(self.adjustment)
        return f'RULE({self.variable1}, {adj_repr})'


class TwoVarRule(Rule):


    def __init__(self, variable1, variable2, application, adjustment, max_applications):
        self.variable1 = variable1
        self.variable2 = variable2

        adjustment.validate((variable1, variable2))
        self.adjustment = adjustment


    def __repr__(self):
        adj_repr = repr(self.adjustment)
        return f'RULE({self.variable1}, {self.variable2}, {adj_repr})'


class ArithmeticExpression:


    def validate(self, scope):
        pass


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


    def get_value(self):
        left_value = self.left_expression.get_value()
        right_value = self.right_expression.get_value()
        if self.operator == '*':
            return (left_value * right_value)
        elif self.operator == '+':
            return (left_value + right_value)
        elif self.operator == '-':
            return (left_value - right_value)


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
