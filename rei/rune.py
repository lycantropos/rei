from typing import Optional

from .consts import MAX_RUNE_VALUE
from .mask import (Bits,
                   Bound,
                   Mask)


class Rune:
    __slots__ = '_value',

    def __new__(cls, characters: bytes) -> 'Rune':
        value = characters_to_rune_value(characters)
        self = super().__new__(cls)
        self._value = value
        return self

    def __lt__(self, other: 'Rune') -> bool:
        return (self._value < other._value
                if isinstance(other, Rune)
                else NotImplemented)

    @property
    def components(self) -> bytes:
        return rune_value_to_characters(self._value)


def characters_to_rune_value(characters: bytes) -> Optional[int]:
    first_character = characters[0]
    if first_character < Bound.X:
        return first_character
    second_character = characters[1] ^ Bound.X
    if second_character & Mask.X_TEST:
        return None
    if first_character < Bound.THIRD:
        if first_character < Bound.SECOND:
            return None
        value = ((first_character << Bits.X) | second_character) & Mask.SECOND
        if value <= Mask.FIRST:
            return None
        return value
    third_character = characters[2] ^ Bound.X
    if third_character & Mask.X_TEST:
        return None
    if first_character < Bound.FOURTH:
        value = ((((first_character << Bits.X) | second_character) << Bits.X)
                 | third_character) & Mask.THIRD
        if value <= Mask.SECOND:
            return None
        return value
    fourth_character = characters[3] ^ Bound.X
    if fourth_character & Mask.X_TEST:
        return None
    if first_character < Bound.FIFTH:
        value = (((((((first_character << Bits.X) | second_character)
                     << Bits.X)
                    | third_character) << Bits.X) | fourth_character)
                 & Mask.FOURTH)
        if value <= Mask.THIRD:
            return None
        return value
    return None


def rune_value_to_characters(value: int) -> bytes:
    if value > MAX_RUNE_VALUE:
        raise ValueError('Rune value is out of range.')
    elif value <= Mask.FIRST:
        return bytes([value])
    elif value <= Mask.SECOND:
        return bytes([Bound.SECOND | (value >> 1 * Bits.X),
                      Bound.X | (value & Mask.X)])
    elif value <= Mask.THIRD:
        return bytes([Bound.THIRD | (value >> 2 * Bits.X),
                      Bound.X | ((value >> 1 * Bits.X) & Mask.X),
                      Bound.X | (value & Mask.X)])
    else:
        return bytes([Bound.FOURTH | (value >> 3 * Bits.X),
                      Bound.X | ((value >> 2 * Bits.X) & Mask.X),
                      Bound.X | ((value >> 1 * Bits.X) & Mask.X),
                      Bound.X | (value & Mask.X)])
