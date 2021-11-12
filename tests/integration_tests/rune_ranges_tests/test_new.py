from hypothesis import given

from tests.binding_tests.hints import BoundRuneRange
from tests.integration_tests.hints import BoundPortedRunesPair
from tests.integration_tests.utils import (
    are_bound_ported_rune_ranges_equivalent)
from tests.port_tests.hints import PortedRuneRange
from . import strategies


@given(strategies.runes_pairs)
def test_single_argument(lows_pair: BoundPortedRunesPair) -> None:
    bound_low, ported_low = lows_pair

    bound, ported = (BoundRuneRange(bound_low), PortedRuneRange(ported_low))

    assert are_bound_ported_rune_ranges_equivalent(bound, ported)


@given(strategies.runes_pairs, strategies.runes_pairs)
def test_basic(lows_pair: BoundPortedRunesPair,
               highs_pair: BoundPortedRunesPair) -> None:
    bound_low, ported_low = lows_pair
    bound_high, ported_high = highs_pair

    bound, ported = (BoundRuneRange(bound_low, bound_high),
                     PortedRuneRange(ported_low, ported_high))

    assert are_bound_ported_rune_ranges_equivalent(bound, ported)
