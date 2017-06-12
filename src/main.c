#include <stdio.h>
#include "deck.h"

int main()
{
    Deck_t unoDeck = deck_InitDeck();
	
	deck_PrintDeck( &unoDeck );
	
	deck_ShuffleDeck( &unoDeck );
	
	deck_PrintDeck( &unoDeck );
	
    return 0;
}