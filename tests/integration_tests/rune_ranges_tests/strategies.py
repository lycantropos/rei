from hypothesis import strategies

from tests.integration_tests.utils import to_bound_with_ported_runes_pair

characters = strategies.characters(blacklist_categories=['Cs']).map(str.encode)
runes_pairs = strategies.builds(to_bound_with_ported_runes_pair, characters)
