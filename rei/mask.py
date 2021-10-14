import enum


class Bits(enum.IntFlag):
    FIRST = 7
    X = 6
    SECOND = 5
    THIRD = 4
    FOURTH = 3
    FIFTH = 2


class Bound(enum.IntFlag):
    FIRST = ((1 << (Bits.FIRST + 1)) - 1) ^ 0xFF
    X = ((1 << (Bits.X + 1)) - 1) ^ 0xFF
    SECOND = ((1 << (Bits.SECOND + 1)) - 1) ^ 0xFF
    THIRD = ((1 << (Bits.THIRD + 1)) - 1) ^ 0xFF
    FOURTH = ((1 << (Bits.FOURTH + 1)) - 1) ^ 0xFF
    FIFTH = ((1 << (Bits.FIFTH + 1)) - 1) ^ 0xFF


class Mask(enum.IntFlag):
    FIRST = (1 << (Bits.FIRST + 0 * Bits.X)) - 1
    SECOND = (1 << (Bits.SECOND + 1 * Bits.X)) - 1
    THIRD = (1 << (Bits.THIRD + 2 * Bits.X)) - 1
    FOURTH = (1 << (Bits.FOURTH + 3 * Bits.X)) - 1
    X = (1 << Bits.X) - 1
    X_TEST = X ^ 0xFF
