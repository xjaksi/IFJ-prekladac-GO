

#include <stdlib.h>
#include <string.h>
#include "dynamicString.h"

int str_Init(tStr *p_str){

    // kontrola zda parametr ukazuje na nejakou strukturu
    if(p_str == NULL){
        return 1;
    }

    // allokace struktury
    p_str->str = malloc(STRING_LEN * sizeof(char));

    // kontrola allokace
    if(p_str == NULL){
        return 1;
    }

    // prirazeni pocatecnich hodnot
    p_str->str[0] = '\0';
    p_str->length = 0;
    p_str->alloc_Size = STRING_LEN;

    return 0;  
}

void str_Dispose(tStr * p_str){
    free(p_str);
    p_str == NULL;
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

}