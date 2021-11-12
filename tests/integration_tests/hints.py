from typing import (List,
                    Tuple)

from tests.binding_tests.hints import (BoundCannedOption,
                                       BoundEncoding,
                                       BoundParseFlag,
                                       BoundRune,
                                       BoundRuneRange,
                                       BoundStatusCode)
from tests.port_tests.hints import (PortedCannedOption,
                                    PortedEncoding,
                                    PortedParseFlag,
                                    PortedRune,
                                    PortedRuneRange,
                                    PortedStatusCode)

BoundPortedCannedOptionsPair = Tuple[BoundCannedOption, PortedCannedOption]
BoundPortedEncodingsPair = Tuple[BoundEncoding, PortedEncoding]
BoundPortedStatusCodesPair = Tuple[BoundStatusCode, PortedStatusCode]
BoundPortedParseFlagsPair = Tuple[BoundParseFlag, PortedParseFlag]
BoundPortedRunesPair = Tuple[BoundRune, PortedRune]
BoundPortedRuneRangesPair = Tuple[BoundRuneRange, PortedRuneRange]
BoundPortedRuneRangeListsPair = Tuple[List[BoundRuneRange],
                                      List[PortedRuneRange]]
