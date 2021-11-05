from hypothesis import given

from tests.integration_tests.hints import BoundPortedStatusCodesPair
from . import strategies


@given(strategies.status_codes_pairs)
def test_basic(pair: BoundPortedStatusCodesPair) -> None:
    bound, ported = pair

    assert str(bound) == str(ported)
