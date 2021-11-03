from hypothesis import strategies

from tests.integration_tests.utils import (
    to_bound_with_ported_status_code_pairs)
from tests.port_tests.hints import PortedStatusCode

status_codes_pairs = strategies.builds(
        to_bound_with_ported_status_code_pairs,
        strategies.sampled_from(range(len(PortedStatusCode))))
