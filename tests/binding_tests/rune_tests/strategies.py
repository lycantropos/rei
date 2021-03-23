from hypothesis import strategies

bytes_sequences = strategies.characters().map(str.encode)
