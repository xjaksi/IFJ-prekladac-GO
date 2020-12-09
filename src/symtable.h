/** -----------------------------------------------
 * @file symtable.h
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Jaksik, Ales <xjaksi01>
 * ------------------------------------------------
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief Knihovna pro tabulka symbolu
 * -----------------------------------------------*/

#ifndef SYMTABLE_H
#define SYMTABLE_H

// nejake knihovny
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include "errors.h"

/* ---------------------------------------------------- */
/*         Pomucky pro praci s binarnim stromem         */

// typ uzlu
typedef enum
{
    ntVar,          // jedna se o promennou
    ntFunc          // koren je funkce
} nodeType;


// informace o obsahu uzlu
typedef struct nodeCont
{
    nodeType nType;                 // jedna se o funkci nebo promennou
    int noParams;                   // pocet parametru teto funkce
    int noReturn;                   // pocet vracenych parametru + 1 (101)
    int *paramsIn;                   // datovy typ argumentu
    int *paramsOut;                  // datovy typ co funkce vraci
    int dType;                 // nazev datoveho typu
} *nodeInfCont;


// struktura samotneho uzlu
typedef struct BSTNode
{
	char * Key;			            // klic/nazev promenne
	struct nodeCont * TBSNodeCont;  // informace o uzlu
	struct BSTNode * LPtr;         // levy podstrom
	struct BSTNode * RPtr;         // pravy podstrom
} *treeNode;


/******************** Seznam *****************/

// uzel
typedef struct ListCont
{
    struct BSTNode *symtab;
    struct ListCont *next;
} *listT;

// seznam
typedef struct ListOfTrees
{
    listT first;
    listT act;
} treeList;

/*__________________ Seznam _________________*/


// prototypy funkci

/**
 * @brief Inicializace stromu
 * @param RootPtr Koren stromu
 */
void BSTInit (treeNode *RootPtr);

/**
 * @brief Vyhledani uzlu
 * 
 * @param RootPtr Koren stromu
 * @param K Klic hledaneho uzlu
 * 
 * @return Obsah hledaneho uzlu
 */
nodeInfCont BSTSearch (treeNode *RootPtr, char* K);

/**
 * @brief Vkladani do uzlu
 * 
 * @param RootPtr Koren stromu
 * @param K Klic hledaneho uzlu
 * @param def Jedna-li se o definici uzlu
 * @param Content Obsah uzlu
 * 
 * @return Cislo chyby pokud nastala, jinak 0
 */
int BSTInsert (treeNode* RootPtr, char* K, int def, nodeInfCont Content);

/**
 * @brief Mazani binarniho stromu, uvolneni pameti
 * 
 * @param RootPtr Koren stromu
 */
void BSTDispose (treeNode *RootPtr);

/**
 * @brief Vytvareni obsahu uzlu
 * 
 * @param nnType Jedna-li se o promennou nebo funkci
 * @param noParam Pocet parametru funkce
 * @param noRet Pocet navratovych hodnot
 * @param arg Pole s datovymi typy parametru funkce
 * @param out Pole s datovymi typy navratovych hodnot
 * @param ddType Datovy typ promenne
 * 
 * @return Struktura obsahujici informace o uzlu
 */
nodeInfCont createCont (nodeType nnType, int noParam, int noRet, int arg[], int out[], int ddType);

/*                Konec binarniho stromu                */
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/*     Seznam/zasobnik pro jednotlive ramce/stromy      */

/**
 * @brief Inicializace seznamu
 * 
 * @param l Odkaz na seznam
 */
void treeListInit(treeList *l);

/**
 * @brief Vkladani strmu do seznamu
 * 
 * @param l Odkaz na seznam
 * @param tree Odkaz na vkladany strom
 * 
 * @return Chyba, bokud ne 0
 */
int treeListInsert(treeList *l, treeNode *tree);

/**
 * @brief Odstraneni prvniho listu seznamu
 * 
 * @param l Odkaz na seznam
 */
void treeListRemove(treeList *l);

/**
 * @brief Zruseni celeho seznamu
 * 
 * @param l Odkaz na seznam
 */
void treeListDestroy(treeList *l);

/**
 * @brief Prohledavani strumu v seznamu
 * 
 * @param l Odkaz na seznam
 * @param k Klic hledaneho uzlu
 * 
 * @return cislo datoveho typu podle tokenListu
 */
int dataSearch(treeList *l, char *k);

#endif
