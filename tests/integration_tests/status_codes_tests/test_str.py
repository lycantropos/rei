from hypothesis import given

from tests.integration_tests.hints import BoundPortedStatusCodePair
from . import strategies


@given(strategies.status_codes_pairs)
def test_basic(pair: BoundPortedStatusCodePair) -> None:
    bound, ported = pair

    assert str(bound) == str(ported)
