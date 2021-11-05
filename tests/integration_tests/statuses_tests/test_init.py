from hypothesis import given

from tests.binding_tests.hints import BoundStatus
from tests.integration_tests.hints import BoundPortedStatusCodesPair
from tests.integration_tests.utils import are_bound_ported_statuses_equivalent
from tests.port_tests.hints import PortedStatus
from . import strategies


@given(strategies.status_codes_pairs, strategies.strings)
def test_basic(status_codes_pair: BoundPortedStatusCodesPair,
               error_arg: str) -> None:
    bound_status_code, ported_status_code = status_codes_pair

    bound, ported = (BoundStatus(bound_status_code, error_arg),
                     PortedStatus(ported_status_code, error_arg))

    assert are_bound_ported_statuses_equivalent(bound, ported)
