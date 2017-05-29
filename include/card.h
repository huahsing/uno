#ifndef __CARD_H__
#define __CARD_H__

#include "utility.h"

typedef enum 
{
    WILD,
    RED,
    GREEN,
    BLUE,
    YELLOW
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
    WILDCARD
} CardValue_e;


/*
int izzQtys[][] =
{
    { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  4 },
    { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 },
    { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 },
    { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 },
    { 1,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  0,  0 }
};
*/

typedef struct
{
    CardSuit_e eSuit;
    CardValue_e eValue;
} Card_t;


/************************************************************/

// Create a card object
Card_t card_CreateCard( CardSuit_e eSuit, CardValue_e eValue );

// get the string from the enum value of a suit
const char* card_GetSuitString( CardSuit_e eSuit );

// get the string from the enum value of a value
const char* card_GetValueString( CardValue_e eValue );

// get the ansi-colour coded string
const char* card_GetPrettyPrint( CardSuit_e eSuit, CardValue_e eValue );







/************************************************************/
#endif