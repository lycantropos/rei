from hypothesis import strategies

characters = strategies.characters(blacklist_categories=['Cs']).map(str.encode)
