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



int parse();

int cScel(tokenList *token, treeNode *funcTab, treeList *tList);
int funcSave(tokenList *token, treeNode *funcTab);
int cBody(tokenList *token, treeNode *funcTab, treeList *tList);
int cId(tokenList *token, bool dev, treeNode *funcTab, treeList *tList);
int cIf(tokenList *token, treeNode *funcTab, treeList *tList);
int cFor(tokenList *token, treeNode *funcTab, treeList *tList);
int cFunc(tokenList *token, treeNode *funcTab, treeList *tList);
int cParams(tokenList *token, treeNode *funcTab, treeList *tList);
int cExpr(tokenList *token, treeNode *funcTab, treeList *tList);

void buidInFunc(treeNode *funcTab);

#endif