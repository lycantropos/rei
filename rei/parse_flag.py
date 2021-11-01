import enum


class ParseFlags(enum.IntFlag):
    NO_PARSE_FLAGS = 0
    FOLD_CASE = 1 << 0
    LITERAL = 1 << 1
    CLASS_NL = 1 << 2
    DOT_NL = 1 << 3
    MATCH_NL = CLASS_NL | DOT_NL
    ONE_LINE = 1 << 4
    LATIN1 = 1 << 5
    NON_GREEDY = 1 << 6
    PERL_CLASSES = 1 << 7
    PERL_B = 1 << 8
    PERL_X = 1 << 9
    UNICODE_GROUPS = 1 << 10
    NEVER_NL = 1 << 11
    NEVER_CAPTURE = 1 << 12
    LIKE_PERL = (CLASS_NL | ONE_LINE | PERL_CLASSES | PERL_B | PERL_X
                 | UNICODE_GROUPS)
    WAS_DOLLAR = 1 << 13
    ALL_PARSE_FLAGS = (1 << 14) - 1
