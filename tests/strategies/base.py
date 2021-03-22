from hypothesis import strategies

from tests.hints import Strategy
from tests.utils import (pack,
                         sort_pair)

patterns_special_characters = ['+', '*', '?', '^', '(', ')', '[', ']', '{',
                               '}', '|', '\\']
escape_special_character = '\\{}'.format
exact_patterns = strategies.text(
        strategies.characters(blacklist_characters=patterns_special_characters,
                              blacklist_categories=['Cs'])
        | (strategies.sampled_from(patterns_special_characters)
           .map(escape_special_character)))
non_empty_exact_patterns = exact_patterns.filter(bool)
characters_lists = non_empty_exact_patterns.map('[{}]'.format)
excluded_characters_lists = non_empty_exact_patterns.map('[^{}]'.format)


def extend_patterns(strategy: Strategy[str]) -> Strategy[str]:
    return strategy.map('({})'.format)


unrepeated_patterns = strategies.recursive(exact_patterns | characters_lists
                                           | excluded_characters_lists,
                                           extend_patterns)


def to_repeated_patterns(strategy: Strategy[str]) -> Strategy[str]:
    counts = strategies.integers(0, 10)
    templates = (strategies.sampled_from(['{}?', '{}*', '{}+'])
                 | counts.map('{{}}{{{{{},}}}}'.format)
                 | counts.map('{{}}{{{{{}}}}}'.format)
                 | (strategies.tuples(counts, counts).map(sort_pair)
                    .map(pack('{{}}{{{{{},{}}}}}'.format))))

    def apply_template(template: str) -> Strategy[str]:
        return strategy.map(template.format)

    return templates.flatmap(apply_template)


patterns = (unrepeated_patterns
            | to_repeated_patterns(unrepeated_patterns.filter(bool)))
patterns |= strategies.tuples(patterns, patterns).map(pack('{}|{}'.format))
patterns |= patterns.map('^{}'.format)
patterns |= patterns.map('{}$'.format)
