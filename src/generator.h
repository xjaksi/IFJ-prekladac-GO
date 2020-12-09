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

// main
void gen_main();

// vyrazy
void gen_expresion(tokenList *);

// funkce
void gen_func_tf_var(TokenPtr, int );
void gen_func_call(char *);
void gen_func_start(char *);
void gen_func_param(char *, int);
void gen_func_ret_start(int);
void gen_func_ret_val(int);
void gen_func_end();
void gen_func_tf_ret(char *, int);

// prirovnani do promenne
void gen_var_assign(char *id_name);

// if-then-else
void gen_if(int);
void gen_else(int);
void gen_else_end(int);

// for 
void gen_for_start(int);
void gen_for_2(int);
void gen_for_3(int);
void gen_for_end(int);

// def
void gen_defvar(char *);

// prevod str na vystupni str
//int str_to_str(tStr *); DELETE
void print_float(char *par);
void print_string(char *par);

// pomocne funkce
void gen_get_stack_bool();

#endif // GENERATOR_H