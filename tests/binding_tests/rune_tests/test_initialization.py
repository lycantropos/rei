from hypothesis import given

from tests.binding_tests.hints import BoundRune
from . import strategies


@given(strategies.bytes_sequences)
def test_initialization(characters: bytes) -> None:
    result = BoundRune(characters)

    assert len(result) == len(characters)
    assert bytes(result) == characters
