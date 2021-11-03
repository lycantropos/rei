from enum import Enum
from typing import Type

from tests.binding_tests.hints import (BoundRune,
                                       BoundStatusCode)
from tests.integration_tests.hints import BoundPortedStatusCodePair
from tests.port_tests.hints import (PortedRune,
                                    PortedStatusCode)


def are_bound_ported_runes_equivalent(bound: BoundRune,
                                      ported: PortedRune) -> bool:
    return bound.components == ported.components


def are_enumerations_equivalent(left: Type[Enum], right: Type[Enum]) -> bool:
    left_members, right_members = left.__members__, right.__members__
    return (left.__name__ == right.__name__
            and left_members.keys() == right_members.keys()
            and all(left_value == right_members[name]
                    for name, left_value in left_members.items()))


def to_bound_with_ported_status_code_pairs(value: int
                                           ) -> BoundPortedStatusCodePair:
    return BoundStatusCode(value), PortedStatusCode(value)
