#ifndef __DECK_H__
#define __DECK_H__

#include "card.h"

#define DECK_SIZE 108

typedef struct
{
	int totalDeckSpace;
	int cardCount;
	Card_t cards[DECK_SIZE];
} Deck_t;

Deck_t deck_InitDeck();

Deck_t deck_InitEmptyDeck();

void deck_DestroyDeck( Deck_t aDeck );

void deck_PrintDeck( Deck_t* const pDeck );

void deck_PushCard( Deck_t* pDeck, Card_t aCard );

Card_t deck_RemoveCardAt( Deck_t* pDeck, int index );

Card_t deck_PopCard( Deck_t* pDeck );

const Card_t deck_PeekTop( Deck_t* const pDeck );

BOOL_t deck_IsEmpty( Deck_t* const pDeck );

void deck_ShuffleDeck( Deck_t* pDeck );

#endif