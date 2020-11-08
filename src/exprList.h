/** ------------------------------------------------
 * @file exprList.h
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Vlasakova, Nela <xvlasa14>
 * ------------------------------------------------
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief funkce pro pouzivani obousmerneho seznamu
 * -----------------------------------------------*/ 

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "expression.h"

/**
 * @struct struktura reprezentujici jednu polozku v obousmerne vazanem seznamu
 * @brief obsahuje hodnotu, datovy typ, symbol v PT a odkaz na nasledujici a predchozi polozku
 */
typedef struct ListItem {
    char value[50];
    PtType ptType;
    DataType dType;
    struct ListItem *next;
    struct ListItem *prev;
} *item;


/**
 * @struct struktura reprezentujici obousmerne vazany seznam
 * @brief obsahuje prvni a aktivni prvek
 */
typedef struct ExpressionList {
    item first;
    item last;
    item act;
} exprList;


/**
 * @brief Inicializace zasobniku
 * @param l seznam, ktery chceme inicializovat (predavat jako &l)
 */
void listInit(exprList *l);


/**
 * @brief vytvori novou polozku a vlozi ji do seznamu, bud na zacatek nebo za aktivni
 * 
 * @param l seznam, kam chceme vkladat
 * @param ptType symbol z precedencni tabulky
 * @param dType datovy typ
 * @param c hodnota, kterou chceme vkladat
 */
ERROR_CODE insertItem(exprList *l, PtType ptType, DataType dType, char *c);


/**
 * @brief vymaze aktivni polozku v seznamu
 * 
 * @param l seznam, ze ktereho chceme odstranit
 */
void removeItem(exprList *l);

