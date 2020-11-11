/* ----------------------------------------------
 	IFJ prekladac jazyka IFJ20
	Tým XX:
		Jaksik, Ales (xjaksi01)
		Vlasakova, Nela (xvlasa14)
		Belohlavek, Jan (xbeloh8)
-------------------------------------------------
Ucel: rozpoznani a klasifikace lexemu, reprezentace lexemu pomoci tokenu
Popis:  jak funguje tento soubor
----------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynamicString.h"

#include "tokenList.h" // protototypy tokenlistu, tokenu, tokentype
#include "scanner.h"
#include "errors.h"



void getTokensTo(tokenList *tListMainPtr){ //fuknce pro precteni dat ze std. vstupu a ulozeni do seznamu , DKA
	DLInitList (tListMainPtr); //// nastavime vsechny pointry na NULL
	tStr teststr1;
	str_Init(&teststr1);
	str_Append(&teststr1, "a" ); ///test pridani a
	str_Append(&teststr1, "b" ); ///test pridani a
	DLInsertLast(tListMainPtr, tID, &teststr1) /// pridani tokenu do tokenlistu z atributu funkce

	return 0;
}
