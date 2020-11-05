/** -----------------------------------------------
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
	stack stackOne;
	stackInit(&stackOne);
	push(&stackOne, PT_PLUS, DT_NONE);
		printf("PT TYPE: %d \t DATA TYPE: %d \n", stackOne.top->ptType, stackOne.top->dType);
	push(&stackOne, PT_MINUS, DT_INT);
		printf("PT TYPE: %d \t DATA TYPE: %d \n", stackOne.top->ptType, stackOne.top->dType);
		
		pop(&stackOne);
	sElem* stackTemp = getTop(&stackOne);
	printf("stack temp type: %d \t stack temp data type: %d \n", stackTemp->ptType, stackTemp->dType);

	printf("%c\n", precTable[2][0]);
}

int tokenToSymbol (int tokenTemp) {
	// konverze tokenu na symbol v tabulce
}


