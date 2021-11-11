from typing import Tuple

from tests.binding_tests.hints import (BoundCannedOption,
                                       BoundEncoding,
                                       BoundParseFlag,
                                       BoundRune,
                                       BoundStatusCode)
from tests.port_tests.hints import (PortedCannedOption,
                                    PortedEncoding,
                                    PortedParseFlag,
                                    PortedRune,
                                    PortedStatusCode)

BoundPortedCannedOptionsPair = Tuple[BoundCannedOption, PortedCannedOption]
BoundPortedEncodingsPair = Tuple[BoundEncoding, PortedEncoding]
BoundPortedStatusCodesPair = Tuple[BoundStatusCode, PortedStatusCode]
BoundPortedParseFlagsPair = Tuple[BoundParseFlag, PortedParseFlag]
BoundPortedRunesPair = Tuple[BoundRune, PortedRune]
