from reprit.base import generate_repr


class Expression:
    __slots__ = '_pattern',

    def __init__(self, pattern: str) -> None:
        self._pattern = pattern

    __repr__ = generate_repr(__init__)

    @property
    def pattern(self) -> str:
        return self._pattern
