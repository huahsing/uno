
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "deck.h"

int cardQtyMatrix[5][15] =
{
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  4 },
    { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 },
    { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 },
    { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 },
    { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 }
};


Deck_t deck_InitDeck()
{
	Deck_t aDeck;
	aDeck.totalDeckSpace = DECK_SIZE;
	int i, j, k;
	
	aDeck.cardCount = 0;
	
	for( i = 0; i < SUIT_CNT; i++ )
	{
		for( j = 0; j < CARDVAL_CNT; j++ )
		{
			for( k = 0; k < cardQtyMatrix[i][j]; k++ )
			{
				Card_t aCard = {i,j};
				deck_PushCard( &aDeck, aCard);
			}
		}
	}
	
	printf( "Total number of cards populated = %d\n", aDeck.cardCount );
	
	return aDeck;
}

Deck_t deck_InitEmptyDeck()
{
	Deck_t aDeck;
	aDeck.totalDeckSpace = DECK_SIZE;
	aDeck.cardCount = 0;
	
	return aDeck;
}

void deck_DestroyDeck( Deck_t aDeck )
{
	// empty implementation
}

void deck_PrintDeck( Deck_t* const pDeck )
{
	int i;
	
	for( i = 0; i < pDeck->cardCount; i++ )
	{
		printf( "[%03d] %s\n", i+1, card_GetPrettyPrint( pDeck->cards[i].eSuit, pDeck->cards[i].eValue ) );
	}
}

void deck_PushCard( Deck_t* pDeck, Card_t aCard )
{
	if( pDeck->cardCount >= DECK_SIZE )
	{
		printf( "Error in adding card, deck is already full!\n" );
	}
	else
	{
		pDeck->cards[pDeck->cardCount++] = aCard;
	}
}

Card_t deck_RemoveCardAt( Deck_t* pDeck, int index )
{
	Card_t removedCard = {-1, -1};
	
	if( index >= pDeck->cardCount )
	{
		printf( "Error in removing card. \
		        Card not found at current index, \
				current card count = %d\n", pDeck->cardCount );
	}
	else
	{
		int i;
		
		removedCard = pDeck->cards[index];
		
		for( i = index+1; i < pDeck->cardCount; i++ )
		{
			pDeck->cards[i-1] = pDeck->cards[i];
		}
		
		pDeck->cardCount--;
	}
	
	return removedCard;
}

Card_t deck_PopCard( Deck_t* pDeck )
{
    return deck_RemoveCardAt( pDeck, pDeck->cardCount-1 );
}

const Card_t deck_PeekTop( Deck_t* const pDeck )
{
    if( !deck_IsEmpty( pDeck ) )
    {
        return pDeck->cards[pDeck->cardCount-1];
    }
    else
        return (Card_t) {-1,-1};
}

BOOL_t deck_IsEmpty( Deck_t* const pDeck )
{
	if( pDeck->cardCount == 0 )
		return BOOL_TRUE;
	else
		return BOOL_FALSE;
}

void deck_ShuffleDeck( Deck_t* pDeck )
{
	Deck_t shuffledDeck = deck_InitEmptyDeck();
	Card_t tmpCard;
	
	srand( time(NULL) );
	
	while( !deck_IsEmpty( pDeck ) )
	{
		tmpCard = deck_RemoveCardAt( pDeck, rand() % pDeck->cardCount );
		deck_PushCard( &shuffledDeck, tmpCard );
	}
	
	*pDeck = shuffledDeck;
}
