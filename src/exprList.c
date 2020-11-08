/** -----------------------------------------------
 * @file exprList.c
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Vlasakova, Nela <xvlasa14>
 * ------------------------------------------------
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief pomocne funkce pro obousmerny seznam
 * -----------------------------------------------*/ 

#include "exprList.h"


void listInit(exprList *l) {
    l->first = NULL;
    l->last = NULL;
    l->act = NULL;
}

ERROR_CODE insertItem(exprList *l, PtType ptType, DataType dType, char *c) {
    // vytvoreni noveho prvku
    item newItem = (struct ListItem*) malloc(sizeof(struct ListItem));
    if (newItem == NULL) {
        return ERROR_COMPILER;
    }

    strcpy(newItem->value, c);
    newItem->ptType = ptType;
    newItem->dType = dType;
    newItem->next = NULL;

    // vlozeni do seznamu pokud je seznam pradzny
    if (l->first == NULL){
        newItem->prev = NULL;
        l->first = newItem;
        l->act = newItem;
        l->last = newItem;
    }

    // vlozeni do seznamu pokud seznam neni prazdny
    else {
        l->last->next = newItem;
        newItem->prev = l->last;
        l->last = newItem;
    }
    return OK;
}

void removeItem(exprList *l) {
    // pokud seznam neni prazdny
    if (l->first != NULL) {

        // pokud seznam obsahuje jeden prvek
        if (l->first == l->last) {
            l->first = NULL;
            l->last = NULL;
            l->act = NULL;
            free(l->last);
        }

        // pokud je jich tam vice
        else {
            l->last = l->last->prev;
            free(l->last->next);
            l->last->next = NULL;
            l->act = l->last;
        }
    }
    
}
