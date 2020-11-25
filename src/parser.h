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

int cScel(tokenList *token, treeNode *funcTab, treeNode *localTab);
int funcSave(tokenList *token, treeNode *funcTab);
int cBody(tokenList *token, treeNode *funcTab, treeNode *localTab);
int cId(tokenList *token, bool dev, treeNode *funcTab, treeNode *localTab);
int cIf(tokenList *token, treeNode *funcTab, treeNode *localTab);
int cFor(tokenList *token, treeNode *funcTab, treeNode *localTab);
int cFunc(tokenList *token, treeNode *funcTab, treeNode *localTab);
int cParams(tokenList *token, treeNode *funcTab, treeNode *localTab);
int cExpr(tokenList *token, treeNode *funcTab, treeNode *localTab);

void buidInFunc(treeNode *funcTab);

#endif