from hypothesis import given

from tests.binding_tests.hints import BoundCharClassBuilder
from tests.integration_tests.hints import BoundPortedRuneRangeListsPair
from tests.integration_tests.utils import (
    are_bound_ported_char_class_builders_equivalent)
from tests.port_tests.hints import PortedCharClassBuilder
from . import strategies


@given(strategies.rune_range_lists_pairs)
def test_basic(rune_range_lists_pair: BoundPortedRuneRangeListsPair) -> None:
    bound_rune_ranges, ported_rune_ranges = rune_range_lists_pair

    bound, ported = (BoundCharClassBuilder(*bound_rune_ranges),
                     PortedCharClassBuilder(*ported_rune_ranges))

    assert are_bound_ported_char_class_builders_equivalent(bound, ported)
