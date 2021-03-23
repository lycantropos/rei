import sys

from hypothesis import given

from tests.binding_tests.hints import BoundRune
from . import strategies


@given(strategies.runes)
def test_basic(rune: BoundRune) -> None:
    result = repr(rune)

    assert result.startswith(BoundRune.__module__)
    assert BoundRune.__qualname__ in result


@given(strategies.runes)
def test_round_trip(rune: BoundRune) -> None:
    result = repr(rune)

    assert eval(result, sys.modules) == rune
