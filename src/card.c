#include "card.h"


#define CASE_ENUM_TO_STR( ENUM_VAL, OUT_STR ) \
    case ENUM_VAL : \
        OUT_STR = #ENUM_VAL; \
        break;
        
#define CASE_ENUMS_TO_PRETTY( ENUM_CSUIT, ENUM_CVAL, OUT_STR ) \
    case ENUM_CVAL : \
        OUT_STR = CARD_COLOUR_##ENUM_CSUIT #ENUM_CVAL CARD_COLOUR_RESET; \
        break;

        
Card_t card_CreateCard( CardSuit_e eSuit, CardValue_e eValue )
{
    Card_t aCard;
    aCard.eSuit = eSuit;
    aCard.eValue = eValue;
    
    return aCard;
}

void card_DestroyCard( Card_t aCard )
{
	// empty implementation
}

const char* card_GetSuitString( CardSuit_e eSuit )
{
    const char* returnString;
    
    switch( eSuit )
    {
        CASE_ENUM_TO_STR( WILD, returnString );
        CASE_ENUM_TO_STR( RED, returnString );
        CASE_ENUM_TO_STR( GREEN, returnString );
        CASE_ENUM_TO_STR( BLUE, returnString );
        CASE_ENUM_TO_STR( YELLOW, returnString );
        default: 
            returnString = "Invalid suit string";
    }
    
    return returnString;
}

const char* card_GetColouredSuitString( CardSuit_e eSuit )
{
    const char* returnString;
    
    switch( eSuit )
    {
        case RED:
            returnString =  CARD_COLOUR_RED "RED" CARD_COLOUR_RESET;
            break;
        
        case GREEN:
            returnString =  CARD_COLOUR_GREEN "GREEN" CARD_COLOUR_RESET;
            break;
            
        case BLUE:
            returnString =  CARD_COLOUR_BLUE "BLUE" CARD_COLOUR_RESET;
            break;
            
        case YELLOW:
            returnString =  CARD_COLOUR_YELLOW "YELLOW" CARD_COLOUR_RESET;
            break;
            
        case WILD:
            returnString =  CARD_COLOUR_YELLOW "W" \
                            CARD_COLOUR_BLUE "I" \
                            CARD_COLOUR_RED "L" \
                            CARD_COLOUR_GREEN "D" \
                            CARD_COLOUR_RESET;
            break;
            
        default: 
            returnString = "Invalid suit string";
    }
    
    return returnString;
}

const char* card_GetValueString( CardValue_e eValue )
{
    const char* returnString;
    
    switch( eValue )
    {
        CASE_ENUM_TO_STR( ZERO, returnString );
        CASE_ENUM_TO_STR( ONE, returnString );
        CASE_ENUM_TO_STR( TWO, returnString );
        CASE_ENUM_TO_STR( THREE, returnString );
        CASE_ENUM_TO_STR( FOUR, returnString );
        CASE_ENUM_TO_STR( FIVE, returnString );
        CASE_ENUM_TO_STR( SIX, returnString );
        CASE_ENUM_TO_STR( SEVEN, returnString );
        CASE_ENUM_TO_STR( EIGHT, returnString );
        CASE_ENUM_TO_STR( NINE, returnString );
        CASE_ENUM_TO_STR( DRAWTWO, returnString );
        CASE_ENUM_TO_STR( REVERSE, returnString );
        CASE_ENUM_TO_STR( SKIP, returnString );
        CASE_ENUM_TO_STR( WILDDRAWFOUR, returnString );
        CASE_ENUM_TO_STR( WILDCARD, returnString );
        default: 
            returnString = "Invalid suit string";
    }
    
     return returnString;
}

const char* card_GetPrettyPrint( CardSuit_e eSuit, CardValue_e eValue )
{
    const char* returnString;
    
    switch( eSuit )
    {
        case RED:
            switch( eValue )
            {
                CASE_ENUMS_TO_PRETTY( RED, ZERO,    returnString );
                CASE_ENUMS_TO_PRETTY( RED, ONE,     returnString );
                CASE_ENUMS_TO_PRETTY( RED, TWO,     returnString );
                CASE_ENUMS_TO_PRETTY( RED, THREE,   returnString );
                CASE_ENUMS_TO_PRETTY( RED, FOUR,    returnString );
                CASE_ENUMS_TO_PRETTY( RED, FIVE,    returnString );
                CASE_ENUMS_TO_PRETTY( RED, SIX,     returnString );
                CASE_ENUMS_TO_PRETTY( RED, SEVEN,   returnString );
                CASE_ENUMS_TO_PRETTY( RED, EIGHT,   returnString );
                CASE_ENUMS_TO_PRETTY( RED, NINE,    returnString );
                CASE_ENUMS_TO_PRETTY( RED, DRAWTWO, returnString );
                CASE_ENUMS_TO_PRETTY( RED, REVERSE, returnString );
                CASE_ENUMS_TO_PRETTY( RED, SKIP,    returnString );
                default:
                    returnString = "Invalid value string for RED";
            }
            break;
            
        case GREEN:
            switch( eValue )
            {
                CASE_ENUMS_TO_PRETTY( GREEN, ZERO,    returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, ONE,     returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, TWO,     returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, THREE,   returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, FOUR,    returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, FIVE,    returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, SIX,     returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, SEVEN,   returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, EIGHT,   returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, NINE,    returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, DRAWTWO, returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, REVERSE, returnString );
                CASE_ENUMS_TO_PRETTY( GREEN, SKIP,    returnString );
                default:
                    returnString = "Invalid value string for GREEN";
            }
            break;
            
        case BLUE:
            switch( eValue )
            {
                CASE_ENUMS_TO_PRETTY( BLUE, ZERO,    returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, ONE,     returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, TWO,     returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, THREE,   returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, FOUR,    returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, FIVE,    returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, SIX,     returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, SEVEN,   returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, EIGHT,   returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, NINE,    returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, DRAWTWO, returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, REVERSE, returnString );
                CASE_ENUMS_TO_PRETTY( BLUE, SKIP,    returnString );
                default:
                    returnString = "Invalid value string for BLUE";
            }
            break;
            
        case YELLOW:
            switch( eValue )
            {
                CASE_ENUMS_TO_PRETTY( YELLOW, ZERO,    returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, ONE,     returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, TWO,     returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, THREE,   returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, FOUR,    returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, FIVE,    returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, SIX,     returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, SEVEN,   returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, EIGHT,   returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, NINE,    returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, DRAWTWO, returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, REVERSE, returnString );
                CASE_ENUMS_TO_PRETTY( YELLOW, SKIP,    returnString );
                default:
                    returnString = "Invalid value string for YELLOW";
            }
            break;
            
        case WILD:
			switch( eValue )
            {
				case WILDDRAWFOUR:
					returnString =  CARD_COLOUR_YELLOW "DR" \
									CARD_COLOUR_BLUE "AW" \
									CARD_COLOUR_RED " FO" \
									CARD_COLOUR_GREEN "UR" \
									CARD_COLOUR_RESET;
					break;
					
				case WILDCARD:
					returnString =  CARD_COLOUR_YELLOW "W" \
									CARD_COLOUR_BLUE "I" \
									CARD_COLOUR_RED "L" \
									CARD_COLOUR_GREEN "D" \
									CARD_COLOUR_RESET;
					break;
					
				default:
                    returnString = "Invalid value string for WILD";
			}
            break;
            
        default:
            returnString = "Invalid card suit";
    }
    
    return returnString;
}