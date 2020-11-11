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

#inslude "tokenList.c" // funkce pro praci s tokenlistem
#inslude "tokenList.c" // protototypy tokenlistu, tokenu, tokentype
#include "scanner.h"
#include "errors.h"

int main(){

	tListPtr tListMain = initTokensList();// vytvoří obousměrné vázány list pro tokeny

	getTokensTo(tListMain); // naplni obousmerne vazany seznam tokeny z stdin
	//printf("%s\n", tListMain->act->value);
return 0;
}


tListPtr initTokensList(){

	tListPtr tListMain = malloc(sizeof(struct tList)); // hlavni list
	tListMain->first = NULL;
	tListMain->act = NULL;
	tListMain->last = NULL;
	tListMain->numberTokensInList = 0;

	return tListMain;
}

void getTokensTo(tListPtr tListMain){ //fuknce pro precteni dat ze std. vstupu a ulozeni do seznamu , DKA

	char tmpString[] = "";

	char helpString = "a";// v kazdem pruchodu getc dostanu novy
	strcat (tmpString, helpString);



	printf("%s\n", tmpString);
		
	tListAdd(tListMain, tID, tmpString);//ulozit do oboustranne vazaneho seznamu

	return 0;
}
