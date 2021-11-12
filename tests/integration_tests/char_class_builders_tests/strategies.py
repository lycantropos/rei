from hypothesis import strategies

from tests.integration_tests.utils import (
    to_bound_with_char_class_builders_pair,
    to_bound_with_ported_rune_ranges_pair,
    to_bound_with_ported_runes_pair)
from tests.utils import transpose

characters = strategies.characters(blacklist_categories=['Cs']).map(str.encode)
runes_pairs = strategies.builds(to_bound_with_ported_runes_pair, characters)
rune_ranges_pairs = strategies.builds(to_bound_with_ported_rune_ranges_pair,
                                      runes_pairs, runes_pairs)
rune_range_lists_pairs = strategies.lists(rune_ranges_pairs).map(transpose)
char_class_builders_pairs = strategies.builds(
        to_bound_with_char_class_builders_pair, rune_range_lists_pairs)
