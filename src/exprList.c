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
 * @brief funkce pro praci se seznamy
 * -----------------------------------------------*/ 

#include "exprList.h"



void listInit(exprList *l) {
    l->first = NULL;
    l->last = NULL;
    l->act = NULL;
}


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

// TO DO !!!!
ERROR_CODE fillMyList (exprList *l, tokenList *tList, treeList *tree) {
    int DATA;   // TO DO: odstranit
     // prochazeni celeho tokenoveho seznamu
    while (tList->Act != NULL) {

        // podle toho, o jaky typ tokenu se jedna, se rozhodne, jak se provede vlozeni do seznamu l
        switch (tList->Act->t_type) {
        // jedna se o konstanty, tedy int, float nebo string
        case tINT: 
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
            DATA = dataSearch(tree, tList->Act->atribute->str);
            if (DATA == 101) {
                return ERROR_UNDEFINED;
            }            
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


ERROR_CODE postfix(tokenList *expr, tokenList *postfixExpr) {
    int precInput, precOp;     
	tokenList operators;        ///< seznam fungujici jako zasobnik pro operatory
	DLInitList(&operators);    

    // prochazeni vstupniho seznamu
	while (expr->Act != NULL) {
		switch (expr->Act->t_type) {

            // pokud jde o operand (ID nebo konstantu), vlozeni na vysledny seznam   
            case tID: case tFLOAT: case tSTRING: case tINT:
                DLInsertLast(postfixExpr, expr->Act->t_type, expr->Act->atribute);
                break;
            
            // pokud jde o levou zavorku, je vlozena na seznam operatoru
            case tLBRACKET:
                // // // // fprintf(stderr, "[debug] pushing (\n");
                DLInsertLast(&operators, expr->Act->t_type, expr->Act->atribute);
                break;

            // pokud jde o pravou zavorku, presouvaji se operatory na vysledny seznam, dokud se nenarazi na pravou zavorku
            case tRBRACKET:
                while (operators.Last->t_type != tLBRACKET) {
                    // // // // fprintf(stderr, "[debug] rbracket removing %s\n", operators.Last->atribute->str);
                    DLInsertLast(postfixExpr, operators.Last->t_type, operators.Last->atribute);
                    deleteToken(&operators);
                }
                // // // // fprintf(stderr, "[debug] rbracket removing %s\n", operators.Last->atribute->str);
                    deleteToken(&operators);
            break;

            // vsechno ostatni by mel byt operator
            default:
                // zjisteni precedence
                if (operators.Last == NULL) {
                        precOp = 0;
                }
                else {
                    precInput = getPrecedence(expr->Act->t_type);
                    precOp = getPrecedence(operators.Last->t_type);                    
                }

                // dokud je na seznamu operatoru neco s vyssi precedenci nez na vstupu
                // // // // // fprintf(stderr, "Input: %s prec: %d \t Operator: %s prec: %d \n",expr->Act->atribute->str, precInput, operators.Last->atribute->str,precOp);
                while (precOp >= precInput) {
                    // presunout tento operator na vysledny seznam a zaroven jej odstranit ze seznamu operatoru
                    // // // // fprintf(stderr, "[debug] removing %s\n", operators.Last->atribute->str);
                    DLInsertLast(postfixExpr, operators.Last->t_type, operators.Last->atribute);
                    deleteToken(&operators); 
                    if (operators.Last == NULL) {
                        precOp = 0;
                    }
                    else {
                        precOp = getPrecedence(operators.Last->t_type);
                    }
                    
                }
                // // // // fprintf(stderr, "[debug] pushing %s\n", expr->Act->atribute->str);
                // nakonec zkoumany operator muzeme vlozit na seznam operatoru
                DLInsertLast(&operators, expr->Act->t_type, expr->Act->atribute);
                break;
        }
        /* operators.Act = operators.First;
        // // // fprintf(stderr, "[operators]: ");
        while (operators.Act != NULL) {
            // // // fprintf(stderr, "%s", operators.Act->atribute->str);
            operators.Act = operators.Act->rptr;
        }
        // // // fprintf(stderr, "\n"); */
        DLSucc(expr);
	}

    // po zpracovani celeho seznamu se zbytek operatoru prida do vystupniho seznamu 
    while (operators.Last != NULL) {
        DLInsertLast(postfixExpr, operators.Last->t_type, operators.Last->atribute);
        deleteToken(&operators);
        DLSucc(&operators);
    }
    return OK;
}


void deleteToken(tokenList *t) {
    // pokud seznam neni prazdny)
    if (t->First != NULL) {

        // pokud seznam obsahuje jeden prvek
        if (t->First ==t->Last) {
            t->First = NULL;
            t->Last = NULL;
            t->Act = NULL;
            free(t->Last);
        }

        // pokud je jich tam vice
        else {
            t->Last = t->Last->lptr;
            free(t->Last->rptr);
            t->Last->rptr = NULL;
            t->Act = t->Last;
        }
    }
}


int getPrecedence(TokenType type) {
    switch (type) {
    case tMUL: case tDIV:
        return 3;
        break;
    
    case tADD: case tSUB:
        return 2;

    case tLEQ: case tGEQ: case tEQ: case tNEQ: case tGT: case tLT:
        return 1;

    default:
        return 0;
        break;
    }
}