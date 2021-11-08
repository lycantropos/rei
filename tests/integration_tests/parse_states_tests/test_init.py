from hypothesis import given

from tests.binding_tests.hints import BoundParseState
from tests.integration_tests.hints import BoundPortedParseFlagsPair
from tests.integration_tests.utils import (
    are_bound_ported_parse_states_equivalent)
from tests.port_tests.hints import PortedParseState
from . import strategies


@given(strategies.parse_flags_pairs, strategies.patterns)
def test_basic(parse_flags_pair: BoundPortedParseFlagsPair,
               pattern: str) -> None:
    bound_status_code, ported_status_code = parse_flags_pair

    bound, ported = (BoundParseState(pattern, bound_status_code),
                     PortedParseState(pattern, ported_status_code))

    assert are_bound_ported_parse_states_equivalent(bound, ported)
