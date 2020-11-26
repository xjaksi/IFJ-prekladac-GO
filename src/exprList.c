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
// DONE NESAHAT !!!!
void listInit(exprList *l) {
    l->first = NULL;
    l->last = NULL;
    l->act = NULL;
}

// DONE NESAHAT !!!!
ERROR_CODE insertItem(exprList *l, PtType ptType, DataType dType, bool isZero) {
    // vytvoreni noveho prvku
    item newItem = (struct ListItem*) malloc(sizeof(struct ListItem));
    if (newItem == NULL) {
        return ERROR_COMPILER;
    }
    
    newItem->isZero = isZero;
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

// DONE NESAHAT !!!!
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


ERROR_CODE fillMyList (exprList *l, tokenList *tList) {
    int DATA;   // TO DO: odstranit
     // prochazeni celeho tokenoveho seznamu
    while (tList->Act != NULL) {

        // podle toho, o jaky typ tokenu se jedna, se rozhodne, jak se provede vlozeni do seznamu l
        switch (tList->Act->t_type) {
        // jedna se o konstanty, tedy int, float nebo string
        case tINT: 
        // TO DO:   isZero flag
            if (strtof(tList->Act->atribute->str, NULL) == 0) {
                insertItem(l, PT_CONST, DT_INT, true);
                break;
            }
            insertItem(l, PT_CONST, DT_INT, false);
            break;

        case tFLOAT: 
            if (strtof(tList->Act->atribute->str, NULL) == 0) {
                insertItem(l, PT_CONST, DT_FLOAT, true);
                break;
            }

            insertItem(l, PT_CONST, DT_FLOAT, false);
            break;

        case tSTRING:
            insertItem(l, PT_CONST, DT_STRING, false);
            break;
        
        case tID:
            DATA = DT_INT; // TO DO: vyhledani v BS
            /**
             * TADY BUDE ZJISTENI JESTLI ID EXISTUJE
             * if (bstSearch() == NULL) {
             *      return ERROR_UNDEFINED;
             * }
             */
            insertItem(l, PT_EXP, DATA, false);
            break;

        // jedna se o operator
        default:
            insertItem(l, tokenToPT(tList->Act->t_type), DT_NONE, false);
            break;
        }
        
        // posunuti na dalsi prvek v seznamu
        tList->Act = tList->Act->rptr;
    }

    // vlozeni stop symbolu na konec
	insertItem(l, PT_STOP, DT_NONE, false);
    return OK;
}

// DONE NESAHAT !!!!
PtType tokenToPT(TokenType tType) {
    switch (tType) {
        case tID:
            return PT_EXP;

        case tADD: 
            return PT_ADD;
        
        case tSUB:
            return PT_SUB;
        
        case tMUL: 
            return PT_MUL;
        case tDIV:
            return PT_DIV;

        case tLT: case tGT: case tLEQ: case tGEQ: case tEQ: case tNEQ:
            return PT_CMPS;

        case tLBRACKET:
            return PT_LBR;
        
        case tRBRACKET:
            return PT_RBR;

        default:
            return ERROR_SYNTAX;
    }
}

void next(exprList *l) {
    l->act = l->act->next;
}

void reset(exprList *l) {
    l->act = l->first;
}