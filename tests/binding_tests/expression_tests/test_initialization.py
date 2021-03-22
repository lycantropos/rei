from hypothesis import given

from tests.binding_tests.hints import BoundExpression
from . import strategies


@given(strategies.patterns)
def test_initialization(pattern: str) -> None:
    result = BoundExpression(pattern)

    assert result.pattern == pattern
