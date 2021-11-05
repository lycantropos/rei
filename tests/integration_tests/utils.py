from enum import Enum
from typing import Type

from tests.binding_tests.hints import (BoundRune,
                                       BoundStatus,
                                       BoundStatusCode)
from tests.integration_tests.hints import BoundPortedStatusCodesPair
from tests.port_tests.hints import (PortedRune,
                                    PortedStatus,
                                    PortedStatusCode)


def are_bound_ported_runes_equivalent(bound: BoundRune,
                                      ported: PortedRune) -> bool:
    return bound.components == ported.components


def are_bound_ported_statuses_equivalent(bound: BoundStatus,
                                         ported: PortedStatus) -> bool:
    return bound.code == ported.code and bound.error_arg == ported.error_arg


def are_enumerations_equivalent(left: Type[Enum], right: Type[Enum]) -> bool:
    left_members, right_members = left.__members__, right.__members__
    return (left.__name__ == right.__name__
            and left_members.keys() == right_members.keys()
            and all(left_value == right_members[name]
                    for name, left_value in left_members.items()))


def to_bound_with_ported_status_code_pairs(value: int
                                           ) -> BoundPortedStatusCodesPair:
    return BoundStatusCode(value), PortedStatusCode(value)
