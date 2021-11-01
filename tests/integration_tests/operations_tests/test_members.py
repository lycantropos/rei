from tests.binding_tests.hints import BoundOperation
from tests.integration_tests.utils import are_enumerations_equivalent
from tests.port_tests.hints import PortedOperation


def test_basic() -> None:
    assert are_enumerations_equivalent(BoundOperation, PortedOperation)
