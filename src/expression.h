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
#include "tokenList.h"
#define PT_SIZE 6

/**
 * @brief typy symbolu v precedencni tabulce
 */
typedef enum {
    
    PT_ADDSUB,  // 0
    PT_MULDIV,  // 1
    PT_CMPS,    // 2
    PT_LBR,     // 3
    PT_RBR,     // 4
    PT_STOP,    // 5
    PT_EXP,     // 6
    PT_CONST,   
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

ERROR_CODE parseExp(/*tokenList *tList*/);

void shift();

ERROR_CODE reduce();

ERROR_CODE accept();