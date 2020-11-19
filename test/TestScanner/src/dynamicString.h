
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
 * @brief Implementovane funkce pro praci s obousmerne vazanym seznamem tokenu
 * -----------------------------------------------*/

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
tStr *str_Init(int *);
void str_Dispose(tStr *);
int str_Append(tStr *, char );
int str_Compare(tStr *, tStr *);
int str_Compare_char(tStr *, char *);

// DELETE
// void str_Clean(tStr *);
// int str_Concat(tStr *, tStr *);
// int str_Copy(tStr *, tStr *);
// int str_Print(tStr *);


#endif // DYNAMIC_STRING_H