from tests.binding_tests.hints import BoundStatusCode
from tests.integration_tests.utils import are_enumerations_equivalent
from tests.port_tests.hints import PortedStatusCode


def test_basic() -> None:
    assert are_enumerations_equivalent(BoundStatusCode, PortedStatusCode)
