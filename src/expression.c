/** ------------------------------------------------
 * @file expression.c
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Vlasakova, Nela <xvlasa14>
 * ------------------------------------------------
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief precedencni analyza vyrazu
 * -----------------------------------------------*/ 

#include "exprList.h"


 char precTable[PT_SIZE][PT_SIZE] = {
  //  +    -    *    /   cmp   (    )    $
	{'R', 'R', 'S', 'S', 'R', 'S', 'R', 'R'},		// +
	{'R', 'R', 'S', 'S', 'R', 'S', 'R', 'R'},		// -
	{'R', 'R', 'R', 'R', 'R', 'S', 'R', 'R'},		// *
	{'R', 'R', 'R', 'R', 'R', 'S', 'R', 'R'},		// /
	{'S', 'S', 'S', 'S', 'R', 'S', 'R', 'R'},		// cmp
	{'S', 'S', 'S', 'S', 'S', 'S', 'S', 'E'},		// (
	{'R', 'R', 'R', 'R', 'R', 'E', 'S', 'R'},		// )
	{'S', 'S', 'S', 'S', 'S', 'S', 'E', 'A'},		// $ 
};

exprList opStack;
exprList idStack;
exprList input;

bool exprFlag = false;

ERROR_CODE finalError;

ERROR_CODE parseExp(tokenList *tList) {
	// inicializace seznamu: vstup, operatory, id/vyrazy
	listInit(&input);
	listInit(&opStack);
	listInit(&idStack);

	// vlozeni stop symbolu na konec seznamu pro operatory a id/vyrazy
	insertItem(&opStack, PT_STOP, DT_NONE, false);
	insertItem(&idStack, PT_STOP, DT_NONE, false);

	
	tList->Act = tList->First;		// [token] nastaveni aktivity na prvni prvek
	fillMyList(&input, tList);		// [token] naplneni seznamu z toknu
	input.act = input.first; 		// [vstup] nastaveni aktivity na prvni prvek

	
	while (input.act != NULL) {		// precedencni analyza - prochazeni vstupniho seznamu
		if(input.act->ptType == PT_EXP || input.act->ptType == PT_CONST) {
			exprFlag = true;
			shift();
		}
		else {		
			// operace se urci podle precedencni tabulky
			switch (precTable[opStack.act->ptType][input.act->ptType]) {			
				case 'R':		// operace je redukce
					finalError = reduce();
					if (finalError != OK) {
						return finalError;
					}	
					break;

				case 'S':		// operace je shift (= nacteni do seznamu operatoru)
					shift();
					break;
		
				case 'A':		// konec analyzy
					finalError = accept();
					if (finalError != OK) {
						return finalError;
					}	
					return finalError;

				case 'E':		// nepovolena sekvence znaku
					return ERROR_SYNTAX;

				default:
					return ERROR_COMPILER;
			}
		}
	}
	return finalError;
}

ERROR_CODE reduce() {
	// (E) -> E redukce
	if (opStack.act->ptType == PT_RBR) {
		// odstraneni zavorek ze seznamu
		removeItem(&opStack);
		removeItem(&opStack);
	}
	
	// E [op] E -> E redukce
	else {	
		DataType finalType;		///< promenna drzici finalni datovy typ

		// KROK 1: zajisteni stejnych datovych typu
		if (idStack.act->dType == idStack.act->prev->dType) {
			finalType = idStack.act->dType;
			
			// KROK 2: zjisteni operace, kterou provadime
			switch (opStack.act->ptType) {
				case PT_ADD:
					// KROK 3: zjistit kompatibilitu typu
					if (finalType == DT_NONE || finalType == DT_BOOL) {
						return ERROR_TYPE_COMPATIBILITY;
					}
					
					removeItem(&opStack);
					removeItem(&idStack); 
					break;

				case PT_MUL:
					/* code */
					break;
				
				case PT_DIV:
					/* code */
					break;

				case PT_CMPS:
					/* code */
					break;
				default:
					break;
			}
			// overeni deleni nulou v int i float pripadech
			if ((strcmp(opStack.act->value, "/") == 0) && (idStack.act->ptType == PT_CONST) && (strtof(idStack.act->value, NULL) == 0)) {	
				return ERROR_ZERO_DIVISION;	
			}
				
			removeItem(&idStack);
			if (idStack.act->ptType == PT_STOP) {
				return ERROR_SYNTAX;
			}
			
			removeItem(&opStack);
		
		}
		else {
			return ERROR_TYPE_COMPATIBILITY;
		}
	} 
	return OK;	
}

// DONE NESAHAT !!!!
void shift() {
	if (exprFlag) {
		insertItem(&idStack, input.act->ptType, input.act->dType);
		idStack.act = idStack.act->next;
		exprFlag = false;
	}
	else {
		insertItem(&opStack, input.act->ptType, input.act->dType);
		opStack.act = opStack.act->next;
	}	
	input.act = input.act->next;
}

// DONE NESAHAT !!!!
ERROR_CODE accept() {
	// v seznamu ID by melo zustat jen jedno E, po odstraneni by tam mel byt jen $
	removeItem(&idStack); 

	if (idStack.act->ptType != PT_STOP) {
		return ERROR_SYNTAX;
	}
	
	input.act = input.act->next;
	return OK;
}
