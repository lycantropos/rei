from hypothesis import given

from rei.consts import MAX_RUNE_SIZE
from tests.binding_tests.hints import BoundRune
from . import strategies


@given(strategies.runes)
def test_initialization(rune: BoundRune) -> None:
    assert 0 < len(rune) <= MAX_RUNE_SIZE
