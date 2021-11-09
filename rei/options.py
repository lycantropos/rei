from .encoding import Encoding


class Options:
    def __init__(self,
                 encoding: Encoding,
                 posix_syntax: bool,
                 longest_match: bool,
                 log_errors: bool,
                 max_memory: int,
                 literal: bool,
                 never_nl: bool,
                 dot_nl: bool,
                 never_capture: bool,
                 case_sensitive: bool,
                 perl_classes: bool,
                 word_boundary: bool,
                 one_line: bool) -> None:
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
