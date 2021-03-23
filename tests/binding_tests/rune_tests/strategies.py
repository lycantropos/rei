from hypothesis import strategies

bytes_sequences = (strategies.characters(blacklist_categories=['Cs'])
                   .map(str.encode))
