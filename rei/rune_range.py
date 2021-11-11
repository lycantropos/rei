from reprit.base import generate_repr

from .rune import Rune


class RuneRange:
    __slots__ = '_low', '_high'

    def __init__(self, low: Rune, high: Rune) -> None:
        self._low = low
        self._high = high

    __repr__ = generate_repr(__init__)

    @property
    def high(self) -> Rune:
        return self._high

    @property
    def low(self) -> Rune:
        return self._low

    def __eq__(self, other: 'RuneRange') -> bool:
        return (self.low == other.low and self.high == other.high
                if isinstance(other, RuneRange)
                else NotImplemented)

    def __lt__(self, other: 'RuneRange') -> bool:
        return (self.high < other.low
                if isinstance(other, RuneRange)
                else NotImplemented)
