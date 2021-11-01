import enum


class StatusCode(enum.IntEnum):
    SUCCESS = 0
    INTERNAL_ERROR = enum.auto()
    BAD_ESCAPE = enum.auto()
    BAD_CHAR_CLASS = enum.auto()
    BAD_CHAR_RANGE = enum.auto()
    MISSING_BRACKET = enum.auto()
    MISSING_PAREN = enum.auto()
    UNEXPECTED_PAREN = enum.auto()
    TRAILING_BACKSLASH = enum.auto()
    REPEAT_ARGUMENT = enum.auto()
    REPEAT_SIZE = enum.auto()
    REPEAT_OP = enum.auto()
    BAD_PERL_OP = enum.auto()
    BAD_UTF8 = enum.auto()
    BAD_NAMED_CAPTURE = enum.auto()

    def __str__(self) -> str:
        return ['no error',
                'unexpected error',
                'invalid escape sequence',
                'invalid character class',
                'invalid character class range',
                'missing ]',
                'missing )',
                'unexpected )',
                'trailing \\',
                'no argument for repetition operator',
                'invalid repetition size',
                'bad repetition operator',
                'invalid perl operator',
                'invalid UTF-8',
                'invalid named capture group'][self.value]
