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
	{'R', 'R', 'R', 'R', 'R', 'E', 'R', 'R'},		// )
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

	DLFirst(tList);				// [token] nastaveni aktivity na prvni prvek
	fillMyList(&input, tList);		// [token] naplneni seznamu z toknu
	reset(&input); 					// [vstup] nastaveni aktivity na prvni prvek
	
	fprintf(stderr, "FILLED LIST: %s", debug(input.act->ptType));
	while (input.act != NULL){
		fprintf(stderr, "%d ", input.act->ptType);
		next(&input);
	}
	fprintf(stderr, "\n");

	reset(&input);

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
					fprintf(stderr, "[DEBUG] opStack: %s \t input: %s \n", debug(opStack.act->ptType), debug(input.act->ptType));
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
		fprintf(stderr, "[INFO] bracket reduction \n");
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

				// KROK 3: zjistit kompatibilitu typu a operace
				case PT_ADD: case PT_CMPS:
					if (finalType == DT_NONE || finalType == DT_BOOL) {		
						return ERROR_TYPE_COMPATIBILITY;
					}
					break;

				case PT_MUL: case PT_DIV: case PT_SUB: 
					if (finalType == DT_NONE || finalType == DT_BOOL || finalType == DT_STRING) {		
						return ERROR_TYPE_COMPATIBILITY;
					}

					if (opStack.act->ptType == PT_DIV) {
						if (idStack.act->isZero == true) {
							return ERROR_ZERO_DIVISION;
						}
					}
					
					break;

				default:
					fprintf(stderr, "OOPS NELA FUCKED UP\n");
					return ERROR_COMPILER;
					break;
			}

			fprintf(stderr, "[INFO] basic reduction E %s E \n", debug(opStack.act->ptType));

			// KROK 4: redukce (odstraneni operatoru a jednoho z vyrazu ze seznamu)
			removeItem(&opStack);
			removeItem(&idStack); 
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
		fprintf(stderr, "[INFO] shifting %s onto idStack \n", debug(input.act->ptType));
		insertItem(&idStack, input.act->ptType, input.act->dType, input.act->isZero);
		idStack.act = idStack.act->next;
		exprFlag = false;
	}
	else {
		fprintf(stderr, "[INFO] shifting %s onto opStack \n", debug(input.act->ptType));
		insertItem(&opStack, input.act->ptType, input.act->dType, input.act->isZero);
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

char* debug(PtType i) {
	char text[100] = "/0";
	char *ptr = text;
	switch (i) {
		case PT_ADD:
			strcpy(text, "+");
			return ptr;

		case PT_SUB:
			strcpy(text, "-");
			return ptr;

		case PT_MUL:
			strcpy(text, "*");
			return ptr;

		case PT_DIV:
			strcpy(text, "/");
			return ptr;

		case PT_CMPS:
			strcpy(text, "CMPS");
			return ptr;

		case PT_LBR:
			strcpy(text, "(");
			return ptr;

		case PT_RBR:
			strcpy(text, ")");
			return ptr;

		case PT_STOP:
			strcpy(text, "$");
			return ptr;

		case PT_EXP:
			strcpy(text, "E");
			return ptr;

		case PT_CONST:
			strcpy(text, "E");
			return ptr;

		default:
			strcpy(text, "ERROR");
			return ptr;
	}
}