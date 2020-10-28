/* ----------------------------------------------
	Předmět: IFJ
	Tým: 101
	Autor:
		Jaksik, Ales (xjaksi01)
-------------------------------------------------
        Vlasakova, Nela (xvlasa14)
        Belohlavek, Honza
        Mraz, Filip (xmrazf00)
-------------------------------------------------
 Účel: Tabulka symbolů
 Popis: Hlavičkový soubor pro tabulku symbolů
 ----------------------------------------------*/

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

void BSTInit   (tBSTNodePtr *);
nodeInfCont BSTSearch  (tBSTNodePtr *, char *);
int BSTInsert (tBSTNodePtr *, char *, nodeInfCont);
void BSTDispose(tBSTNodePtr *);

nodeInfCont createCont (nodeType, int, bool, dataType);
void BSTContActual (tBSTNodePtr *, char*, dataType, int);

/*                Konec binarniho stromu                */
/* ---------------------------------------------------- */

#endif
