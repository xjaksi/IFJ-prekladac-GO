
/** -----------------------------------------------
 * @file tokenList.c
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Jaksik, Ales <xjaksi01>
 * ------------------------------------------------
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief Implementovane funkce pro praci s obousmerne vazanym seznamem tokenu
 * -----------------------------------------------*/

#ifndef TOKEN_LIST_C
#define TOKEN_LIST_C

#include<stdio.h>
#include<stdlib.h>
#include "tokenList.h"
#include "scanner.h"



void DLInitList (tokenList *L) {
   
    // nastavime vsechny pointry na NULL
    L->First = NULL;
    L->Act = NULL;
    L->Last = NULL;
    // globalni pocitadla id
    L->if_cnt = 0;
    L->for_cnt = 0;
}

/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Vsechny prvky uvolni funkci free().
**/

void DLDisposeList(tokenList *L, int flag_atribute_dispose) {
   
    // pomocny ukazatel na strukturu prvku
    TokenPtr sup;

    // cyklus rozkladajici seznam prvek po prvku
    while(L->First != NULL){
        sup = L->First->rptr;

        // pokud byl pouzit atribut, tez se uvolni jeho pamet
        if((L->First->atribute != NULL) && flag_atribute_dispose){
            str_Dispose(L->First->atribute);
        }

        free(L->First);
        L->First = sup;
    }
    
    // nastavime pointry Act a Last na NULL
    L->Act = NULL;
    L->Last = NULL;
}

/*
** Vloží nový prvek na konec seznamu L
**/
int DLInsertLast(tokenList *L, TokenType param_t_type, tStr *p_atribute) {
    
    // pomocny pointer ukazuje na prave vytvoreny novy prvek pomoci malloc()
    TokenPtr sup = malloc(sizeof(struct Token));

    // kontrola spravne allokace
    if(sup == NULL){
        return 99;
    }
    
    // prirazeni hodnoty do noveho prvku
    sup->atribute = p_atribute;
    sup->t_type = param_t_type;
    sup->rptr = NULL;
    sup->lptr = L->Last;

    // pokud nevkladame do prazdneho seznamu, napoji se predchozi prvek na prvek pridavany 
    if(sup->lptr != NULL){
        L->Last->rptr = sup;
    }
    else{ // pokud se jedna o prvvni pridany prvek do seznamu nastavime First na sup
        L->First = sup; 
    }
   
    // aktualizce posledniho prvku na prvek pridavany
    L->Last = sup;
    return 0;
}

/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
**/
void DLSucc (tokenList *L) {

    // kontrola aktivity
    if(L->Act == NULL){
        return;
    }

    L->Act = L->Act->rptr;
}

/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
**/
void DLPred (tokenList *L) {

    // kontrola aktivity
    if(L->Act == NULL){
        return;
    }

    // presunuti aktivity na levy prvek
    L->Act = L->Act->lptr;
}

/*
** Nastaví aktivitu na první prvek seznamu L.
**/
void DLFirst (tokenList *L) {
    L->Act = L->First;
}

/*
** Nastaví aktivitu na poslední prvek seznamu L.
**/
void DLLast (tokenList *L) {	
    L->Act = L->Last;	
}

/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
**/
int DLActive (tokenList *L) {

    // ternarni operator priradi 0 kdyz seznam neni aktivni jinak 1 kdyz je
    return (L->Act == NULL)? 0: 1;
}

#endif //TOKEN_LIST_C
