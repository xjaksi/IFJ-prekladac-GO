/** -----------------------------------------------
 * @file main.c
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief Tady vse zacina a konci MAIN
 * -----------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"


int main() {
    
    int result = parse();

    switch (result)
    {
    case ERROR_LEXICAL:
        fprintf(stderr, "[ERROR] lexical error.\n");
        break;
    
    case OK:
        fprintf(stderr, "[OK] everything is ok.\n");
        break;
    
    case ERROR_SYNTAX:
        fprintf(stderr, "[ERROR] syntax error.\n");
        break;

    default:
    fprintf(stderr, "[ERROR] something went wrong.\n");
        break;
    }

    return result;
}