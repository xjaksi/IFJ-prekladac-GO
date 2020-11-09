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

// datove typy
typedef enum datatype
{
    typeNo,
    typeInt,
    typeDouble,
    typeString,
} dataType;

// informace o obsahu uzlu
typedef struct nodeCont
{
    nodeType nType;                 // jedna se o funkci nebo promennou
    int noParams;                   // pocet parametru teto funkce
    struct tBSTNode *localFrame;    // localni tabulka funkce
    bool defined;                   // bylo jiz definovano?
    dataType dType;                 // nazev datoveho typu
} *nodeInfCont;


// struktura samotneho uzlu
typedef struct tBSTNode
{
	char * Key;			            // klic/nazev promenne
	struct nodeCont * TBSNodeCont;  // informace o uzlu
	struct tBSTNode * LPtr;         // levy podstrom
	struct tBSTNode * RPtr;         // pravy podstrom
} *tBSTNodePtr;

// prototypy funkci

/**
 * @brief Inicializace stromu
 * @param RootPtr Koren stromu
 */
void BSTInit (tBSTNodePtr *RootPtr);

/**
 * @brief Vyhledani uzlu
 * 
 * @param RootPtr Koren stromu
 * @param K Klic hledaneho uzlu
 * 
 * @return Obsah hledaneho uzlu
 */
nodeInfCont BSTSearch (tBSTNodePtr *RootPtr, char* K);
int BSTInsert (tBSTNodePtr *, char *, nodeInfCont);
void BSTDispose(tBSTNodePtr *);

nodeInfCont createCont (nodeType nnType, int noParam, struct tBSTNode *localTable, bool isDefined, dataType ddType);
void BSTContActual (tBSTNodePtr *, char*, dataType, int);

/*                Konec binarniho stromu                */
/* ---------------------------------------------------- */

#endif
