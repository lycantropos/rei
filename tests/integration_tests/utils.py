from enum import Enum
from typing import Type

from tests.binding_tests.hints import (BoundCannedOption,
                                       BoundEncoding,
                                       BoundOptions,
                                       BoundParseFlag,
                                       BoundParseState,
                                       BoundRune,
                                       BoundRuneRange,
                                       BoundStatus,
                                       BoundStatusCode)
from tests.integration_tests.hints import (BoundPortedCannedOptionsPair,
                                           BoundPortedEncodingsPair,
                                           BoundPortedParseFlagsPair,
                                           BoundPortedRuneRangesPair,
                                           BoundPortedRunesPair,
                                           BoundPortedStatusCodesPair)
from tests.port_tests.hints import (PortedCannedOption,
                                    PortedEncoding,
                                    PortedOptions,
                                    PortedParseFlag,
                                    PortedParseState,
                                    PortedRune,
                                    PortedRuneRange,
                                    PortedStatus,
                                    PortedStatusCode)


def are_bound_ported_runes_equivalent(bound: BoundRune,
                                      ported: PortedRune) -> bool:
    return bound.components == ported.components


def are_bound_ported_rune_ranges_equivalent(bound: BoundRuneRange,
                                            ported: PortedRuneRange) -> bool:
    return (are_bound_ported_runes_equivalent(bound.low, ported.low)
            and are_bound_ported_runes_equivalent(bound.high, ported.high))


def are_bound_ported_statuses_equivalent(bound: BoundStatus,
                                         ported: PortedStatus) -> bool:
    return bound.code == ported.code and bound.error_arg == ported.error_arg


def are_bound_ported_options_equivalent(bound: BoundOptions,
                                        ported: PortedOptions) -> bool:
    return (bound.encoding == ported.encoding
            and bound.posix_syntax is ported.posix_syntax
            and bound.longest_match is ported.longest_match
            and bound.log_errors is ported.log_errors
            and bound.max_memory == ported.max_memory
            and bound.literal is ported.literal
            and bound.never_nl is ported.never_nl
            and bound.dot_nl is ported.dot_nl
            and bound.never_capture is ported.never_capture
            and bound.case_sensitive is ported.case_sensitive
            and bound.perl_classes is ported.perl_classes
            and bound.word_boundary is ported.word_boundary
            and bound.one_line is ported.one_line)


def are_bound_ported_parse_states_equivalent(bound: BoundParseState,
                                             ported: PortedParseState) -> bool:
    return (bound.flag == ported.flag
            and bound.pattern == ported.pattern
            and are_bound_ported_statuses_equivalent(bound.status,
                                                     ported.status))


def are_enumerations_equivalent(left: Type[Enum], right: Type[Enum]) -> bool:
    left_members, right_members = left.__members__, right.__members__
    return (left.__name__ == right.__name__
            and left_members.keys() == right_members.keys()
            and all(left_value == right_members[name]
                    for name, left_value in left_members.items()))


def to_bound_with_ported_canned_options_pair(value: int
                                             ) -> BoundPortedCannedOptionsPair:
    return BoundCannedOption(value), PortedCannedOption(value)


def to_bound_with_ported_encodings_pair(value: int
                                        ) -> BoundPortedEncodingsPair:
    return BoundEncoding(value), PortedEncoding(value)


def to_bound_with_ported_runes_pair(characters: bytes) -> BoundPortedRunesPair:
    return BoundRune(characters), PortedRune(characters)


def to_bound_with_ported_rune_ranges_pair(lows_pair: BoundPortedRunesPair,
                                          highs_pair: BoundPortedRunesPair
                                          ) -> BoundPortedRuneRangesPair:
    bound_low, ported_low = lows_pair
    bound_high, ported_high = highs_pair
    return (BoundRuneRange(bound_low, bound_high),
            PortedRuneRange(ported_low, ported_high))


def to_bound_with_ported_status_codes_pair(value: int
                                           ) -> BoundPortedStatusCodesPair:
    return BoundStatusCode(value), PortedStatusCode(value)


def to_bound_with_ported_parse_flags_pair(value: int
                                          ) -> BoundPortedParseFlagsPair:
    return BoundParseFlag(value), PortedParseFlag(value)
