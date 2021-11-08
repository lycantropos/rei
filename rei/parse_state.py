from reprit.base import generate_repr

from .parse_flag import ParseFlag
from .status import Status


class ParseState:
    __slots__ = '_flag', '_pattern', '_status'

    def __init__(self, pattern: str, flag: ParseFlag) -> None:
        self._flag = flag
        self._pattern = pattern
        self._status = Status()

    __repr__ = generate_repr(__init__)

    @property
    def flag(self) -> ParseFlag:
        return self._flag

    @property
    def pattern(self) -> str:
        return self._pattern

    @property
    def status(self) -> Status:
        return self._status
