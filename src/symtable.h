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
    int noReturn;                   // pocet vracenych parametru
    int paramsIn;                   // datovy typ argumentu
    int paramsOut;                  // datovy typ co funkce vraci
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
int BSTInsert (treeNode *, char *, bool, nodeInfCont);
void BSTDispose(treeNode *);

nodeInfCont createCont (nodeType nnType, int noParam, int noRet, int arg, int out, int ddType)

/*                Konec binarniho stromu                */
/* ---------------------------------------------------- */

#endif
