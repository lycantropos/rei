from typing import Iterator

from dendroid import red_black
from reprit.base import generate_repr

from .consts import MAX_RUNE_VALUE
from .rune import Rune
from .rune_range import RuneRange


class CharClassBuilder:
    __slots__ = '_lower', '_ranges', '_runes_count', '_upper'

    def __new__(cls, *_ranges: RuneRange) -> 'CharClassBuilder':
        self = super().__new__(cls)
        self._lower = self._upper = 0
        self._runes_count = 0
        self._ranges = red_black.set_()
        for range_ in _ranges:
            self.add_range(range_)
        return self

    __repr__ = generate_repr(__new__)

    def __eq__(self, other: 'CharClassBuilder') -> bool:
        return (self._ranges == other._ranges
                if isinstance(other, CharClassBuilder)
                else NotImplemented)

    def __iter__(self) -> Iterator[RuneRange]:
        return iter(self._ranges)

    def __len__(self) -> int:
        return len(self._ranges)

    @property
    def runes_count(self) -> int:
        return self._runes_count

    def add_range(self, range_: RuneRange) -> bool:
        low, high = range_.low, range_.high
        if high < low:
            return False
        if low <= Rune(b'z') and high >= Rune(b'A'):
            lo1 = max(low, Rune(b'A')).value
            hi1 = min(high, Rune(b'Z')).value
            if lo1 <= hi1:
                self._upper |= (((1 << (hi1 - lo1 + 1)) - 1)
                                << (lo1 - Rune(b'A').value))
            lo1 = max(low, Rune(b'a')).value
            hi1 = min(high, Rune(b'z')).value
            if lo1 <= hi1:
                self._lower |= (((1 << (hi1 - lo1 + 1)) - 1)
                                << (lo1 - Rune(b'a').value))
        node = self._ranges.tree.find(RuneRange(low))
        if (node is not red_black.NIL
                and node.value.low <= low
                and high <= node.value.high):
            return False
        if low.value > 0:
            node = self._ranges.tree.find(
                    RuneRange(Rune.from_value(low.value - 1)))
            if node is not red_black.NIL:
                low = node.value.low
                if node.value.high > high:
                    high = node.value.high
                self._runes_count -= (node.value.high.value
                                      - node.value.low.value + 1)
                self._ranges.tree.remove(node)
        if high.value < MAX_RUNE_VALUE:
            node = self._ranges.tree.find(
                    RuneRange(Rune.from_value(high.value + 1)))
            if node is not red_black.NIL:
                high = node.value.high
                self._runes_count -= (node.value.high.value
                                      - node.value.low.value + 1)
                self._ranges.tree.remove(node)
        while True:
            node = self._ranges.tree.find(RuneRange(low, high))
            if node is red_black.NIL:
                break
            self._runes_count -= (node.value.high.value - node.value.low.value
                                  + 1)
            self._ranges.tree.remove(node)
        self._runes_count += high.value - low.value + 1
        self._ranges.add(RuneRange(low, high))
        return True
