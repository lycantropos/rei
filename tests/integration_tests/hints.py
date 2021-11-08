from typing import Tuple

from tests.binding_tests.hints import (BoundParseFlag,
                                       BoundStatusCode)
from tests.port_tests.hints import (PortedParseFlag,
                                    PortedStatusCode)

BoundPortedStatusCodesPair = Tuple[BoundStatusCode, PortedStatusCode]
BoundPortedParseFlagsPair = Tuple[BoundParseFlag, PortedParseFlag]
