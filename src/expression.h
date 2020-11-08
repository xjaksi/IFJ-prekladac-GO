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
#include "scanner.h"
#define PT_SIZE 6

/**
 * @brief typy symbolu v precedencni tabulce
 */
typedef enum {
    
    PT_ADDSUB,
    PT_MULDIV,
    PT_CMPS,
    PT_LBR,
    PT_RBR,
    PT_STOP,
    PT_EXP,
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

ERROR_CODE parseExp();


/**
 * @brief funkce projde seznam tokenu a z toho naplni globalni seznam
 * 
 * @param t seznam tokenu
 * @return ERROR_CODE pokud se neco nepovede, vrati chybovou hodnotu
 */
ERROR_CODE fillMyList(tList *t);

void shift();

ERROR_CODE reduce();