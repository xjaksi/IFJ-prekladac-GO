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

#include "stack.h"

 char precTable[PT_SIZE][PT_SIZE] = {
  //  +-   */  cmp   (    )    $
	{'R', 'S', 'R', 'S', 'R', 'R'},		// +-
	{'R', 'S', 'R', 'S', 'R', 'R'},		// */
	{'S', 'S', 'R', 'S', 'R', 'R'},		// cmp
	{'S', 'S', 'S', 'S', 'Q', 'E'},		// (
	{'R', 'R', 'R', 'E', 'S', 'R'},		// )
	{'S', 'S', 'S', 'S', 'E', 'A'},		// $ 
};

void printStuff() {
	exprList testList;
	listInit(&testList);

	insertItem(&testList, PT_EXP, DT_INT, "a");
	insertItem(&testList, PT_ADDSUB, DT_NONE, "+");
	insertItem(&testList, PT_EXP, DT_INT, "b");

	while(testList.act != NULL) {
		printf("VALUE IS: %s\n", testList.act->value);
		testList.act = testList.act->next;
	} 
}

ptType tokenToSymbol (token t) {
	switch (token.type) {
	case tID: case tINT: case tFLOAT: case tSTRING:
		return PT_EXP;
	
	case tADD: case tSUB:
		return PT_ADDSUB;
	
	case tDIV: case tMUL:
		return PT_MULDIV;
	
	case tLT: case tLEQ: case tGT: case tGEQ: case tEQ: case tNEQ:
		return PT_CMPS;
	
	case tLBRACKET: 
		return PT_LBR;

	case tRBRACKET:
		return PT_RBR;

	default:
		break;
	}
}

fillMyList(token t) {
	exprList eList;
	listInit(&testList);
}


