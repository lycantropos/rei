from hypothesis import given

from tests.integration_tests.hints import BoundPortedRunesPair
from tests.utils import equivalence
from . import strategies


@given(strategies.runes_pairs, strategies.runes_pairs)
def test_basic(first_pair: BoundPortedRunesPair,
               second_pair: BoundPortedRunesPair) -> None:
    first_bound, first_ported = first_pair
    second_bound, second_ported = second_pair

    assert equivalence(first_bound < second_bound,
                       first_ported < second_ported)
