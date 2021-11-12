from hypothesis import given

from tests.integration_tests.hints import BoundPortedCharClassBuildersPair
from tests.utils import equivalence
from . import strategies


@given(strategies.char_class_builders_pairs,
       strategies.char_class_builders_pairs)
def test_basic(first_pair: BoundPortedCharClassBuildersPair,
               second_pair: BoundPortedCharClassBuildersPair) -> None:
    first_bound, first_ported = first_pair
    second_bound, second_ported = second_pair

    assert equivalence(first_bound == second_bound,
                       first_ported == second_ported)
