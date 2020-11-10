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

#include "scanner.h"
#include "errors.h"

int main(){

	tListPtr tListMain = initScanner();// vytvoří obousměrné vázány list pro tokeny

	get_token(tListMain); // naplni obousmerne vazany seznam tokeny z stdin
	//printf("%s\n", tListMain->act->value);
return 0;
}



tListPtr initScanner(){

	tListPtr tListMain = malloc(sizeof(struct tList)); // hlavni list
	tListMain->first = NULL;
	tListMain->act = NULL;
	tListMain->last = NULL;
	tListMain->numberTokensInList = 0;

	return tListMain;
}

void tListAdd(tListPtr atr, TokenType type, char valu[]){

	TokenPtr newItem = malloc(sizeof(struct Token));//zalozi novy prvek listu
  	if (newItem == NULL) {
    	return ERROR_COMPILER;
   	}


	if(atr->numberTokensInList == 0){
		atr->first = newItem;
		atr->act = newItem;
		atr->last = newItem;
		atr->numberTokensInList++;
		newItem->type = type;//uloži do nej hodnoty
		newItem->value[1000000] = valu;
		newItem->next = NULL;
		newItem->prev = NULL;
		return 0;
	}



	newItem->prev = atr->act; // nastaveni predchoziho prvku pro momentalni prvek
	atr->act->next = newItem; // nastaveni nasledujiciho prvku predchozimu prvku
	newItem->next = NULL; // nastaveni nasledujiciho prvku
	atr->act = newItem; //aktualizace act prvku v seznamu 
	atr->last = newItem; // TODO: dat to jednou na konec 
	atr->numberTokensInList++;
	newItem->type =type;//uloži do nej hodnoty
	newItem->value[1000000] = valu;

	return 0;
}



void get_token(tListPtr tListMain){ //fuknce pro precteni dat ze std. vstupu a ulozeni do seznamu , DKA

	char tmpString[] = "";

	char helpString = "a";// v kazdem pruchodu getc dostanu novy
	strcat (tmpString, helpString);



	printf("%s\n", tmpString);
		
	tListAdd(tListMain, tID, tmpString);

	//ulozit do oboustranne vazaneho seznamu



return 0;
}
