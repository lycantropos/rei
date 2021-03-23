from hypothesis import given

from tests.binding_tests.hints import BoundRune
from tests.utils import (equivalence,
                         implication)
from . import strategies


@given(strategies.runes)
def test_reflexivity(rune: BoundRune) -> None:
    assert rune == rune


@given(strategies.runes, strategies.runes)
def test_symmetry(first_rune: BoundRune,
                  second_rune: BoundRune) -> None:
    assert equivalence(first_rune == second_rune, second_rune == first_rune)


@given(strategies.runes, strategies.runes, strategies.runes)
def test_transitivity(first_rune: BoundRune, second_rune: BoundRune,
                      third_rune: BoundRune) -> None:
    assert implication(first_rune == second_rune and second_rune == third_rune,
                       first_rune == third_rune)


@given(strategies.runes, strategies.runes)
def test_connection_with_inequality(first_rune: BoundRune,
                                    second_rune: BoundRune) -> None:
    assert equivalence(not first_rune == second_rune,
                       first_rune != second_rune)
