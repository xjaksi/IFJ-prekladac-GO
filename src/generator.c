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
    \n .IFJcode20\
    \n DEFVAR GF@$$res\
    \n DEFVAR GF@$$operand_1\
    \n DEFVAR GF@$$operand_2\
    \n DEFVAR GF@$$type_1\
    \n DEFVAR GF@$$type_2\
    \n JUMP $$main\n\
    ");
    
    // generovani vestavenych funkci
    gen_print();
    gen_inputs();
    gen_itputi();
    gen_inputf();
    gen_int2float();
    gen_float2int();
    gen_len();
    gen_ord();
    gen_chr();

    /*
    gen_substr();
    */
   // generovani pomocnych funkci
   gen_get_stack_bool();

}

// generovani zacatku mainu
void gen_main(){
    printf("\
    \n LABEL $$main\
    \n CREATEFRAME\
    \n PUSHFRAME\
    ");
}


// generovani promennych
void gen_defvar(char *name){
    printf("\n DEFVAR LF@$%s", name);
    printf("\n POPS LF@$%s", name);
}

// funkce

void gen_func_tf_var(TokenPtr token, int par_num){

    par_num++;

    printf("\n DEFVAR TF@*par%d", par_num);

    switch(token->t_type){
        case tINT:
            printf("\n MOVE TF@*par%d int@%s", par_num, token->atribute->str);
            break;
        case tFLOAT:
            printf("\n MOVE TF@*par%d ",par_num);
            print_float(token->atribute->str);
            break;
        case tSTRING:
            printf("\n MOVE TF@*par%d ",par_num);
            print_string(token->atribute->str);
            break;
        case tID:
            printf("\n MOVE TF@*par%d LF@$%s", par_num, token->atribute->str); // TODO nejaky id pridat
            break;
        default:
            break;
    }

}
void gen_func_call(char *func_name){
    printf("\n CALL $%s", func_name);
}

void gen_func_start(char *func_name){
    printf("\n LABEL $%s", func_name);
    printf("\n PUSHFRAME");
}

void gen_func_ret_start(int ret_num){
    printf("\n DEFVAR LF@_ret%d", ret_num);
    printf("\n MOVE LF@_ret%d nil@nil", ret_num);
}

void gen_func_param(char *par_name, int par_num){
    printf("\n DEFVAR LF@$%s", par_name);
    printf("\n MOVE LF@$%s LF@*par%d", par_name, par_num);
}

void gen_func_ret_val(int ret_num){
    printf("\n POPS LF@_ret%d", ret_num);
}

void gen_func_end(){
    printf("\n POPFRAME");
    printf("\n RETURN");
}

void gen_func_tf_ret(char *id_name, int ret_cnt){
    ret_cnt++;
    printf("\n MOVE LF@$%s TF@_ret%d",id_name, ret_cnt);
}

// generovani if_else_then
void gen_if(int if_id){
    printf("\n CALL $$get_stack_bool");
    printf("\n POPS GF@$$res");
    printf("\n JUMPIFEQ $$else_%d GF@$$res bool@false", if_id);
}

void gen_else(int if_id){
    printf("\n JUMP $$if_end_%d", if_id);
    printf("\n LABEL $$else_%d", if_id);
}

void gen_else_end(int if_id){
    printf("\n LABEL $$if_end_%d", if_id);
}

// generovani for
void gen_for(int for_id){
    printf("\n LABEL $$for_%d", for_id);
    //printf("\nJUMPIFEQ $for_end_%d", for_id);
}

void gen_for_end(int for_id){
    printf("\n LABEL $$for_%d", for_id);
    printf("\n JUMPIFEQ $$for_end_%d", for_id);
}

// generovani vyrazu
void gen_expresion(tokenList *l){
    
    /* DELETE
    DLFirst(l);
    while(l->Act != NULL){
        printf("\n %d", l->Act->t_type);
        if(l->Act->atribute != NULL){
            printf("\t\t%s", l->Act->atribute->str);
        }
        DLSucc(l);
    }

    */
    printf("\n"); //DELETE

    DLFirst(l);

    while(l->Act != NULL){
        switch(l->Act->t_type){
            case tID:
                printf("\n PUSHS LF@$%s", l->Act->atribute->str); // TODO nejaky identifikator id
                break;
            case tINT:
                printf("\n PUSHS int@%s", l->Act->atribute->str);
                break;
            case tFLOAT:
                printf("\n PUSHS ");
                print_float(l->Act->atribute->str);
                break;
            case tSTRING:
                printf("\n PUSHS ");
                print_string(l->Act->atribute->str);
                break;
            case tADD:
                printf("\n ADDS");
                break;
            case tSUB:
                printf("\n SUBS");
                break;
            case tDIV:
                // TODO
                break;
            case tMUL:
                printf("\n MULS");
                break;
            case tLT:
                printf("\n LTS");
                break;
            case tLEQ:
                /// TODO
                break;
            case tGT:
                printf("\n GTS");
                break;
            case tGEQ:
                // TODO
                break;
            case tEQ:
                printf("\n EQS");
                break;
            case tNEQ:
                // TODO
                break;
            default:
                break;
        }
        DLSucc(l);
    }

    printf("\n"); // DELETE

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
    \n DEFVAR LF@ret1$type\
    \n TYPE LF@ret1$type LF@_ret1\
    \n JUMPIFNEQ $inputs$cond LF@ret1$type string@nil\
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
    \n DEFVAR LF@ret1$type\
    \n TYPE LF@ret1$type LF@_ret1\
    \n JUMPIFNEQ $inputi$cond LF@ret1$type string@nil\
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
    \n DEFVAR LF@ret1$type\
    \n TYPE LF@ret1$type LF@_ret1\
    \n JUMPIFNEQ $inputf$cond LF@ret1$type string@nil\
    \n MOVE LF@_ret2 int@1\
    \n LABEL $inputf$cond\
    \n POPFRAME\
    \n RETURN\
    \n");
}

void gen_print(){
    printf("\
    \n LABEL $print\
    \n PUSHFRAME\
    \n DEFVAR LF@par1\
    \n MOVE LF@par1 LF@*par1\
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
    \n MOVE LF@par1 LF@*par1\
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
    \n MOVE LF@par1 LF@*par1\
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
    \n MOVE LF@par1 LF@*par1\
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
    \n MOVE LF@par1 LF@*par1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFEQ $substr$par1 string@string LF@par1$type\
    \n EXIT int@6\
    \n LABEL $substr$par1\
    \
    \n DEFVAR LF@par2\
    \n MOVE LF@par2 LF@*par2\
    \n DEFVAR LF@par2$type\
    \n TYPE LF@par2$type LF@par2\
    \n JUMPIFEQ $substr$par2 string@int LF@par2$type\
    \n EXIT int@6\
    \n LABEL $substr$par2\
    \
    \n DEFVAR LF@par3\
    \n MOVE LF@par3 LF@*par3\
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
    \n SUB LF@higherbound LF@higherbound int@1\
    \n LT LF@cmp1 LF@par2 int@0\
    \n GT LF@cmp2 LF@par2 LF@higherbound\
    \n JUMPIFNEQ $substr$cond LF@cmp1 LF@cmp2\
    \
    \n LT LF@cmp1 LF@par3 int@0\
    \n JUMPTIFEQ $substr$cond LF@cmp1 bool@true\
    \
    \n DEFVAR LF@sup\
    \n MOVE LF@sup nil@nil\
    \n LABEL $substr$loop\
    \n JUMPIFEQ $substr$loop_end LF@par3 int@0\
    \n SUB LF@par3 LF@par3 int@1\
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
    \n MOVE LF@par1 LF@*par1\
    \n DEFVAR LF@par1$type\
    \n TYPE LF@par1$type LF@par1\
    \n JUMPIFEQ $ord$par1 string@string LF@par1$type\
    \n EXIT int@6\
    \n LABEL $ord$par1\
    \
    \n DEFVAR LF@par2\
    \n MOVE LF@par2 LF@*par2\
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
    \n SUB LF@higherbound LF@higherbound int@1\
    \n LT LF@cmp1 LF@par2 int@0\
    \n GT LF@cmp2 LF@par2 LF@higherbound\
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
    \n MOVE LF@par1 LF@*par1\
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


void print_float(char *par){
    printf("float@%a",strtod(par,NULL));
}

void print_string(char *par){
    printf("string@");
    int i = 0;
    while(par[i] != '\0'){
        if(par[i] < 33){
            printf("\\0%d", par[i]);
        }else if(par[i] == 35){
            printf("\\0%d", par[i]);
        }else if(par[i] == 92){
            printf("\\0%d", par[i]);
        }else{
            putc(par[i], stdout);
        }
        i++;
    }

}

/* DELETE: udelat jinak, sak ja vim jak
int str_to_str(tStr *s){
    int i = 0;
    char old_str[s->length];
    strcpy(old_str, s->str);
    
    do{
        switch(s->str[i]){
            case '\010':
                str_Append(s, '\\');
                str_Append(s, '0');
                str_Append(s, '1');
                str_Append(s, '0');
                i += 4;
                break;
            case '\032':
                str_Append(s, '\\');
                str_Append(s, '0');
                str_Append(s, '3');
                str_Append(s, '2');
                i += 4;
                break;
            case '\035':
                str_Append(s, '\\');
                str_Append(s, '0');
                str_Append(s, '3');
                str_Append(s, '5');
                i+= 4;
                break;
            case '\\':
                str_Append(s, '\\');
                str_Append(s, '0');
                str_Append(s, '9');
                str_Append(s, '2');
                i += 4;
                break;
            default:
                i++;
                break;
        }
    }while(old_str[i] != '\0');

    return 0;
}
*/

// pomocne funkce (vestavene)

void gen_get_stack_bool(){
    printf("\
    \n LABEL $$get_stack_bool\
    \n POPS GF@$$operand_1\
    \n TYPE GF@$$type_1 GF@$$operand_1\
    \n JUMPIFEQ $$operand_int GF@$$type_1 string@int\
	\n JUMPIFEQ $$operand_float GF@$$type_1 string@float\
	\n JUMPIFEQ $$operand_string GF@$$type_1 string@string\
	\n JUMPIFEQ $$operand_false GF@$$type_1 string@nil\
    \n PUSHS GF@$$operand_1\
    \n RETURN\
    \n LABEL $$operand_int\
    \n JUMPIFEQ $$operand_false GF@$$operand_1 int@0\
    \n PUSHS bool@true\
    \n RETURN\
    \n LABEL $$operand_float\
    \n JUMPIFEQ $$operand_false GF@$$operand_1 float@0x0p+0\
    \n PUSHS bool@true\
    \n RETURN\
    \n LABEL $$operand_string\
    \n JUMPIFEQ $$operand_false GF@$$operand_1 string@nil\
    \n PUSHS bool@true\
    \n RETURN\
    \n LABEL $$operand_false\
    \n PUSHS bool@false\
    \n RETURN\
    \n");
}


