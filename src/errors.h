/* ----------------------------------------------
	Předmět: IFJ
	Tým: 
		Vlasakova, Nela (xvlasa14)
        Jaksik, Ales (xjaksi01)
        Belohlavek, Honza
-------------------------------------------------
 Datum: 24.9. 2020
 Účel: Chyby
 Popis:  Seznam chyb
 ----------------------------------------------*/


#ifndef ERRORS_H
#define ERRORS_H

#include <stdlib.h>

typedef enum error_code{
    NOERROR_OK = 0,
    ERROR_LEX = 1,
    ERROR_SYN = 2,
    ERROR_SEM_VAR = 3,  // neefinovana funkce/promenna, pokus o redefinici
    ERROR_SEM_FUN = 4,  // chyba typove kompatibility
    ERROR_SEM_ARG = 5,  // spatny pocet argumentu
    ERROR_SEM = 6,
    ERROR_ZERO_DIV = 9,
    ERROR_COMPILATOR = 99,
} ERROR_CODE;

#endif
