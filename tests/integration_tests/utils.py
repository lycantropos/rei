from tests.binding_tests.hints import BoundRune
from tests.port_tests.hints import PortedRune


def are_bound_ported_runes_equivalent(bound: BoundRune,
                                      ported: PortedRune) -> bool:
    return bound.components == ported.components
