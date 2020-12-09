/** -----------------------------------------------
 * @file parser.h
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Jaksik, Ales <xjaksi01>
 * ------------------------------------------------
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief Knihovna pro parser.c
 * -----------------------------------------------*/

#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include "expression.h"


/**
 * @brief Zacatek synatakticke a lexikalni kontroly, pravidlo 1
 * 
 * @return Cislo chyby, jinak 0
 */
int parse();

/**
 * @brief Kontrola celkove struktury programu, pravidla 2-4
 * 
 * @param token Seznam s tokeny
 * @param funcTab Tabulku symbolu s funkcemi
 * @param tList List s tabulkama symbolu pro promenne
 * 
 * @return Cislo chyby, jinak 0
 */
int cScel(tokenList *token, treeNode *funcTab, treeList *tList);

/**
 * @brief Predbezny pruchod, kontroluje a nahrava definovane funkce, pravidlo 5
 * 
 * @param token Seznam s tokeny
 * @param funcTab Tabulku symbolu s funkcemi
 * 
 * @return Cislo chyby, jinak 0
 */
int funcSave(tokenList *token, treeNode *funcTab);

/**
 * @brief Kontrola tela, pravidla 6-11
 * 
 * @param token Seznam s tokeny
 * @param funcTab Tabulku symbolu s funkcemi
 * @param tList List s tabulkama symbolu pro promenne
 * @param retVal seznam s navratovymi datovymi typy, pokud jsme ve funkci
 * 
 * @return Cislo chyby, jinak 0
 */
int cBody(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal);

/**
 * @brief Kontrola id, pravidla 12-14
 * 
 * @param token Seznam s tokeny
 * @param funcTab Tabulku symbolu s funkcemi
 * @param tList List s tabulkama symbolu pro promenne
 * 
 * @return Cislo chyby, jinak 0
 */
int cId(tokenList *token, treeNode *funcTab, treeList *tList);

/**
 * @brief Kontrola prirazeni do promenne, pravidla 26-27
 * 
 * @param token Seznam s tokeny
 * @param funcTab Tabulku symbolu s funkcemi
 * @param tList List s tabulkama symbolu pro promenne
 * @param item Pocet id do kterych se snazim zapsat data
 * @param flag_func Informace pro generator, ze se nachazim ve funkci
 * 
 * @return Cislo chyby, jinak 0
 */
int cAssign(tokenList *token, treeNode *funcTab, treeList *tList, int item, int *flag_func);

/**
 * @brief Kontrola if, pravidlo 45
 * 
 * @param token Seznam s tokeny
 * @param funcTab Tabulku symbolu s funkcemi
 * @param tList List s tabulkama symbolu pro promenne
 * @param retVal seznam s navratovymi datovymi typy, pokud jsme ve funkci
 * 
 * @return Cislo chyby, jinak 0
 */
int cIf(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal);

/**
 * @brief Kontrola for, pravidlo 46
 * 
 * @param token Seznam s tokeny
 * @param funcTab Tabulku symbolu s funkcemi
 * @param tList List s tabulkama symbolu pro promenne
 * @param retVal seznam s navratovymi datovymi typy, pokud jsme ve funkci
 * 
 * @return Cislo chyby, jinak 0
 */
int cFor(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal);

/**
 * @brief Kontrola volani funkce
 * 
 * @param token Seznam s tokeny
 * @param funcTab Tabulku symbolu s funkcemi
 * @param tList List s tabulkama symbolu pro promenne
 * @param noItems Pokud zapisuji do promenne, tak pocet parametru prred =
 * @param ass Jedna li se o prirazeni
 * 
 * @return Cislo chyby, jinak 0
 */
int cFunc(tokenList *token, treeNode *funcTab, treeList *tList, int noItems, bool ass);

/**
 * @brief Nahrani vyrazu do vlastniho seznamu a poslani na kontrolu do parseExp
 * 
 * @param token Seznam s tokeny
 * @param tList List s tabulkama symbolu pro promenne
 * @param type parseExp vrati zkrz tento parametr vysledny datovy typ
 * 
 * @return Cislo chyby, jinak 0
 */
int cExpr(tokenList *token, treeList *tList, int *type);

/**
 * @brief Vlozeni zabudovanych funkci do tabulky symbolu
 * 
 * @param funcTab Tabulku symbolu s funkcemi
 */
void buidInFunc(treeNode *funcTab);

#endif