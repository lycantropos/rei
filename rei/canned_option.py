import enum


class CannedOption(enum.IntEnum):
    DEFAULT = 0
    LATIN_1 = enum.auto()
    POSIX = enum.auto()
    QUIET = enum.auto()
