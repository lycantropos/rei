from typing import Tuple

from tests.binding_tests.hints import (BoundEncoding,
                                       BoundParseFlag,
                                       BoundStatusCode)
from tests.port_tests.hints import (PortedEncoding,
                                    PortedParseFlag,
                                    PortedStatusCode)

BoundPortedEncodingsPair = Tuple[BoundEncoding, PortedEncoding]
BoundPortedStatusCodesPair = Tuple[BoundStatusCode, PortedStatusCode]
BoundPortedParseFlagsPair = Tuple[BoundParseFlag, PortedParseFlag]
