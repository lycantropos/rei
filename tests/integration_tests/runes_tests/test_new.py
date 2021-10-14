from hypothesis import given

from tests.utils import (BoundRune,
                         PortedRune,
                         are_bound_ported_runes_equivalent)
from . import strategies


@given(strategies.characters)
def test_basic(characters: bytes) -> None:
    bound = BoundRune(characters)
    ported = PortedRune(characters)

    assert are_bound_ported_runes_equivalent(bound, ported)
