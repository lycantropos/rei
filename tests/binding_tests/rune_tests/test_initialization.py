from hypothesis import given

from tests.binding_tests.hints import BoundRune
from . import strategies


@given(strategies.bytes_sequences)
def test_initialization(components: bytes) -> None:
    result = BoundRune(components)

    assert result.components == components
