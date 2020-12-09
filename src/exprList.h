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
 * @brief funkce pro pouzivani obousmernych seznamu
 * -----------------------------------------------*/ 
#ifndef EXPRLIST_H
#define EXPRLIST_H
#include <stdio.h>
#include <stdlib.h>

#include "expression.h"

/**
 * @struct struktura reprezentujici jednu polozku v obousmerne vazanem seznamu
 * @brief obsahuje hodnotu, datovy typ, symbol v PT a odkaz na nasledujici a predchozi polozku
 */
typedef struct ListItem {
    bool isZero;
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
 * @param isZero priznak, jestli je hodnota nula
 */
ERROR_CODE insertItem(exprList *l, PtType ptType, DataType dType, bool isZero);


/**
 * @brief vymaze aktivni polozku v seznamu
 * 
 * @param l seznam, ze ktereho chceme odstranit
 */
void removeItem(exprList *l);


/**
 * @brief vrati ekvivalent typu tokenu z precedencni tabulky
 * 
 * @param tType token typ
 * @return PtType ekvivalent zadaneho tokenu
 */
PtType tokenToPT(TokenType tType);


/**
 * @brief funkce vezme vstupni seznam tokenu a naplni jim druhy seznam, provadi i kontroly semantiky
 * 
 * @param l seznam, ktery bude naplnen
 * @param tList seznam tokenu
 * @return ERROR_CODE navratova hodnota, OK nebo nejaky chybovy kod
 */
ERROR_CODE fillMyList (exprList *l, tokenList *tList, treeList *tree);


/**
 * @brief funkce posune aktivitu na dalsi prvek v seznamu
 * 
 * @param l seznam, ve kterem se chceme posunout
 */
void next(exprList *l);


/**
 * @brief funkce vrati/nastavi aktivitu na prvni prvek
 * 
 * @param l seznam, kde chceme napravit aktivitu
 */
void reset(exprList *l);


/**
 * @brief prevod z infixu na postfix
 * 
 * @param expr seznam tokenu v infix formatu
 * @param output seznam token v postfix formatu
 * @return ERROR_CODE chybova hodnota
 */
ERROR_CODE postfix(tokenList *expr, tokenList *output);


/**
 * @brief vymazani posledniho tokenu aka "pop"
 * 
 * @param t seznam tokenu
 */
void deleteToken(tokenList *t);


/**
 * @brief vrati stupen precedence daneho operatoru
 * 
 * @param type typ tokenu
 * @return int hodnota precedence
 */
int getPrecedence(TokenType type);

ERROR_CODE prerunExpr(tokenList *t);

void debugg(PtType i);

#endif