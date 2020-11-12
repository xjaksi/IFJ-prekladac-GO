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
	{'R', 'R', 'R', 'S', 'R', 'R'},		// */
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

ERROR_CODE parseExp(tokenList *tList) {
	int count = 1;
	// inicializace vsech potrebnych seznamu
	listInit(&input);
	listInit(&opStack);
	insertItem(&opStack, PT_STOP, DT_NONE, "$");
	listInit(&idStack);
	insertItem(&idStack, PT_STOP, DT_NONE, "$");

	fillMyList(&input, tList);

	/*			 M O C K   D A T A
	insertItem(&input, PT_LBR, DT_NONE, "(");
	insertItem(&input, PT_EXP, DT_INT, "a");
	insertItem(&input, PT_MULDIV, DT_NONE, "*");
	insertItem(&input, PT_EXP, DT_INT, "10");
	insertItem(&input, PT_ADDSUB, DT_NONE, "+");
	insertItem(&input, PT_EXP, DT_INT, "c");
	insertItem(&input, PT_RBR, DT_NONE, ")");
	insertItem(&input, PT_MULDIV, DT_NONE, "/");
	insertItem(&input, PT_CONST, DT_INT, "10");
	insertItem(&input, PT_STOP, DT_NONE, "$");
												

	while (input.act != NULL) {
		printf("VEC: %d\n", input.act->ptType);
		input.act = input.act->next;
	}
		*/
	input.act = input.first;

	// prochazeni seznamu reprezentujiciho vstupni vyraz
	while (input.act != NULL) {

		// pokud jde o vyraz, provede se nacteni do vlastniho seznamu
		if(input.act->ptType == PT_EXP || input.act->ptType == PT_CONST) {
			exprFlag = true;
			printf("SHIFT EXP: %d\n", count);
				printf("\tSTACK: %d", opStack.act->ptType);
				printf("\tINPUT: %d\n", input.act->ptType);
				printf("\n\n");
			shift();
		}
		
		// vsechno ostatni je operator a podrobuje se precedencni analyze
		else {		
			switch (precTable[opStack.act->ptType][input.act->ptType]) {
			case 'R':
				printf("REDUCTION\n");
				printf("\t STACK: %d", opStack.act->ptType);
				printf("\t INPUT: %d\n", input.act->ptType);
				finalError = reduce();
				printf("\tRESULT: %d\n", finalError);
				printf("\n\n");
			 	if (finalError != OK) {
					 return finalError;
				 }	
				break;
			case 'S':
				printf ("SHIFT %c: %d\n",precTable[opStack.act->ptType][input.act->ptType], count);
				printf("\tSTACK: %d", opStack.act->ptType);
				printf("\tINPUT: %d\n", input.act->ptType);
				printf("\n\n");
				shift();
				break;

			case 'A':
				finalError = accept();
				if (finalError != OK) {
					 return finalError;
				 }	
				return finalError;

			case 'E':
				return ERROR_SYNTAX;

			default:
				return ERROR_COMPILER;
			}
		}
		
		count++;
	}
	printf("idStack: %d\n", idStack.act->ptType);
	return finalError;
}

ERROR_CODE reduce() {
	// (E) -> E redukce
	if (opStack.act->ptType == PT_RBR) {
		removeItem(&opStack);
		removeItem(&opStack);
	}
	
	// E [op] E -> E redukce
	else {		
		//DataType eOne = idStack.act->dType;
		//DataType eTwo = idStack.act->prev->dType;
		//PtType op = opStack.act->ptType;

		// zajisteni stejnych datovych typu
	/*	if (idStack.act->dType == idStack.act->prev->dType) {

			// overeni deleni nulou v int i float pripadech
			if ((strcmp(opStack.act->value, "/") == 0) && (idStack.act->ptType == PT_CONST) && (strtof(idStack.act->value, NULL) == 0)) {	
				return ERROR_ZERO_DIVISION;	
			}
				*/
			removeItem(&idStack);
			if (idStack.act->ptType == PT_STOP) {
				return ERROR_SYNTAX;
			}
			
			removeItem(&opStack);
	/*		
		}
		else {
			return ERROR_TYPE_COMPATIBILITY;
		}*/
	} 
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

ERROR_CODE accept() {
	// v seznamu ID by melo zustat jen jedno E, po odstraneni by tam mel byt jen $
	removeItem(&idStack); 
	printf("ACCEPT TEST \n");	
	printf("\tSTACK: %d", opStack.act->ptType);
	printf("\tINPUT: %d\n", input.act->ptType);
	printf("\n\n");

	if (idStack.act->ptType != PT_STOP) {
		return ERROR_SYNTAX;
	}
	
	input.act = input.act->next;
	return OK;
}
