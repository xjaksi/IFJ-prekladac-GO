/* ------------------------------------------------
 * @file stack.h
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Vlasakova, Nela <xvlasa14>
 * ------------------------------------------------
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief funkce pro pouzivani zasobniku
 * -----------------------------------------------*/ 

#include <stdio.h>
#include <stdlib.h>

#include "expression.h"

/**
 * @struct struktura reprezentujici jednu polozku zasobniku
 * @brief obsahuje datovy typ, symbol v PT a odkaz na nasledujici polozku
 */
typedef struct StackElement {
    PtType ptType;
    DataType dType;
    struct StackElement *next;
} sElem;

/**
 * @struct struktura reprezentujici zasobnik
 * @brief obsahuje ukazatel na vrchol zasobniku
 */
typedef struct Stack {
    sElem *top;
} stack;

/**
 * @brief Inicializace zasobniku
 * @param s zasobnik, ktery chceme inicializovat (predavat jako &s)
 */
void stackInit(stack* s);

/**
 * @brief vrati polozku na vrcholu zasobniku
 * @param s zasobnik, ktereho vrchol chceme znat
 * @return sElem* vrchol zasobniku
 */
sElem* getTop(stack* s);

/**
 * @brief vymaze polozku na vrcholu zasobniku a zaroven nastavi novy vrchol
 * @param s zasobnik, ze ktereho chceme dat vrchol pryc
 */
int pop(stack* s);

/**
 * @brief vytvori novou polozku a vlozi ji na vrchol zasobniku
 * 
 * @param s zasobnik, kam chceme vkladat
 * @param ptType symbol z precedencni tabulky
 * @param dType datovy typ
 */
void push(stack* s, PtType ptType, DataType dType);
