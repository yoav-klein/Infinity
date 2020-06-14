/*******************************************************************************
* Programmer: Eldad Carin
* Reviewer: 
* File: DEFS.H
*******************************************************************************/
/*Print Colors*****************************************************************/
#define BLUE      "\x1b[34m"
#define RED       "\x1b[31m"
#define REDL      "\x1b[91m"
#define YELLOW    "\x1b[1;33m" 
#define YELLOW2   "\x1b[33m"   
#define CYANDARK  "\x1b[36m"
#define CYAN      "\x1b[36m\x1b[1m"
#define GREEN     "\x1B[32m"
#define GREENL    "\x1b[92m"
#define MAGENTA   "\x1B[35m"
#define ORANGE    "\x1b[33m"
#define WHITE     "\x1B[37m"

#define BLINKON   "\x1b[5m"
#define BLINKOFF  "\x1b[25m"
#define BOLDON    "\x1b[1m"
#define BOLDOFF   "\x1b[21m"
#define UNDERLON  "\x1b[4m"
#define UNDERLOFF "\x1b[24m"

#define MINTBCK     "\x1b[46m"
#define PURPBCK     "\x1b[45m"
#define BLUEBCK     "\x1b[44m"
#define LEMONBCK    "\x1b[43m"
#define LIMEBCK     "\x1b[42m"

#define RESET     "\x1b[0m"

/***************** DEBUG *********************/

#ifndef NDEBUG
#include <iostream>
#define DEBUG_ONLY(code) code
#else
#define DEBUG_ONLY(code)
#endif
