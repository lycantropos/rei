from .canned_option import CannedOption
from .encoding import Encoding


class Options:
    def __init__(self,
                 encoding: Encoding = Encoding.UTF_8,
                 posix_syntax: bool = False,
                 longest_match: bool = False,
                 log_errors: bool = True,
                 max_memory: int = 8 << 20,
                 literal: bool = False,
                 never_nl: bool = False,
                 dot_nl: bool = False,
                 never_capture: bool = False,
                 case_sensitive: bool = True,
                 perl_classes: bool = False,
                 word_boundary: bool = False,
                 one_line: bool = False) -> None:
        self.encoding = encoding
        self.posix_syntax = posix_syntax
        self.longest_match = longest_match
        self.log_errors = log_errors
        self.max_memory = max_memory
        self.literal = literal
        self.never_nl = never_nl
        self.dot_nl = dot_nl
        self.never_capture = never_capture
        self.case_sensitive = case_sensitive
        self.perl_classes = perl_classes
        self.word_boundary = word_boundary
        self.one_line = one_line

    @classmethod
    def from_canned_option(cls, option: CannedOption) -> 'Options':
        return cls(encoding=(Encoding.LATIN_1
                             if option is CannedOption.LATIN_1
                             else Encoding.UTF_8),
                   posix_syntax=option is CannedOption.POSIX,
                   longest_match=option is CannedOption.POSIX,
                   log_errors=option is not CannedOption.QUIET)
