#include <stdio.h>
#include "deck.h"

int main()
{
    printf( "%s\n", card_GetPrettyPrint(RED, ONE) );
    printf( "%s\n", card_GetPrettyPrint(GREEN, DRAWTWO) );
    printf( "%s\n", card_GetPrettyPrint(BLUE, REVERSE) );
    printf( "%s\n", card_GetPrettyPrint(YELLOW, ZERO) );
    
    printf( "%sHello World%s\n", ANSI_COLOUR_BLUE, ANSI_COLOUR_RESET );
    
    return 0;
}