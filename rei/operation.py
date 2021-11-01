import enum


class Operation(enum.IntEnum):
    NO_MATCH = enum.auto()
    EMPTY_MATCH = enum.auto()
    LITERAL = enum.auto()
    LITERAL_STRING = enum.auto()
    CONCAT = enum.auto()
    ALTERNATE = enum.auto()
    STAR = enum.auto()
    PLUS = enum.auto()
    QUEST = enum.auto()
    REPEAT = enum.auto()
    CAPTURE = enum.auto()
    ANY_CHAR = enum.auto()
    ANY_BYTE = enum.auto()
    BEGIN_LINE = enum.auto()
    END_LINE = enum.auto()
    WORD_BOUNDARY = enum.auto()
    NO_WORD_BOUNDARY = enum.auto()
    BEGIN_TEXT = enum.auto()
    END_TEXT = enum.auto()
    CHAR_CLASS = enum.auto()
    HAVE_MATCH = enum.auto()
    MAX_OPERATION = HAVE_MATCH
