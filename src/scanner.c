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
	printf("Hello world!!! \n");
	get_token();
	printf("%s\n", tListMain.act.value);
return 0;
}


tList* initScanner(){

	tList* tListMain = malloc(sizeof(tList)); // hlavni list
	tListMain.first = NULL;
	tListMain.act = NULL;
	tListMain.last = NULL;
	int numberTokensInList = 0;

	char tmpString[] = "";
	return 0;
}

void tListAdd(tList *atr){

	Token newItetem = malloc(sizeof(struct Token);//zalozi novy prvek listu
  		if (newItem == NULL) {
      		return ERROR_COMPILER;
   	}
	if(numberTokensInList == 0){
		tlistMain.first = newItem*;
		tlistMain.act = newItem*;
		tlistMain.last = newItem*;
		numberTokensInList++;
		newItem.type = atr1 -> type;//uloži do nej hodnoty
		newItem.value = atr1 -> value;
		newItem.next = NULL;
		newItem.prev = NULL;
		return 0;
	}
	newItem.prev = tListMain.act*; // nastaveni predchoziho prvku pro momentalni prvek
	tListMain.act.next = newItem*; // nastaveni nasledujiciho prvku predchozimu prvku
	newItem.next = NULL; // nastaveni nasledujiciho prvku
	tlistMain.act = newItem*; //aktualizace act prvku v seznamu
	tlistMain.last = newItem*;
	numberTokensInList++;
	newItem.type = atr1 -> type;//uloži do nej hodnoty
	newItem.value = atr1 -> value;


	return 0;


}



void get_token(){ //fuknce pro precteni dat ze std. vstupu a ulozeni do seznamu , DKA
	initScanner(); // zavolani inicializace promenych


	char helpString = "a";// v kazdem pruchodu getc dostanu novy
	strcat (tmpString, helpString);



	printf("%s\n", tmpString);
	struct Token helpToken; // token ktery budu ukladat do seznamu


	helpToken.TokenType = tID; //nastavení typu tokenu
	helpToken.value = tmpString; //nastavení hodnoty tokenu

	//ulozit do oboustranne vazaneho seznamu

	tListAdd(helpToken*);

return 0;
}
