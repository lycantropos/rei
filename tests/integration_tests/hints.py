from typing import Tuple

from tests.binding_tests.hints import (BoundCannedOption,
                                       BoundEncoding,
                                       BoundParseFlag,
                                       BoundStatusCode)
from tests.port_tests.hints import (PortedCannedOption,
                                    PortedEncoding,
                                    PortedParseFlag,
                                    PortedStatusCode)

BoundPortedCannedOptionsPair = Tuple[BoundCannedOption, PortedCannedOption]
BoundPortedEncodingsPair = Tuple[BoundEncoding, PortedEncoding]
BoundPortedStatusCodesPair = Tuple[BoundStatusCode, PortedStatusCode]
BoundPortedParseFlagsPair = Tuple[BoundParseFlag, PortedParseFlag]
