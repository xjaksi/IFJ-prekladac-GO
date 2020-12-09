/** -----------------------------------------------
 * @file scanner.h
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Belohlavek, Jan <xbeloh08>
 * ------------------------------------------------
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief hlavickovy soubor pro scanner.c
 * -----------------------------------------------*/
#include <stdlib.h>
#include "tokenList.h"

// TokenType - definovan v tokenList.h
// *TokenPtr definovan v tokenList.h
// tokenList definovan v tokenList.h

//prototypy funkci

int getTokensTo(tokenList *tListMain);
