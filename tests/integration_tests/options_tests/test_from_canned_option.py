from hypothesis import given

from tests.binding_tests.hints import BoundOptions
from tests.integration_tests.hints import BoundPortedCannedOptionsPair
from tests.integration_tests.utils import are_bound_ported_options_equivalent
from tests.port_tests.hints import PortedOptions
from . import strategies


@given(strategies.canned_options_pairs)
def test_basic(canned_options_pair: BoundPortedCannedOptionsPair) -> None:
    bound_canned_option, ported_canned_option = canned_options_pair

    bound, ported = (BoundOptions.from_canned_option(bound_canned_option),
                     PortedOptions.from_canned_option(ported_canned_option))

    assert are_bound_ported_options_equivalent(bound, ported)
