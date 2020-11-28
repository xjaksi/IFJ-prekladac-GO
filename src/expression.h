/** -----------------------------------------------
 * @file expression.h
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Vlasakova, Nela <xvlasa14>
 * ------------------------------------------------
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief struktury, definice funkci
 * -----------------------------------------------*/ 

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "errors.h"
#include "tokenList.h"
#include "dynamicString.h"
#include "symtable.h"
#define PT_SIZE 8

/**
 * @brief typy symbolu v precedencni tabulce
 */
typedef enum {
    
    PT_ADD,     // 0
    PT_SUB,     // 1
    PT_MUL,     // 2
    PT_DIV,     // 3
    PT_CMPS,    // 4
    PT_LBR,     // 5
    PT_RBR,     // 6
    PT_STOP,    // 7
    PT_EXP,     // 8
    PT_CONST,   
} PtType;

/**
 * @brief datove typy 
 */
typedef enum {
    DT_NONE,
    DT_INT,
    DT_FLOAT,
    DT_STRING,
    DT_BOOL,
} DataType;


/**
 * @brief hlavni funkce, ktera provede precedencni analyzu predaneho vyrazu a zapise vysledny datovy typ do predane promenne
 * 
 * @param tList seznam tokenu tvorici vyraz
 * @return ERROR_CODE navratova hodnota, OK nebo chybovy kod
 */
ERROR_CODE parseExp(tokenList *tList, treeList *tree, int *final);


/**
 * @brief nahrani momentalni polozky do seznamu
 */
void shift();


/**
 * @brief redukce vyrazu, kontrola datove kompatibility
 * 
 * @return ERROR_CODE OK nebo chybovy kod
 */
ERROR_CODE reduce();


/**
 * @brief stav prijeti vyrazu jako spravny, finalni kontrola spravnosti
 * 
 * @param final ukazatel na hodnotu nesouci finalni datovy typ
 * @return ERROR_CODE chybovy kod nebo OK
 */
ERROR_CODE accept(int *final);


/**
 * @brief debugovaci funkce, ktera podle cisla zapise do globalni promenne text
 * 
 * @param i cislo reprezentujici symbol v precedencni tabulce
 */
void debug(PtType i);

