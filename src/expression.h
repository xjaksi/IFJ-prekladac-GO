/** -----------------------------------------------
 * @file expression.h
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Vlasakova, Nela <xvlasa14>
 * ------------------------------------------------
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief struktury, definice funkci
 * -----------------------------------------------*/ 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


#include "errors.h"
#define PT_SIZE 6

/**
 * @brief typy symbolu v precedencni tabulce
 */
typedef enum {
    PT_PLUS,
    PT_MINUS,
    PT_TIMES,
    PT_DIV,
    PT_ID,
    PT_EXP,
    PT_INT,
    PT_STRING,
} PtType;

/**
 * @brief datove typy 
 */
typedef enum {
    DT_INT,
    DT_STRING,
    DT_FLOAT,
    DT_NONE,
} DataType;

void printStuff();

int tokenToSymbol (int tokenTemp);
