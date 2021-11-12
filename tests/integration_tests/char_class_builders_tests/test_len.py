from hypothesis import given

from tests.integration_tests.hints import BoundPortedCharClassBuildersPair
from . import strategies


@given(strategies.char_class_builders_pairs)
def test_basic(pair: BoundPortedCharClassBuildersPair) -> None:
    bound, ported = pair

    assert len(bound) == len(ported)
