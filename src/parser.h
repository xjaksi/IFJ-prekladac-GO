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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "tokenList.h"
#include "symtable.h"
#include "expression.h"



int parse();

int cProg(tokenList *token, treeNode *funcTab, treeNode *localTab);
int cScel(tokenList token, treeNode *funcTab, treeNode *localTab);
int funcSave(tokenList token, treeNode *funcTab);
int cBody(tokenList token, treeNode *funcTab, treeNode *localTab);
int cId(tokenList token, treeNode *funcTab, treeNode *localTab);
int cIf(tokenList token, treeNode *funcTab, treeNode *localTab);
int cFor(tokenList token, treeNode *funcTab, treeNode *localTab);
int cFunc(tokenList token, treeNode *funcTab, treeNode *localTab);
int cParams(tokenList token, treeNode *funcTab, char *K);
int cExpr(tokenList token, treeNode *funcTab, treeNode *localTab);
