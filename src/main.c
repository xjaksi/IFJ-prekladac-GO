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

    case 3:
        fprintf(stderr, "[ERROR] 3\n");
        break;

    case 4:
        fprintf(stderr, "[ERROR] 4\n");
        break;

    case 5:
        fprintf(stderr, "[ERROR] 5\n");
        break;

    case 6:
        fprintf(stderr, "[ERROR] 6\n");
        break;

    case 7:
        fprintf(stderr, "[ERROR] 7\n");
        break;

    case 9:
        fprintf(stderr, "[ERROR] 9\n");
        break;

    case 99:
        fprintf(stderr, "[ERROR] 99\n");
        break;

    default:
    fprintf(stderr, "[ERROR] something went wrong.\n");
        break;
    }

    return result;
}