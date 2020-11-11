

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STRING_LEN 10
#define RESERVED 5

typedef struct
{
	char *str;      // ukazatel na zacatek stringu
	int length;     // pocet znaku v stringu vcetne '\0'
	int alloc_Size;  // pocet allokovanych znaku
} tStr;

// prototypy funkci
int str_Init(tStr *);
void str_Dispose(tStr *);
int str_Append(tStr *, char );
int str_Compare(tStr *, tStr *);

// DELETE
// void str_Clean(tStr *);
// int str_Concat(tStr *pram_str1, tStr *param_str2);
// int str_Copy(tStr *pram_str1, tStr *param_str2);
// int str_Compare_char(tString *s1, char* s2);
// int str_Print(tString *s);


#endif // DYNAMIC_STRING_H