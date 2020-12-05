/** -----------------------------------------------
 * @file tokenList.c
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Jaksik, Ales <xjaksi01>
 * ------------------------------------------------
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief Knihovna pro generator.c
 * -----------------------------------------------*/

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exprList.h"
// #include "parser.h"      TODO

void inception_gen();

// vestavene funkce
void gen_inputs();
void gen_itputi();
void gen_inputf();
void gen_print();
void gen_int2float();
void gen_float2int();
void gen_len();
void gen_substr();
void gen_ord();
void gen_chr();

// vyrazy
void gen_expresion(tokenList *);

// for funkce
void gen_for(int);
void gen_for_end(int);

// if-then-else
void gen_if(int);
void gen_else(int);
void gen_else_end(int);

// def
void gen_defvar(char *);

#endif // GENERATOR_H