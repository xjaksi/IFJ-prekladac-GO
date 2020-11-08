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
  //  +-   */  cmp   (    )    $
	{'R', 'S', 'R', 'S', 'R', 'R'},		// +-
	{'R', 'S', 'R', 'S', 'R', 'R'},		// */
	{'S', 'S', 'R', 'S', 'R', 'R'},		// cmp
	{'S', 'S', 'S', 'S', 'S', 'E'},		// (
	{'R', 'R', 'R', 'E', 'S', 'R'},		// )
	{'S', 'S', 'S', 'S', 'E', 'A'},		// $ 
};

exprList opStack;
exprList idStack;
exprList input;

bool exprFlag = false;

ERROR_CODE finalError;

ERROR_CODE parseExp() {
	// inicializace vsech potrebnych seznamu
	listInit(&input);
	listInit(&opStack);
	insertItem(&opStack, PT_STOP, DT_NONE, "$");
	listInit(&idStack);
	insertItem(&idStack, PT_STOP, DT_NONE, "$");

	/////////////// M O C K   D A T A ///////////////
	insertItem(&input, PT_LBR, DT_NONE, "(");
	insertItem(&input, PT_EXP, DT_INT, "a");
	insertItem(&input, PT_MULDIV, DT_NONE, "*");
	insertItem(&input, PT_EXP, DT_INT, "10");
	insertItem(&input, PT_ADDSUB, DT_NONE, "+");
	insertItem(&input, PT_EXP, DT_INT, "c");
	insertItem(&input, PT_RBR, DT_NONE, ")");
	insertItem(&input, PT_MULDIV, DT_NONE, "/");
	insertItem(&input, PT_CONST, DT_INT, "0");
	insertItem(&input, PT_STOP, DT_NONE, "$");
	/////////////////////////////////////////////////

	// TO DO sestaveni input seznamu, dohledani datovych typu

	// prochazeni seznamu reprezentujiciho vstupni vyraz
	while(input.act != NULL) {

		// pokud jde o vyraz, provede se nacteni do vlastniho seznamu
		if(input.act->ptType == PT_EXP || input.act->ptType == PT_CONST) {
			exprFlag = true;
			shift();
		}
		
		// vsechno ostatni je operator a podrobuje se precedencni analyze
		else {		
			switch (precTable[opStack.act->ptType][input.act->ptType]) {
			case 'R': 
			 	// (E) -> E redukce
				if (opStack.act->ptType == PT_RBR) {
					removeItem(&opStack);
					removeItem(&opStack);
				}
				
				// E [op] E -> E redukce
				else {		
					DataType eOne = idStack.act->dType;
					DataType eTwo = idStack.act->prev->dType;
					PtType op = opStack.act->ptType;

					// zajisteni stejnych datovych typu
					if (idStack.act->dType == idStack.act->prev->dType) {

						// overeni deleni nulou v int i float pripadech
						if ((strcmp(opStack.act->value, "/") == 0) && (idStack.act->ptType == PT_CONST) && (strtof(idStack.act->value, NULL) == 0)) {	
							return ERROR_ZERO_DIVISION;	
						}

						removeItem(&idStack);
						removeItem(&opStack);
						
					}
					else {
						return ERROR_TYPE_COMPATIBILITY;
					}
				}				
				break;
			case 'S':
				shift();
				break;

			case 'A':
				// v seznamu ID by melo zustat jen jedno El, po odstraneni by tam mel byt jen $
				removeItem(&idStack); 	
				if (idStack.act->ptType != PT_STOP) {
					return ERROR_SYNTAX;
				}
				
				input.act = input.act->next;
				return OK;

			case 'E':
				return ERROR_SYNTAX;

			default:
				return ERROR_COMPILER;
			}
		}
	}
	return finalError;
}

ERROR_CODE reduce() {
	return OK;
}

void shift() {
	if (exprFlag) {
		insertItem(&idStack, input.act->ptType, input.act->dType, input.act->value);
		idStack.act = idStack.act->next;
		exprFlag = false;
	}
	else {
		insertItem(&opStack, input.act->ptType, input.act->dType, input.act->value);
		opStack.act = opStack.act->next;
	}	

	input.act = input.act->next;
}
// TO DO NAPRAVIT
ERROR_CODE fillMyList(tList *t) {
	while(t->act != NULL) {
		fprintf(stderr, "VALUE IS: %s\n", t->act->value);
		t->act = t->act->next;
	}
	return OK;
}


