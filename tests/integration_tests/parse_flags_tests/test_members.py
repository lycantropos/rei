from tests.binding_tests.hints import BoundParseFlag
from tests.integration_tests.utils import are_enumerations_equivalent
from tests.port_tests.hints import PortedParseFlag


def test_basic() -> None:
    assert are_enumerations_equivalent(BoundParseFlag, PortedParseFlag)
