from hypothesis import given

from tests.integration_tests.hints import BoundPortedRuneRangesPair
from tests.utils import equivalence
from . import strategies


@given(strategies.rune_ranges_pairs, strategies.rune_ranges_pairs)
def test_basic(first_pair: BoundPortedRuneRangesPair,
               second_pair: BoundPortedRuneRangesPair) -> None:
    first_bound, first_ported = first_pair
    second_bound, second_ported = second_pair

    assert equivalence(first_bound < second_bound,
                       first_ported < second_ported)
