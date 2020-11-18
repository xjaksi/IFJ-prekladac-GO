
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
 * @brief Knihovna pro dynamicString.c
 * -----------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include "dynamicString.h"



tStr *str_Init(int *err_number){

    tStr *p_str;

    // allokace struktury samotneho tStr
    p_str = malloc(sizeof(tStr));

    // kontrola allokace
    if(p_str == NULL){
        *err_number = 1;
        return NULL;
    }

    // allokace struktury str
    p_str->str = malloc(STRING_LEN * sizeof(char));

    // kontrola allokace
    if(p_str->str == NULL){
        free(p_str);
        *err_number = 1;
        return NULL;
    }

    // prirazeni pocatecnich hodnot
    p_str->str[0] = '\0';
    p_str->length = 0;
    p_str->alloc_Size = STRING_LEN;

    return p_str;  
}

void str_Dispose(tStr * p_str){
    free(p_str->str);
    free(p_str);
    p_str = NULL;
}


int str_Append(tStr *p_str, char c){
    
    // kontrola zda parametr ukazuje na nejakou strukturu
    if(p_str == NULL){
        return 1;
    }

    if(p_str->length == p_str->alloc_Size - RESERVED){
        p_str->str = realloc(p_str->str,(p_str->alloc_Size + STRING_LEN) * sizeof(char));

        // kontrola uspesnosti reallokace
        if(p_str->str == NULL){
            return 1;
        }

        p_str->alloc_Size = p_str->alloc_Size + STRING_LEN;
    }

    // konkatenace znaku
    p_str->str[p_str->length] = c;
    p_str->length += 1;
    p_str->str[p_str->length] = '\0';

    
    return 0;
}

int str_Compare(tStr *p_str1, tStr *p_str2){

    // kontrola zda dane stringy nejsou NULL
    if(p_str1 == NULL || p_str2 == NULL){
        return 99;
    }

    // c funkce pro kontrolu retezcu
    return strcmp(p_str1->str,p_str2->str);
}

int str_Compare_char(tStr *p_str, char *c_arr){
    
    // kontrola zda dane stringy nejsou NULL
    if(p_str == NULL || c_arr == NULL){
        return 99;
    }

    // c funkce pro kontrolu retezcu
    return strcmp(p_str->str, c_arr);;
}
