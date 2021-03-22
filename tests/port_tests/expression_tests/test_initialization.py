from hypothesis import given

from tests.port_tests.hints import PortedExpression
from . import strategies


@given(strategies.patterns)
def test_initialization(pattern: str) -> None:
    result = PortedExpression(pattern)

    assert result.pattern == pattern
