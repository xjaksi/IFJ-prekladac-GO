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
char text[100] = " ";	///< promenna pro debugovani
///< precedencni tabulka
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

exprList opStack;		///< seznam fungujici jako zasobnik pro operatory
exprList idStack;		///< seznam fungujici jako zasobnik pro vyrazy/id
exprList input;			///< seznam reprezentujici cely vstupni vyraz 
bool exprFlag = false;	///< priznak pro urceni, jestli se jedna o ID nebo operator (pro shift funkci)

ERROR_CODE finalError;	///< navratova hodnota

ERROR_CODE parseExp(tokenList *tList, treeList *tree, int *final) {
	// inicializace seznamu: vstup, operatory, id/vyrazy
	listInit(&input);
	listInit(&opStack);
	listInit(&idStack);

	// vlozeni stop symbolu na konec seznamu pro operatory a id/vyrazy
	insertItem(&opStack, PT_STOP, DT_NONE, false);
	insertItem(&idStack, PT_STOP, DT_NONE, false);


		
						
					


	DLFirst(tList);							// [token] nastaveni aktivity na prvni prvek
	finalError = fillMyList(&input, tList, tree);		// [token] naplneni seznamu z toknu
	if (finalError != OK) {
		return finalError;
	}
	reset(&input); 							// [vstup] nastaveni aktivity na prvni prvek
	
	// fprintf(stderr, "VYRAZ: ");
	while(input.act != NULL) {
		debug(input.act->ptType);
		// fprintf(stderr, "%s ", text);
		next(&input);
	}
	// fprintf(stderr, "\n");
	reset(&input);
	
	DLFirst(tList);
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
					finalError = accept(final);
					if (finalError != OK) {
						return finalError;
					}	
					tokenList output;
					DLInitList(&output);

					DLFirst(tList);	
		fprintf(stderr,"[TOKEN] input:\t");
		while (tList->Act != NULL)
		{
			if(tList->Act->atribute == NULL) {
				debug(tokenToPT(tList->Act->t_type));
				fprintf(stderr, "%s ", text);
			}
			else {
				fprintf(stderr, "%s ", tList->Act->atribute->str);
			}
			
			tList->Act = tList->Act->rptr;
		}
		fprintf(stderr,"\n");
						
					DLFirst(tList);

					 postfix(tList, &output);

					DLFirst(&output);

					 fprintf(stderr,"[POSFITX] posfixed output:\t");
					while(output.Act != NULL) {
						if(output.Act->atribute == NULL) {
							debug(tokenToPT(output.Act->t_type));
							fprintf(stderr, "%s ", text);
						}
						else {
							fprintf(stderr, "%s ", output.Act->atribute->str);			
						}
						output.Act = output.Act->rptr;
					}
					fprintf(stderr, "\n");
					DLDisposeList(&output, 0); 
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
		// // fprintf(stderr, "[INFO] bracket reduction \n");
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
			if (finalType == DT_NONE) {
				return ERROR_SYNTAX;
			}
			
			// KROK 2: zjisteni operace, kterou provadime
			switch (opStack.act->ptType) {

				// KROK 3: zjistit kompatibilitu typu a operace
				case PT_ADD: 
					if (finalType == DT_BOOL) {	
						return ERROR_TYPE_COMPATIBILITY;
					}
					break;
				case PT_CMPS:
					if (finalType == DT_BOOL) {	
						return ERROR_TYPE_COMPATIBILITY;
					}
					idStack.act->prev->dType = DT_BOOL;
					break;

				case PT_MUL: case PT_DIV: case PT_SUB: 
					if (finalType == DT_BOOL || finalType == DT_STRING) {		
						return ERROR_TYPE_COMPATIBILITY;
					}

					if (opStack.act->ptType == PT_DIV) {
						if (idStack.act->isZero == true) {
							return ERROR_ZERO_DIVISION;
						}
					}
					
					break;

				default:
					// // fprintf(stderr, "OOPS NELA FUCKED UP\n");
					return ERROR_COMPILER;
			}
			debug(opStack.act->ptType);
			// fprintf(stderr, "[INFO] basic reduction E %s E \n", text);

			// KROK 4: redukce (odstraneni operatoru a jednoho z vyrazu ze seznamu)
			removeItem(&opStack);
			removeItem(&idStack); 
		}
		else {
			// fprintf(stderr, "ID typ: %d   ID prev typ: %d\n", idStack.act->dType, idStack.act->prev->dType);
			if (idStack.act->dType == DT_NONE || idStack.act->prev->dType == DT_NONE) {
				return ERROR_SYNTAX;
			}
			else {
				return ERROR_TYPE_COMPATIBILITY;
			}
			
		}
	} 
	return OK;	
}


void shift() {
	if (exprFlag) {
		debug(input.act->ptType);
		// // fprintf(stderr, "[INFO] shifting %s onto idStack \n", text);
		insertItem(&idStack, input.act->ptType, input.act->dType, input.act->isZero);
		idStack.act = idStack.act->next;
		exprFlag = false;
	}
	else {
		debug(input.act->ptType);
		// // fprintf(stderr, "[INFO] shifting %s onto opStack \n", text);
		insertItem(&opStack, input.act->ptType, input.act->dType, input.act->isZero);
		opStack.act = opStack.act->next;
	}	
	input.act = input.act->next;
}


ERROR_CODE accept(int *final) {
	*final = idStack.act->dType;
	// v seznamu ID by melo zustat jen jedno E, po odstraneni by tam mel byt jen $
	removeItem(&idStack); 

	if (idStack.act->ptType != PT_STOP) {
		return ERROR_SYNTAX;
	}
	
	// input.act = input.act->next;
	return OK;
}


void debug(PtType i) {
	switch (i) {
		case PT_ADD:
			strcpy(text, "+");
			break;
		case PT_SUB:
			strcpy(text, "-");
			break;

		case PT_MUL:
			strcpy(text, "*");
			break;

		case PT_DIV:
			strcpy(text, "/");
			break;

		case PT_CMPS:
			strcpy(text, "CMPS");
			break;

		case PT_LBR:
			strcpy(text, "(");
			break;

		case PT_RBR:
			strcpy(text, ")");
			break;

		case PT_STOP:
			strcpy(text, "$");
			break;

		case PT_EXP:
			strcpy(text, "E");
			break;

		case PT_CONST:
			strcpy(text, "E");
			break;

		default:
			strcpy(text, "ERROR");
			break;
	}
}

	
	


