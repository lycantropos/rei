from hypothesis import strategies

from tests.integration_tests.utils import (
    to_bound_with_ported_parse_flags_pairs)
from tests.port_tests.hints import PortedParseFlag
from tests.strategies import patterns

parse_flags_pairs = strategies.builds(
        to_bound_with_ported_parse_flags_pairs,
        strategies.sampled_from(list(map(int, PortedParseFlag))))
patterns = patterns
