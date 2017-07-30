#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"

typedef enum 
{
    CHOICE_NULL,
    CHOICE_NEW_GAME,
    CHOICE_EXIT
} E_CHOICES;

#define SCAN_BUF_SIZE 1024
static char scanBuf[SCAN_BUF_SIZE];

void displayMainMenu()
{
    CLEAR_SCREEN();
    printf( "\n====================\n"
            "   Main Menu\n"
            "====================\n"
            "%d. Start New Game\n"
            "%d. Exit Program\n"
            "Choice? ",  CHOICE_NEW_GAME, CHOICE_EXIT  );
}


int main()
{
    int choice = -1;
    char *c = 0x0;
    
    while( choice != 2 )
    {
        displayMainMenu();
        fgets( scanBuf, SCAN_BUF_SIZE, stdin );
        choice = strtol( scanBuf, &c, 10 );
        
        switch( choice )
        {
            case CHOICE_NEW_GAME:
                if( !game_CreateNewGame() )
                    printf( "Game creation canceled\n");
                else
                {
                    printf("Game created, press 'Enter' to continue...");
                    fgets( scanBuf, SCAN_BUF_SIZE, stdin );
                    game_RunGame();
                }
                break;
            case CHOICE_EXIT:
                game_DestroyGame();
                printf("Exiting program...\n" );
                break;
            default:
                printf("Invalid choice.\n" );
        }
    }
	
    return 0;
}