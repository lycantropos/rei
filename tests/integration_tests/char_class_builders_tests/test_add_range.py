from hypothesis import given

from tests.integration_tests.hints import (BoundPortedCharClassBuildersPair,
                                           BoundPortedRuneRangesPair)
from tests.integration_tests.utils import (
    are_bound_ported_char_class_builders_equivalent)
from tests.utils import equivalence
from . import strategies


@given(strategies.char_class_builders_pairs, strategies.rune_ranges_pairs)
def test_basic(pair: BoundPortedCharClassBuildersPair,
               rune_ranges_pair: BoundPortedRuneRangesPair) -> None:
    bound, ported = pair
    bound_rune_range, ported_rune_range = rune_ranges_pair

    bound_result, ported_result = (bound.add_range(bound_rune_range),
                                   ported.add_range(ported_rune_range))

    assert equivalence(bound_result, ported_result)
    assert are_bound_ported_char_class_builders_equivalent(bound, ported)
