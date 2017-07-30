
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
    
    int previousPlayer;
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
static void displayPlayersAndTurnsState();
static void displayCurrentPlayerHand();
static void drawCardsForCurrentPlayer(int count);
static void nextTurn();

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
        
        
        dealCards();
        printf( "Cards have been dealt, ready to start game.\n" );
        
        Card_t aCard = deck_PopCard( &gameData.drawPile );
        deck_PushCard( &gameData.discardPile, aCard );
        
        gameCreated = BOOL_TRUE;
        
        return BOOL_TRUE;
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
        printf( "[Discard Pile] Top Card: ");
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
            "%d. Play Cards\n"
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
    
    printf("Press 'Enter' to get back to normal view\n");
    fgets( scanBuf, SCAN_BUF_SIZE, stdin );
    displayNormalGameState();
}

void displayNormalGameState()
{
    CLEAR_SCREEN();
    displayLastPlayedCards();
    displayTopCardInDiscardPile();
    displayPlayersAndTurnsState();
    displayCurrentPlayerHand();
    displayGameMenu();
}

void displayPlayersAndTurnsState()
{
    printf( "Current Turn: %d\n", gameData.currentTurn );
    printf( "Current Player: %s\n", gameData.playerNames[gameData.currentPlayer]);
    printf( "Next Player: " );
    
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
    
    printf( "%s\n", gameData.playerNames[idx] );
    
    printf( "Player Order (%s): ", gameData.gameDirection == DIR_CLOCKWISE ? "clockwise" : "anti-clockwise");
    idx = gameData.currentPlayer; 
    for( i = 0; i < gameData.playerCnt; i++ )
    {
        printf( "%s, ", gameData.playerNames[idx] );
        
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
    
    printf( "Players in Uno: " );
    int unoflag = 0;
    for( i = 0; i < gameData.playerCnt; i++ )
    {
        if( gameData.unoStatus[i] )
        {
            unoflag = 1;
            printf( "%s, ", gameData.playerNames[i] );
        }
    }
    
    if( unoflag == 0 ) printf( "None" );
    
    printf( "\n" );
}

void displayCurrentPlayerHand()
{
    printf("\n>>>>>>>>>>>>>>>>>>>>\n");
    printf("%s, your current hand is:\n", gameData.playerNames[gameData.currentPlayer] );
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
    
    printf( "%s has just drawn %d %s\n", gameData.playerNames[gameData.currentPlayer], count, count == 1 ? "card" : "cards" );
}

void nextTurn()
{
    gameData.currentTurn++;
    
    if( gameData.gameDirection == DIR_CLOCKWISE )
    {
        gameData.currentPlayer = (gameData.currentPlayer + 1) % gameData.playerCnt;
    }
    else
    {
        if( gameData.currentPlayer == 0 ) 
            gameData.currentPlayer = gameData.playerCnt - 1;
        else
            gameData.currentPlayer = gameData.currentPlayer - 1;
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
    
    while( choice != GAMECHOICE_QUIT )
    {
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
                printf( "%s has passed, press 'Enter' to continue...", gameData.playerNames[gameData.currentPlayer] );
                fgets( scanBuf, SCAN_BUF_SIZE, stdin );
                nextTurn();
                break;
                
            case GAMECHOICE_PLAYCARDS:
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