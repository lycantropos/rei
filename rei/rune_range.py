from typing import Optional

from reprit.base import generate_repr

from .rune import Rune


class RuneRange:
    __slots__ = '_low', '_high'

    def __new__(cls, low: Rune, high: Optional[Rune] = None) -> 'RuneRange':
        self = super().__new__(cls)
        self._low = low
        self._high = low if high is None else high
        return self

    __repr__ = generate_repr(__new__)

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
