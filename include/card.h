#ifndef __CARD_H__
#define __CARD_H__

#include "utility.h"

/* mix in bold and light grey background for some colours  */
#define CARD_COLOUR_RED     ANSI_COLOUR_LIGHTGREY_BG ANSI_COLOUR_BOLD ANSI_COLOUR_RED
#define CARD_COLOUR_GREEN   ANSI_COLOUR_LIGHTGREY_BG ANSI_COLOUR_BOLD ANSI_COLOUR_GREEN
#define CARD_COLOUR_YELLOW  ANSI_COLOUR_LIGHTGREY_BG ANSI_COLOUR_BOLD ANSI_COLOUR_YELLOW
#define CARD_COLOUR_BLUE    ANSI_COLOUR_LIGHTGREY_BG ANSI_COLOUR_BOLD ANSI_COLOUR_BLUE

#define CARD_COLOUR_RESET   ANSI_COLOUR_RESET

typedef enum 
{
    WILD,
    RED,
    GREEN,
    BLUE,
    YELLOW,
	SUIT_CNT
} CardSuit_e;

typedef enum 
{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    DRAWTWO,
    REVERSE,
    SKIP,
    WILDDRAWFOUR,
    WILDCARD,
	CARDVAL_CNT
} CardValue_e;


typedef struct
{
    CardSuit_e eSuit;
    CardValue_e eValue;
} Card_t;


/************************************************************/

// Create a card object
Card_t card_CreateCard( CardSuit_e eSuit, CardValue_e eValue );

// Destroy a card object
void card_DestroyCard( Card_t aCard );

// get the string from the enum value of a suit
const char* card_GetSuitString( CardSuit_e eSuit );

// get the string from the enum value of a value
const char* card_GetValueString( CardValue_e eValue );

// get the ansi-colour coded string
const char* card_GetPrettyPrint( CardSuit_e eSuit, CardValue_e eValue );







/************************************************************/
#endif