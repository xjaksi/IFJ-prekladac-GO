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
#include <stdbool.h>


#include "errors.h"
#include "tokenList.h"
#include "dynamicString.h"
#define PT_SIZE 8

/**
 * @brief typy symbolu v precedencni tabulce
 */
typedef enum {
    
    PT_ADD,     // 0
    PT_SUB,     // 1
    PT_MUL,     // 2
    PT_DIV,     // 3
    PT_CMPS,    // 4
    PT_LBR,     // 5
    PT_RBR,     // 6
    PT_STOP,    // 7
    PT_EXP,     // 8
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
    DT_BOOL,
} DataType;

ERROR_CODE parseExp(tokenList *tList);

void shift();

ERROR_CODE reduce();

ERROR_CODE accept();