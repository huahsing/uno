
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deck.h"
#include "game.h"


#define DIR_CLOCKWISE 1
#define DIR_ANTICLOCKWISE 2

typedef struct
{
    int playerCnt;
    Deck_t drawPile;
    Deck_t discardPile;
    Deck_t *playerHands;
    Deck_t lastPlayedCards;
    char **playerNames;
    BOOL_t * unoStatus;
    
    int currentTurn;
    int currentPlayer;
    int gameDirection;
    CardSuit_e currentSuit;
    int numOfCardsToDrawForCurrentPlayer;
    int previousPlayer;
    BOOL_t skip;
    
    BOOL_t gameWon;
} GameData_t;

typedef enum 
{
    GAMECHOICE_NULL,
    GAMECHOICE_VIEWALLGAMESTATE,
    GAMECHOICE_PASSTURN,
    GAMECHOICE_PLAYCARDS,
    GAMECHOICE_QUIT
} E_GAMECHOICES;

static GameData_t gameData;
static BOOL_t gameCreated = BOOL_FALSE;
static BOOL_t gameStarted = BOOL_FALSE;

#define SCAN_BUF_SIZE 1024
static char scanBuf[SCAN_BUF_SIZE];


// prototypes
static void dealCards();
static void displayLastPlayedCards();
static void displayTopCardInDiscardPile();
static void displayGameMenu();
static void displayAllGameState();
static void displayNormalGameState();
static void displayMessageToCurrentPlayer(int numberOfPlayableCards);
static void displayPlayersAndTurnsState();
static int displayCurrentPlayerHand();
static void drawCardsForCurrentPlayer(int count);
static BOOL_t isALegalMatch(Card_t bottom, Card_t top);
static BOOL_t playCardsForCurrentPlayer();
static void nextTurn();
static void resolvePlay(Card_t playedCard);

BOOL_t game_CreateNewGame()
{
    char *c;
    memset( &gameData, 0, sizeof(GameData_t) );
    
    printf("Creating new game. Enter number of players (2-10): " );
    
    do 
    {
        fgets( scanBuf, SCAN_BUF_SIZE, stdin );
        gameData.playerCnt = strtol( scanBuf, &c, 10 );
        
        if( gameData.playerCnt < 2 || gameData.playerCnt > 10 )
            printf( "Number of players must be between 2-10: ");
    } while (gameData.playerCnt < 2 || gameData.playerCnt > 10);
        
    if( gameData.playerCnt == 0 )
        return BOOL_FALSE;
    else
    {
        int i, nameSz;
        
        printf( "Number of players = %d\n", gameData.playerCnt );
        
        gameData.playerNames = (char**) malloc( sizeof(char*) * gameData.playerCnt );
        gameData.playerHands = (Deck_t*) malloc( sizeof(Deck_t) * gameData.playerCnt );
        gameData.unoStatus = (BOOL_t*) malloc( sizeof(BOOL_t) *  gameData.playerCnt );
        
        for( i = 0; i < gameData.playerCnt; i++ )
        {
            printf("Enter name of player %d: ", i+1);
            fgets( scanBuf, SCAN_BUF_SIZE, stdin );
            nameSz = (strlen(scanBuf));
            gameData.playerNames[i] = (char*) malloc( sizeof(char) * nameSz );
            strncpy( gameData.playerNames[i], scanBuf, nameSz-1 );
            gameData.playerNames[i][nameSz-1] = '\0'; // remove newline character and terminate it properly
            
            gameData.playerHands[i] = deck_InitEmptyDeck();
            gameData.unoStatus[i] = BOOL_FALSE;
        }
        
        printf( "The order of players are:\n");
        for( i = 0; i < gameData.playerCnt; i++ )
        {
            printf( "%2d: %s\n", i+1, gameData.playerNames[i] );
        }
        
        gameData.drawPile = deck_InitDeck();
        deck_ShuffleDeck( &gameData.drawPile );
        gameData.discardPile = deck_InitEmptyDeck();
        gameData.lastPlayedCards = deck_InitEmptyDeck();
        
        gameData.currentTurn = 0;
        gameData.currentPlayer = 0;
        gameData.gameDirection = DIR_CLOCKWISE;
        gameData.previousPlayer = -1;
        gameData.numOfCardsToDrawForCurrentPlayer = 0;
        gameData.skip = BOOL_FALSE;
        gameData.gameWon = BOOL_FALSE;
        
        
        dealCards();
        printf( "Cards have been dealt, ready to start game.\n" );
        
        // cannot start with WILD card
        while( deck_PeekTop( &gameData.drawPile ).eSuit == WILD )
            deck_ShuffleDeck( &gameData.drawPile );
        
        Card_t aCard = deck_PopCard( &gameData.drawPile );
        gameData.currentSuit = aCard.eSuit;
        deck_PushCard( &gameData.discardPile, aCard );
        
        return (gameCreated = BOOL_TRUE);
    }
}


BOOL_t game_DestroyGame()
{
    int i;
    
    if( gameCreated )
    {
        if( gameData.playerNames )
        {
           for( i = 0; i < gameData.playerCnt; i++ )
           {
               if( gameData.playerNames[i] )
                   free( gameData.playerNames[i] );
           }
           
           free( gameData.playerNames );
        }
        
        memset( &gameData, 0, sizeof(GameData_t) );
    }
    gameCreated = BOOL_FALSE;
    return BOOL_TRUE;
}

void dealCards()
{
    int i, j;
    Card_t aCard;
    
    for( i = 0; i < 7; i++ )
    {
        for( j = 0; j < gameData.playerCnt; j++ )
        {
            aCard = deck_PopCard( &gameData.drawPile );
            deck_PushCard( &gameData.playerHands[j], aCard );
        }
    }
}

void displayLastPlayedCards()
{
    if( !deck_IsEmpty( &gameData.lastPlayedCards ) )
    {
        printf( "Last played cards by %s:\n", gameData.previousPlayer != -1 ? gameData.playerNames[gameData.previousPlayer] : "");
        deck_PrintDeck(&gameData.lastPlayedCards);
    }
}

void displayTopCardInDiscardPile()
{
    if( !deck_IsEmpty( &gameData.discardPile ) )
    {
        Card_t topCard = deck_PeekTop( &gameData.discardPile );
        printf( ANSI_COLOUR_GREEN "[Discard Pile] Top Card: " ANSI_COLOUR_RESET);
        printf( "%s\n", card_GetPrettyPrint( topCard.eSuit, topCard.eValue ) );
    }
}

void displayGameMenu()
{
    printf( "\n********************\n"
            "   Game Menu\n"
            "--------------------\n"
            "%d. View All Game State\n"
            "%d. Pass Turn\n"
            "%d. Play Card\n"
            "%d. Quit Game\n"
            "********************\n",
            GAMECHOICE_VIEWALLGAMESTATE,
            GAMECHOICE_PASSTURN,
            GAMECHOICE_PLAYCARDS,
            GAMECHOICE_QUIT );
}

void displayAllGameState()
{
    int i;
    CLEAR_SCREEN();
    printf("SHOWING ALL GAME STATE INFORMATION\n");
    printf("########## DRAW PILE ##########\n");
    deck_PrintDeck( &gameData.drawPile );
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    printf("\n########## DISCARD PILE ##########\n");
    deck_PrintDeck( &gameData.discardPile );
    printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    
    for( i = 0; i < gameData.playerCnt; i++ )
    {
        printf("\n########## %s's HAND ##########\n", gameData.playerNames[i]);
        deck_PrintDeck( &gameData.playerHands[i] );
        printf("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    }
    printf("\n");
    displayLastPlayedCards();
    displayTopCardInDiscardPile();
    displayPlayersAndTurnsState();
    
    printf(ANSI_COLOUR_CYAN "Press 'Enter' to get back to normal view\n" ANSI_COLOUR_RESET);
    fgets( scanBuf, SCAN_BUF_SIZE, stdin );
    displayNormalGameState();
}

void displayNormalGameState()
{
    int numOfPlayableCards;
    CLEAR_SCREEN();
    displayLastPlayedCards();
    displayTopCardInDiscardPile();
    displayPlayersAndTurnsState();
    numOfPlayableCards = displayCurrentPlayerHand();
    displayMessageToCurrentPlayer(numOfPlayableCards);
    displayGameMenu();
}

void displayMessageToCurrentPlayer(int numberOfPlayableCards)
{
}

void displayPlayersAndTurnsState()
{
    printf( ANSI_COLOUR_MAGENTA "Current Turn: " ANSI_COLOUR_RESET "%d\n" , gameData.currentTurn );
    printf( ANSI_COLOUR_MAGENTA "Current Player: " ANSI_COLOUR_RESET "%s\n", gameData.playerNames[gameData.currentPlayer]);
    printf( ANSI_COLOUR_MAGENTA "Current active suit: " ANSI_COLOUR_RESET "%s\n" ANSI_COLOUR_RESET, 
            card_GetColouredSuitString( gameData.currentSuit ) );
    printf( ANSI_COLOUR_MAGENTA "Next Player: " ANSI_COLOUR_RESET );
    
    int idx, i;
    if( gameData.gameDirection == DIR_CLOCKWISE )
    {
        idx = (gameData.currentPlayer + 1) % gameData.playerCnt;
    }
    else
    {
        if( gameData.currentPlayer == 0 ) 
            idx = gameData.playerCnt - 1;
        else
            idx = gameData.currentPlayer - 1;
    }
    
    printf( ANSI_COLOUR_CYAN "%s\n" ANSI_COLOUR_RESET, gameData.playerNames[idx] );
    
    printf( ANSI_COLOUR_MAGENTA "Player Order (%s): " ANSI_COLOUR_RESET, gameData.gameDirection == DIR_CLOCKWISE ? "clockwise" : "anti-clockwise");
    idx = gameData.currentPlayer; 
    for( i = 0; i < gameData.playerCnt; i++ )
    {
        printf( ANSI_COLOUR_CYAN "%s, " ANSI_COLOUR_RESET, gameData.playerNames[idx] );
        
        if( gameData.gameDirection == DIR_CLOCKWISE )
        {
            idx = (idx + 1) % gameData.playerCnt;
        }
        else
        {
            if( idx == 0 ) 
                idx = gameData.playerCnt - 1;
            else
                idx = idx - 1;
        }
    }
    printf("\n");
    
    printf( ANSI_COLOUR_MAGENTA "Players in Uno: " ANSI_COLOUR_RESET );
    int unoflag = 0;
    for( i = 0; i < gameData.playerCnt; i++ )
    {
        if( gameData.playerHands[i].cardCount == 1 )
        {
            unoflag = 1;
            printf( ANSI_COLOUR_CYAN "%s, " ANSI_COLOUR_RESET, gameData.playerNames[i] );
        }
    }
    
    if( unoflag == 0 ) printf( "None" );
    
    printf( "\n" );
}

int displayCurrentPlayerHand()
{
    printf("\n>>>>>>>>>>>>>>>>>>>>\n");
    printf( ANSI_COLOUR_MAGENTA "%s " ANSI_COLOUR_GREEN ", your current hand is:\n" ANSI_COLOUR_RESET, 
            gameData.playerNames[gameData.currentPlayer] );
    deck_PrintDeck( &gameData.playerHands[gameData.currentPlayer] );
    printf("<<<<<<<<<<<<<<<<<<<<\n");
}

void drawCardsForCurrentPlayer(int count)
{
    int i;
    Card_t aCard;
    for( i = 0; i < count; i++ )
    {
        if( deck_IsEmpty( &gameData.drawPile ) )
        {
            aCard = deck_PopCard( &gameData.discardPile );
            gameData.drawPile = gameData.discardPile;
            deck_ShuffleDeck( &gameData.drawPile );
            gameData.drawPile = deck_InitEmptyDeck();
            deck_PushCard( &gameData.drawPile, aCard );
        }
        
        aCard = deck_PopCard( &gameData.drawPile );
        deck_PushCard( &gameData.playerHands[gameData.currentPlayer], aCard );
    }
    
    printf( ANSI_COLOUR_GREEN "%s has just drawn %d %s, press 'Enter' to continue...\n" ANSI_COLOUR_RESET, 
            gameData.playerNames[gameData.currentPlayer], count, count == 1 ? "card" : "cards" );
            
    fgets( scanBuf, SCAN_BUF_SIZE, stdin );
}

BOOL_t isALegalMatch(Card_t bottom, Card_t top)
{
    BOOL_t retFlag = BOOL_FALSE;
    if( top.eSuit == WILD || 
        (bottom.eSuit == WILD && gameData.currentSuit == top.eSuit) ||
        (bottom.eSuit != WILD && (bottom.eSuit == top.eSuit || bottom.eValue == top.eValue) ) )
        retFlag = BOOL_TRUE;
    
    return retFlag;
}

BOOL_t playCardsForCurrentPlayer()
{
    Deck_t playableCards = deck_InitEmptyDeck();
    int i, choice = -1;
    Card_t aCard;
    char *c;
    int currentIdx = 0;
    
    while( currentIdx < gameData.playerHands[gameData.currentPlayer].cardCount )
    {
        if( isALegalMatch( deck_PeekTop( &gameData.discardPile ), gameData.playerHands[gameData.currentPlayer].cards[currentIdx] ) )
        {
            aCard = deck_RemoveCardAt( &gameData.playerHands[gameData.currentPlayer], currentIdx );
            deck_PushCard( &playableCards, aCard );
        }
        else
            currentIdx++;
    }
    
    if( playableCards.cardCount == 0 )
    {
        printf( "Sorry, you have no legal cards to play. Press 'Enter' to go back to show game state and menu...\n" );
        fgets( scanBuf, SCAN_BUF_SIZE, stdin );
        return BOOL_FALSE;
    }
    else
    {
        printf( "%s, you can play the following cards from your hand:\n", gameData.playerNames[gameData.currentPlayer] );
        deck_PrintDeck( &playableCards );
        int choiceCnt = 0;
CHOOSE_CARD_TO_PLAY:
        do
        {
            printf("%snter your choice: ", choiceCnt ? "Invalid choice, e" : "E");
            fgets( scanBuf, SCAN_BUF_SIZE, stdin );
            choice = strtol( scanBuf, &c, 10 );
            choiceCnt++; 
        } while( choice <= 0 || choice > playableCards.cardCount );

        printf( "You have chosen to play %s, are you sure? (y/n)\n", 
                card_GetPrettyPrint(
                    playableCards.cards[choice-1].eSuit,
                    playableCards.cards[choice-1].eValue
                ) );
                
        fgets( scanBuf, SCAN_BUF_SIZE, stdin );
        if( (scanBuf[0] == 'y' || scanBuf[0] == 'Y' ) && scanBuf[1] == '\n')
        {
            // play card into discard pile
            aCard = deck_RemoveCardAt( &playableCards, choice-1 );
            deck_PushCard( &gameData.discardPile, aCard );
            gameData.lastPlayedCards = deck_InitEmptyDeck();
            deck_PushCard( &gameData.lastPlayedCards, aCard );
            // reinsert rest of playable cards back into hand
            while( !deck_IsEmpty( &playableCards ) )
            {
                aCard = deck_PopCard( &playableCards );
                deck_PushCard( &gameData.playerHands[gameData.currentPlayer], aCard );
            }
            
            if( gameData.playerHands[gameData.currentPlayer].cardCount == 0 )
            {
                printf( ANSI_COLOUR_CYAN "%s, CONGRATULATIONS, you have won the game!! Press 'Enter' to continue...\n" \
                        ANSI_COLOUR_RESET, 
                        gameData.playerNames[gameData.currentPlayer] );
                gameData.gameWon = BOOL_TRUE;
                fgets( scanBuf, SCAN_BUF_SIZE, stdin );
                return BOOL_TRUE;
            }
            
            if( gameData.playerHands[gameData.currentPlayer].cardCount == 1 )
            {
                printf( ANSI_COLOUR_CYAN "%s, you are in UNO, press 'Enter' to continue...\n" ANSI_COLOUR_RESET, 
                        gameData.playerNames[gameData.currentPlayer]);
                fgets( scanBuf, SCAN_BUF_SIZE, stdin );
            }
            
            resolvePlay( deck_PeekTop( &gameData.discardPile ) );
        }
        else
        {
            choiceCnt = 0;
            goto CHOOSE_CARD_TO_PLAY;
        }
        
        return BOOL_TRUE;
    }
}

void resolvePlay(Card_t playedCard)
{   
    if( playedCard.eSuit == WILD )
    {
        int choice = -1, choiceCnt = 0;
        char *c;
        printf("%s, you have played a WILD card, now choose the suit.\n", gameData.playerNames[gameData.currentPlayer]);
        
        do
        {
            printf( "[%d] %s\n"
                    "[%d] %s\n"
                    "[%d] %s\n"
                    "[%d] %s\n",
                    RED, card_GetSuitString(RED),
                    GREEN, card_GetSuitString(GREEN),
                    BLUE, card_GetSuitString(BLUE),
                    YELLOW, card_GetSuitString(YELLOW) );
                    
            printf("%snter your choice: ", choiceCnt ? "Invalid choice, e" : "E");
            fgets( scanBuf, SCAN_BUF_SIZE, stdin );
            choice = strtol( scanBuf, &c, 10 );
            choiceCnt++; 
        } while( choice <= 0 || choice > 4 );
        
        gameData.currentSuit = choice;
        printf( "Suit chosen is %s, press 'Enter' to continue...\n", card_GetColouredSuitString(choice) );
        fgets( scanBuf, SCAN_BUF_SIZE, stdin );
    }
    else
        gameData.currentSuit = playedCard.eSuit;
    
    if( playedCard.eValue == DRAWTWO )
        gameData.numOfCardsToDrawForCurrentPlayer = 2;
    
    if( playedCard.eValue == WILDDRAWFOUR )
        gameData.numOfCardsToDrawForCurrentPlayer = 4;
    
    if( playedCard.eValue == SKIP )
    {
        gameData.skip = BOOL_TRUE;
    }
    
    if( playedCard.eValue == REVERSE )
    {
        if( gameData.gameDirection == DIR_CLOCKWISE )
        {
            gameData.gameDirection = DIR_ANTICLOCKWISE;
            printf( ANSI_COLOUR_GREEN "Direction changed from CLOCKWISE to ANTI-CLOCKWISE\n" ANSI_COLOUR_RESET );
        }
        else
        {
            gameData.gameDirection = DIR_CLOCKWISE;
            printf( ANSI_COLOUR_GREEN "Direction changed from ANTI-CLOCKWISE to CLOCKWISE\n" ANSI_COLOUR_RESET );
        }
    }
}

void nextTurn()
{
    gameData.currentTurn++;
    gameData.previousPlayer = gameData.currentPlayer;
    if( gameData.gameDirection == DIR_CLOCKWISE )
    {
        if( gameData.skip )
        {
            gameData.currentPlayer = (gameData.currentPlayer + 1) % gameData.playerCnt;
            printf( ANSI_COLOUR_GREEN "Skipping %s's turn, press 'Enter' to continue...\n" ANSI_COLOUR_RESET, 
                    gameData.playerNames[gameData.currentPlayer] );
            gameData.currentPlayer = (gameData.currentPlayer + 1) % gameData.playerCnt;
            gameData.skip = BOOL_FALSE;
            fgets( scanBuf, SCAN_BUF_SIZE, stdin );
        }
        else
        {
            gameData.currentPlayer = (gameData.currentPlayer + 1) % gameData.playerCnt;
        }
    }
    else
    {
        if( gameData.skip )
        {
            if( gameData.currentPlayer == 0 ) 
                gameData.currentPlayer = gameData.playerCnt - 1;
            else
                gameData.currentPlayer = gameData.currentPlayer - 1;
            
            printf( ANSI_COLOUR_GREEN "Skipping %s's turn, press 'Enter' to continue...\n" ANSI_COLOUR_RESET, 
                    gameData.playerNames[gameData.currentPlayer] );
            
            if( gameData.currentPlayer == 0 ) 
                gameData.currentPlayer = gameData.playerCnt - 1;
            else
                gameData.currentPlayer = gameData.currentPlayer - 1;
            
            gameData.skip = BOOL_FALSE;
            fgets( scanBuf, SCAN_BUF_SIZE, stdin );
        }
        else
        {
            if( gameData.currentPlayer == 0 ) 
                gameData.currentPlayer = gameData.playerCnt - 1;
            else
                gameData.currentPlayer = gameData.currentPlayer - 1;
        }
    }
}

void game_RunGame()
{
    int choice = -1;
    char *c;
    
    gameData.currentTurn = 1;
    
    if( deck_PeekTop( &gameData.discardPile ).eValue == SKIP )
    {
        printf( "First card is SKIP, so we are skipping %s's turn\n", gameData.playerNames[0] );
        gameData.currentPlayer = 1;
        
        printf("Press any 'Enter' to continue...\n" );
        fgets( scanBuf, SCAN_BUF_SIZE, stdin );
    }
    
    if( deck_PeekTop( &gameData.discardPile ).eValue == REVERSE )
    {
        printf( "First card is REVERSE, direction is changed to anti-clockwise,"
                " so after %s's turn, it will be %s's turn\n", gameData.playerNames[0], gameData.playerNames[gameData.playerCnt-1]);
        gameData.gameDirection = DIR_ANTICLOCKWISE;
        
        printf("Press 'Enter' to continue...\n" );
        fgets( scanBuf, SCAN_BUF_SIZE, stdin );
    }
    
    if( deck_PeekTop( &gameData.discardPile ).eValue == DRAWTWO )
        gameData.numOfCardsToDrawForCurrentPlayer = 2;
    
    while( choice != GAMECHOICE_QUIT )
    {
        if( gameData.gameWon )
            break;
        
        if( gameData.numOfCardsToDrawForCurrentPlayer != 0 )
        {
            drawCardsForCurrentPlayer(gameData.numOfCardsToDrawForCurrentPlayer);
            printf( ANSI_COLOUR_CYAN "%s drew %d cards\n" ANSI_COLOUR_RESET, 
                    gameData.playerNames[gameData.currentPlayer],
                    gameData.numOfCardsToDrawForCurrentPlayer );
            gameData.numOfCardsToDrawForCurrentPlayer = 0;
        }
        displayNormalGameState();
        fgets( scanBuf, SCAN_BUF_SIZE, stdin );
        choice = strtol( scanBuf, &c, 10 );
        
        switch( choice )
        {
            case GAMECHOICE_VIEWALLGAMESTATE:
                displayAllGameState();
                break;
                
            case GAMECHOICE_PASSTURN:
                drawCardsForCurrentPlayer(1);
                printf( ANSI_COLOUR_CYAN "%s has passed, press 'Enter' to continue..." ANSI_COLOUR_RESET, 
                        gameData.playerNames[gameData.currentPlayer] );
                fgets( scanBuf, SCAN_BUF_SIZE, stdin );
                nextTurn();
                break;
                
            case GAMECHOICE_PLAYCARDS:
                if( playCardsForCurrentPlayer() )
                {
                    printf( ANSI_COLOUR_CYAN "%s is done playing his card, press 'Enter' to continue..." ANSI_COLOUR_RESET,
                            gameData.playerNames[gameData.currentPlayer] );
                    fgets( scanBuf, SCAN_BUF_SIZE, stdin );
                    nextTurn();
                }
                break;
                
            case GAMECHOICE_QUIT:
                printf("You have chosen to quit game, are you sure?(y/n): ");
                fgets( scanBuf, SCAN_BUF_SIZE, stdin );
                if( (scanBuf[0] == 'y' || scanBuf[0] == 'Y' ) && scanBuf[1] == '\n')
                {
                    printf("Quit confirmed, back to main menu...\n");
                }
                else
                {
                    printf("Quit canceled, back to game menu...\n");
                    choice = -1;
                }
                break;
                
            default:
                printf("Invalid choice\n");
        }
    }
}