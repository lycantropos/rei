from hypothesis import strategies

from tests.binding_tests.hints import BoundRune

bytes_sequences = (strategies.characters(blacklist_categories=['Cs'])
                   .map(str.encode))
runes = strategies.builds(BoundRune, bytes_sequences)
