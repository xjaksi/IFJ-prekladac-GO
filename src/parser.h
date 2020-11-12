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

#include "tokenList.h"
#include "expression.h"



int parse();

int cProg(tokenList token);
int cScel(tokenList token);
int funcSave(tokenList token);
int cBody(tokenList token);
int cId(tokenList token);
int cIf(tokenList token);
int cFor(tokenList token);
int cFunc(tokenList token);
int cParams(tokenList token);
int cExpr(tokenList token);

#endif