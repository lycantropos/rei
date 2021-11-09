from tests.binding_tests.hints import BoundCannedOption
from tests.integration_tests.utils import are_enumerations_equivalent
from tests.port_tests.hints import PortedCannedOption


def test_basic() -> None:
    assert are_enumerations_equivalent(BoundCannedOption, PortedCannedOption)
