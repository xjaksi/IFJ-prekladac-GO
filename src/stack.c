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


void stackInit(stack* s) {
    s->top = NULL;
}

void push(stack* s, PtType ptType, DataType dType){
    sElem* newElem;
    newElem = (sElem*) malloc(sizeof(sElem));
    // TO DO osetrit malloc error
    newElem->next = s->top;
    newElem->ptType = ptType;
    newElem->dType = dType;

    s->top = newElem;
}

sElem* getTop(stack* s) {
    return s->top;
}

int pop(stack* s) {
    if (s->top != NULL) {
        printf("got here \n");
        sElem* tempToPop = s->top;
        s->top = tempToPop->next;
        free(tempToPop);
        return OK;
    }
    else {
        return ERROR_COMPILER;
    }
    
}