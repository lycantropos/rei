from tests.binding_tests.hints import BoundEncoding
from tests.integration_tests.utils import are_enumerations_equivalent
from tests.port_tests.hints import PortedEncoding


def test_basic() -> None:
    assert are_enumerations_equivalent(BoundEncoding, PortedEncoding)
