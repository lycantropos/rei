from hypothesis import given

from tests.integration_tests.hints import BoundPortedRunesPair
from . import strategies


@given(strategies.runes_pairs)
def test_basic(pair: BoundPortedRunesPair) -> None:
    bound, ported = pair

    assert len(bound) == len(ported)
