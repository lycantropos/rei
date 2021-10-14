from hypothesis import given

from tests.binding_tests.hints import BoundRune
from tests.integration_tests.utils import are_bound_ported_runes_equivalent
from tests.port_tests.hints import PortedRune
from . import strategies


@given(strategies.characters)
def test_basic(characters: bytes) -> None:
    bound = BoundRune(characters)
    ported = PortedRune(characters)

    assert are_bound_ported_runes_equivalent(bound, ported)
