from typing import Optional

from reprit.base import generate_repr

from .canned_option import CannedOption
from .options import Options


class Expression:
    __slots__ = '_options', '_pattern'

    def __init__(self,
                 pattern: str,
                 options: Optional[Options] = None) -> None:
        self._pattern = pattern
        self._options = (Options.from_canned_option(CannedOption.DEFAULT)
                         if options is None
                         else options)

    __repr__ = generate_repr(__init__)

    @property
    def options(self) -> Options:
        return self._options

    @property
    def pattern(self) -> str:
        return self._pattern
