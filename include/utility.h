#ifndef __UTILITY_H__
#define __UTILITY_H__


#define ANSI_COLOUR_RED     "\x1b[31m"
#define ANSI_COLOUR_GREEN   "\x1b[32m"
#define ANSI_COLOUR_YELLOW  "\x1b[33m"
#define ANSI_COLOUR_BLUE    "\x1b[34m"
#define ANSI_COLOUR_MAGENTA "\x1b[35m"
#define ANSI_COLOUR_CYAN    "\x1b[36m"
#define ANSI_COLOUR_RESET   "\x1b[0m"

#define ANSI_COLOUR_BOLD    "\x1b[1m"
#define ANSI_COLOUR_LIGHTGREY_BG "\x1b[47m"


#define ANSI_CLEAR_SCREEN   "\x1b[2J"

#define BOOL_TRUE 1
#define BOOL_FALSE 0

typedef int BOOL_t;

#if 0
    #define CLEAR_SCREEN() printf( ANSI_CLEAR_SCREEN )
#else
    #define CLEAR_SCREEN() printf("\n\n##################################################\n\n");
#endif

#endif