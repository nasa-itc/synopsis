from ply.lex import lex
from ply.yacc import yacc

from rule_ast import *

reserved = (
    'RULE',
    'APPLIES',
    'ADJUST',
    'UTILITY',
    'MAXIMUM',
    'APPLICATIONS',
    'AND',
    'OR',
    'NOT',
    'EXISTS',
    'COUNT',
    'SUM',
    'LESS',
    'THAN',
)

tokens = reserved + (
    # Literals and constants
    'ID', 'ICONST', 'FCONST', 'SCONST',

    # Comparators
    'LT', 'LE', 'GT', 'GE', 'EQ', 'NE',

    # Operators
    'PLUS', 'MINUS', 'TIMES',

    # Delimiters
    'LPAREN', 'RPAREN',
    'COMMA', 'PERIOD', 'SEMI', 'COLON',
)

# Comparators
t_LT = r'<'
t_GT = r'>'
t_LE = r'<='
t_GE = r'>='
t_EQ = r'=='
t_NE = r'!='

# Operators
t_PLUS = r'\+'
t_MINUS = r'-'
t_TIMES = r'\*'

# Delimeters
t_LPAREN = r'\('
t_RPAREN = r'\)'
t_COMMA = r','
t_PERIOD = r'\.'
t_SEMI = r';'
t_COLON = r':'

RESERVED_MAP = { r.lower() : r for r in reserved }

def t_ID(t):
    r'[A-Za-z_][\w_]*'
    t.type = RESERVED_MAP.get(t.value.lower(), "ID")
    return t

# Integer literal
t_ICONST = r'\d+?'

# Floating literal
t_FCONST = r'((\d+)(\.\d+)(e(\+|-)?(\d+))? | (\d+)e(\+|-)?(\d+))'

# String literal
t_SCONST = r'\"([^\\\n]|(\\.))*?\"'


def t_NEWLINE(t):
    r'\n+'
    t.lexer.lineno += t.value.count("\n")


def t_comment(t):
    r'\#(.)*?\n'
    t.lexer.lineno += 1


def t_error(t):
    print("Illegal character %s" % repr(t.value[0]))
    t.lexer.skip(1)


# Ignore tabs and spaces
t_ignore = ' \t'


precedence = (
    ('left', 'PLUS', 'MINUS'),
    ('left', 'TIMES'),
    ('right', 'UMINUS'),
)


def p_rule_declaration(p):
    """
    rule_declaration : rule_declaration_onevar
                     | rule_declaration_twovar
    """
    p[0] = p[1]


def p_rule_declaration_onevar(p):
    """
    rule_declaration_onevar : RULE decl_onevar COLON rule_body SEMI
    """
    variable_list = p[2]
    rule_kwargs = p[4]
    p[0] = OneVarRule(*variable_list, **rule_kwargs)


def p_rule_declaration_twovar(p):
    """
    rule_declaration_twovar : RULE decl_twovar COLON rule_body SEMI
    """
    variable_list = p[2]
    rule_kwargs = p[4]
    p[0] = TwoVarRule(*variable_list, **rule_kwargs)


def p_decl_onevar(p):
    """
    decl_onevar : LPAREN ID RPAREN
    """
    p[0] = (p[2],)


def p_decl_twovar(p):
    """
    decl_twovar : LPAREN ID COMMA ID RPAREN
    """
    p[0] = (p[2], p[4])


def p_rule_body(p):
    """
    rule_body : applies_clause adjust_clause
              | applies_clause adjust_clause max_app_clause
    """
    if len(p) == 4:
        max_applications = p[3]
    else:
        max_applications = None

    p[0] = {
        'application': p[1],
        'adjustment': p[2],
        'max_applications': max_applications,
    }


def p_applies_clause(p):
    """
    applies_clause : APPLIES
    """
    p[0] = p[1]


def p_adjust_clause(p):
    """
    adjust_clause : ADJUST UTILITY arithmetic_expression
    """
    p[0] = p[3]


def p_max_app_clause(p):
    """
    max_app_clause : MAXIMUM APPLICATIONS ICONST
    """
    p[0] = int(p[3])


def p_arithmetic_expression(p):
    """
    arithmetic_expression : field
    """
    p[0] = p[1]


def p_unary_arithmetic_expression(p):
    """
    arithmetic_expression : MINUS arithmetic_expression %prec UMINUS
    """

    # If operand is constant, negate directly
    if type(p[2]) == ConstExpression:
        p[0] = ConstExpression(-p[2].value)

    # Otherwise, represent negation
    else:
        p[0] = MinusExpression(p[2])


def p_paren_arithmetic_expression(p):
    """
    arithmetic_expression : LPAREN arithmetic_expression RPAREN
    """
    p[0] = p[2]


def p_binary_arithmetic_expression(p):
    """
    arithmetic_expression : arithmetic_expression TIMES arithmetic_expression
                          | arithmetic_expression PLUS arithmetic_expression
                          | arithmetic_expression MINUS arithmetic_expression
    """

    # If both expressions are constant, evaluate operation
    if (type(p[1]) == ConstExpression) and (type(p[3]) == ConstExpression):
        op = p[2]
        lval = p[1].value
        rval = p[3].value
        if op == '*':
            val = lval * rval
        elif op == '+':
            val = lval + rval
        elif op == '-':
            val = lval - rval
        p[0] = ConstExpression(val)

    # Otherwise, represent binary operation
    else:
        p[0] = BinaryExpression(p[2], p[1], p[3])


def p_const_arithmetic_expression(p):
    """
    arithmetic_expression : ICONST
                          | FCONST
    """
    p[0] = ConstExpression(p[1])


def p_field(p):
    """
    field : ID PERIOD ID
    """
    p[0] = Field(p[1], p[3], p.slice[1].lineno)


SYNOPSIS_LEXER = lex()
SYNOPSIS_PARSER = yacc()


EXAMPLE1 = """\
RULE(x):
APPLIES
ADJUST UTILITY -0.5 * x.sue
MAXIMUM APPLICATIONS 1;
"""
EXAMPLE2 = """\
RULE(x, y):
APPLIES
ADJUST UTILITY (x.sue + 1.0);
"""

print(SYNOPSIS_PARSER.parse(EXAMPLE1))
print(SYNOPSIS_PARSER.parse(EXAMPLE2))
