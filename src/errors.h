/** ------------------------------------------------
 * @file errors.h
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief chybove navratove kody
 * -----------------------------------------------*/ 


#ifndef ERRORS_H
#define ERRORS_H

#include <stdlib.h>

typedef enum error_code{
	OK = 0,
	ERROR_LEXICAL = 1,			// chyba v programu v ramci lexikalni analyzy
	ERROR_SYNTAX = 2,			// chyba v programu v ramci syntakticke analyzy
	ERROR_UNDEFINED = 3, 			// semanticka chyba, nedefinovana promenna nebo funkce
	ERROR_REDEFINITION = 3,			// semanticka chyba, redefinice promenne nebo funkce
	ERROR_TYPE_INFERENCE = 4,		// semanticka chyba pri odvozovani datového typu nove definovane promenne
	ERROR_TYPE_COMPATIBILITY = 5,		// semanticka chyba typove kompatibility ve vyrazech
	ERROR_PARAMETERS = 6,			// semanticka chyba poctu nebo typu parametru
	ERROR_RETURN_VALUE = 6, 		// semanticka chyba navratovych hodnot u funkci
	ERROR_SEMANTICS = 7,			// jine semanticke chyby
	ERROR_ZERO_DIVISION = 9,		// semanticka chyba deleni nulou
	ERROR_COMPILER = 99,			// interni chyba prekladace (alokace pameti, ...)
} ERROR_CODE;

#endif
