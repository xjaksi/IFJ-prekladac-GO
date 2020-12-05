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

#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
// symtable

void inception_gen(){
    printf("\
    \n.IFJcode20\
    \nJUMP $$main\n\
    ");
    
    gen_inputs();
    gen_itputi();
    gen_inputf();
    gen_print();
    gen_int2float();
    gen_float2int();
    gen_len();
    gen_substr();
    gen_ord();
    gen_chr();
    
}

// generovani promennych
void gen_defvar(char *name){
    printf("\nDEFVAR LF@%s", name);
}




// generovani if_else_then
void gen_if(int if_id){
    printf("\nJUMPIFEQ $else_%d", if_id);
}

void gen_else(int if_id){
    printf("\nJUMP $if_end_%d", if_id);
    printf("\nLABEL $else_%d", if_id);
}

void gen_else_end(int if_id){
    printf("LABEL $end_if%d", if_id);
}

// generovani for
void gen_for(int for_id){
    printf("\nLABEL $for_%d", for_id);
    //printf("\nJUMPIFEQ $for_end_%d", for_id);
}

void gen_for_end(int for_id){
    printf("\nLABEL $for_%d", for_id);
    printf("\nJUMPIFEQ $for_end_%d", for_id);
}

// generovani vyrazu
void gen_expresion(tokenList *l){
    while(l->First != NULL){
        printf("\n %d", l->First->t_type);
    }
}

// oddil vestavenych funkci

void gen_inputs(){
    printf("\
    \n LABEL $inputs\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \n DEFVAR LF@_ret2\
    \n MOVE LF@_ret2 int@0\
    \
    \n READ LF@_ret1 string\
    \n DEFVAR LF@cret1$type\
    \n TYPE LF@ret1$type LF@_ret1\
    \n JUMPIFNEQ $inputs$cond LF@ret1type string@nil\
    \n MOVE LF@_ret2 int@1\
    \n LABEL $inputs$cond\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_itputi(){
    printf("\
    \n LABEL $inputi\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \n DEFVAR LF@_ret2\
    \n MOVE LF@_ret2 int@0\
    \
    \n READ LF@_ret1 int\
    \n DEFVAR LF@cret1$type\
    \n TYPE LF@ret1$type LF@_ret1\
    \n JUMPIFNEQ $inputi$cond LF@ret1type string@nil\
    \n MOVE LF@_ret2 int@1\
    \n LABEL $inputi$cond\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_inputf(){
    printf("\
    \n LABEL $inputf\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \n DEFVAR LF@_ret2\
    \n MOVE LF@_ret2 int@0\
    \
    \n READ LF@_ret1 float\
    \n DEFVAR LF@cret1$type\
    \n TYPE LF@ret1$type LF@_ret1\
    \n JUMPIFNEQ $inputf$cond LF@ret1type string@nil\
    \n MOVE LF@_ret2 int@1\
    \n LABEL $inputf$cond\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_print(){
    printf("\
    \n LABEL $inputf\
    \n PUSHFRAME\
    \n DEFVAR LF@par1\
    \n MOVE LF@par1 LF@%%1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFNEQ $print$par1 string@nil LF@par1$type\
    \n POPFRAME\
    \n RETURN\
    \n LABEL $print$par1\
    \n WRITE LF@par1\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_int2float(){
    printf("\
    \n LABEL $int2float\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \
    \n DEFVAR LF@par1\
    \n MOVE LF@par1 LF@%%1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFEQ $int2float$par1 string@int LF@par1$type\
    \n EXIT int@6\
    \n LABEL $int2float$par1\
    \n INT2FLOAT LF@_ret1 LF@par1\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_float2int(){
    printf("\
    \n LABEL $float2int\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \
    \n DEFVAR LF@par1\
    \n MOVE LF@par1 LF@%%1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFEQ $float2int$par1 string@float LF@par1$type\
    \n EXIT int@6\
    \n LABEL $float2int$par1\
    \n FLOAT2INT LF@_ret1 LF@par1\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_len(){
    printf("\
    \n LABEL $len\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \
    \n DEFVAR LF@par1\
    \n MOVE LF@par1 LF@%%1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFEQ $len$par1 string@string LF@par1$type\
    \n EXIT int@6\
    \n LABEL $len$par1\
    \
    \n STRLEN LF@_ret1 LF@par1\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_substr(){
    printf("\
    \n LABEL $substr\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \n DEFVAR LF@_ret2\
    \n MOVE LF@_ret2 int@0\
    \
    \n DEFVAR LF@par1\
    \n MOVE LF@par1 LF@%%1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFEQ $substr$par1 string@string LF@par1$type\
    \n EXIT int@6\
    \n LABEL $substr$par1\
    \
    \n DEFVAR LF@par2\
    \n MOVE LF@par2 LF@%%2\
    \n DEFVAR LF@par2$type\
    \n TYPE LF@par2$type LF@par2\
    \n JUMPIFEQ $substr$par2 string@int LF@par2$type\
    \n EXIT int@6\
    \n LABEL $substr$par2\
    \
    \n DEFVAR LF@par3\
    \n MOVE LF@par3 LF@%%3\
    \n DEFVAR LF@par3$type\
    \n TYPE LF@par3$type LF@par3\
    \n JUMPIFEQ $substr$par3 string@int LF@par3$type\
    \n EXIT int@6\
    \n LABEL $substr$par3\
    \
    \n DEFVAR LF@cmp1\
    \n DEFVAR LF@cmp2\
    \n DEFVAR LF@higherbound\
    \n STRLEN LF@higherbound LF@par2\
    \n SUB LF@higherbound int@1\
    \n LT LF@cmp1 LF@par2 int@0\
    \n GT LF@cmp2 LF@par2 LF@higherbound\
    \n JUMPIFNEQ $substr$cond LF@cmp1 LF@cmp2\
    \
    \n LT LF@cmp1 LFpar3 int@0\
    \n JUMPTIFEQ $substr$cond LF@cmp1 bool@true\
    \
    \n DEFVAR LF@sup\
    \n MOVE LF@sup nil@nil\
    \n LABEL $substr$loop\
    \n JUMPIFEQ $substr$loop_end LF@par3 int@0\
    \n SUB LF@par3 int@1\
    \n GETCHAR LF@sup LF@par1 LF@par2\
    \n CONCAT LF@_ret1 LF@_ret1 LF@sup\
    \n JUMPIFEQ $substr$loop_end LF@par2 LF@higherbound\
    \n ADD LF@par2 int@1\
    \n JUMP $substr$loop\
    \n LABEL $substr$loop_end\
    \n POPFRAME\
    \n RETURN\
    \
    \n LABEL $substr$cond\
    \n MOVE LF@_ret2 int@1\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_ord(){
    printf("\
    \n LABEL $ord\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \n DEFVAR LF@_ret2\
    \n MOVE LF@_ret2 int@0\
    \
    \n DEFVAR LF@par1\
    \n MOVE LF@par1 LF@%%1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFEQ $ord$par1 string@string LF@par1$type\
    \n EXIT int@6\
    \n LABEL $ord$par1\
    \
    \n DEFVAR LF@par2\
    \n MOVE LF@par2 LF@%%2\
    \n DEFVAR LF@par2$type\
    \n TYPE LF@par2$type LF@par2\
    \n JUMPIFEQ $ord$par2 string@int LF@par2$type\
    \n EXIT int@6\
    \n LABEL $ord$par2\
    \
    \n DEFVAR LF@cmp1\
    \n DEFVAR LF@cmp2\
    \n DEFVAR LF@higherbound\
    \n STRLEN LF@higherbound LF@par1\
    \n SUB LF@higherbound int@1\
    \n LT LF@cmp1 LF@par1 int@0\
    \n GT LF@cmp2 LF@par1 LF@higherbound\
    \n JUMPIFEQ $ord$cond LF@cmp1 LF@cmp2\
    \n MOVE LF@_ret2 int@1\
    \n POPFRAME\
    \n RETURN\
    \n LABEL $ord$cond\
    \n STRI2INT LF@_ret1 LF@par1 LF@par2\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_chr(){
    printf("\
    \n LABEL $chr\
    \n PUSHFRAME\
    \n DEFVAR LF@_ret1\
    \n MOVE LF@_ret1 nil@nil\
    \n DEFVAR LF@_ret2\
    \n MOVE LF@_ret2 int@0\
    \
    \n DEFVAR LF@par1\
    \n MOVE LF@par1 LF@%%1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFEQ $chr$par1 string@int LF@par1$type\
    \n EXIT int@6\
    \n LABEL $chr$par1\
    \
    \n DEFVAR LF@cmp1\
    \n DEFVAR LF@cmp2\
    \n LT LF@cmp1 LF@par1 int@0\
    \n GT LF@cmp2 LF@par1 int@255\
    \n JUMPIFEQ $chr$cond LF@cmp1 LF@cmp2\
    \n MOVE LF@_ret2 int@1\
    \n POPFRAME\
    \n RETURN\
    \n LABEL $chr$cond\
    \n INT2CHAR LF@_ret1 LF@par1\
    \n POPFRAME\
    \n RETURN\
    \n");
}