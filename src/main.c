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
        fprintf(stderr, "[ERROR] lexical error. CODE: %d\n", result);
        break;
    
    case OK:
        fprintf(stderr, "[OK] everything is ok. CODE: %d\n", result);
        break;
    
    case ERROR_SYNTAX:
        fprintf(stderr, "[ERROR] syntax error. CODE: %d\n", result);
        break;

    case ERROR_SEMANTICS:
        fprintf(stderr, "[ERROR] semantic error. CODE: %d\n", result);
        break;

    case ERROR_UNDEFINED:
        fprintf(stderr, "[ERROR] semantic error - undefined or redefinition. CODE: %d\n", result);
        break;

    case ERROR_TYPE_INFERENCE:
        fprintf(stderr, "[ERROR] semantic error - type inference. CODE: %d\n", result);
        break;

    case ERROR_TYPE_COMPATIBILITY:
        fprintf(stderr, "[ERROR] semantic error - data type compatibility. CODE: %d\n", result);
        break;

    case ERROR_PARAMETERS:
        fprintf(stderr, "[ERROR] semantic error - wrong function parameters or return value. CODE: %d\n", result);
        break;

    case ERROR_ZERO_DIVISION:
        fprintf(stderr, "[ERROR] semantic error - division by zero. CODE: %d\n", result);
        break;

    default:
    fprintf(stderr, "[ERROR] something went wrong. CODE: %d\n", result);
        break;
    }

    printf("\n EXIT int@0");
    return result;
}