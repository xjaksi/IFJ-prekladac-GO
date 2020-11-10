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

    return result;
}