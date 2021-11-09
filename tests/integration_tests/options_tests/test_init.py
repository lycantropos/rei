from hypothesis import given

from tests.binding_tests.hints import BoundOptions
from tests.integration_tests.hints import BoundPortedEncodingsPair
from tests.integration_tests.utils import are_bound_ported_options_equivalent
from tests.port_tests.hints import PortedOptions
from . import strategies


def test_no_arguments() -> None:
    bound, ported = BoundOptions(), PortedOptions()

    assert are_bound_ported_options_equivalent(bound, ported)


@given(strategies.encodings_pairs, strategies.booleans, strategies.booleans,
       strategies.booleans, strategies.integers_64, strategies.booleans,
       strategies.booleans, strategies.booleans, strategies.booleans,
       strategies.booleans, strategies.booleans, strategies.booleans,
       strategies.booleans)
def test_basic(encodings_pair: BoundPortedEncodingsPair,
               posix_syntax: bool,
               longest_match: bool,
               log_errors: bool,
               max_memory: int,
               literal: bool,
               never_nl: bool,
               dot_nl: bool,
               never_capture: bool,
               case_sensitive: bool,
               perl_classes: bool,
               word_boundary: bool,
               one_line: bool) -> None:
    bound_encoding, ported_encoding = encodings_pair

    bound, ported = (BoundOptions(bound_encoding, posix_syntax,
                                  longest_match, log_errors, max_memory,
                                  literal, never_nl, dot_nl, never_capture,
                                  case_sensitive, perl_classes, word_boundary,
                                  one_line),
                     PortedOptions(ported_encoding, posix_syntax,
                                   longest_match, log_errors, max_memory,
                                   literal, never_nl, dot_nl, never_capture,
                                   case_sensitive, perl_classes, word_boundary,
                                   one_line))

    assert are_bound_ported_options_equivalent(bound, ported)
