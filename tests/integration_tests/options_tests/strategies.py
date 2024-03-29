from hypothesis import strategies

from tests.integration_tests.utils import (
    to_bound_with_ported_canned_options_pair,
    to_bound_with_ported_encodings_pair)
from tests.port_tests.hints import (PortedCannedOption,
                                    PortedEncoding)

MAX_SIGNED_INTEGER_64_MODULUS = 1 << 63
booleans = strategies.booleans()
encodings_pairs = strategies.builds(
        to_bound_with_ported_encodings_pair,
        strategies.sampled_from(list(map(int, PortedEncoding))))
canned_options_pairs = strategies.builds(
        to_bound_with_ported_canned_options_pair,
        strategies.sampled_from(list(map(int, PortedCannedOption))))
integers_64 = strategies.integers(-MAX_SIGNED_INTEGER_64_MODULUS,
                                  MAX_SIGNED_INTEGER_64_MODULUS - 1)
