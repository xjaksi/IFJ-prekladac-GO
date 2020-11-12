/* ----------------------------------------------
 	IFJ prekladac jazyka IFJ20
	Tým XX:
		Jaksik, Ales (xjaksi01)
		Vlasakova, Nela (xvlasa14)
		Belohlavek, Jan (xbeloh8)
-------------------------------------------------
Ucel: rozpoznani a klasifikace lexemu, reprezentace lexemu pomoci tokenu
Popis:  jak funguje tento soubor
----------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynamicString.h"

#include "tokenList.h" // protototypy tokenlistu, tokenu, tokentype
#include "scanner.h"
#include "errors.h"

#define SCANNER_STATE_EOF 100
#define SCANNER_STATE_START 90


void getTokensTo(tokenList *tListMainPtr){ //fuknce pro precteni dat ze std. vstupu a ulozeni do seznamu , DKA
	DLInitList (tListMainPtr); //// nastavime vsechny pointry na NULL
	tStr teststr1;
	str_Init(&teststr1);
	str_Append(&teststr1, 'a' ); ///test pridani a
	str_Append(&teststr1, 'b' ); ///test pridani a
	DLInsertLast(tListMainPtr, tID, &teststr1); /// pridani tokenu do tokenlistu z atributu funkce

	int state = SCANNER_START;
	char c; // pro načítání znaku
	tStr strPom;
	str_Init(&strPom);

	while (state != SCANNER_STATE_EOF)
	{
		
		c = getc(stdin);
		
	
		switch (state)
		{
		case SCANNER_STATE_START:
			if (c == EOF)
			{
				DLInsertLast(tListMainPtr, tEOF, NULL);
				break;
			}
			if ((c == '\n') || (c == '\r')) //EOL
			{
				DLInsertLast(tListMainPtr, tEOL, NULL);
				break;
			}
			if(c == ','){
                DLInsertLast(tListMainPtr, tCOMMA, NULL);
                str_clean(str);
                break;


			if (isalpha(c) || (c == '_'))
			{
				state = 9;								
				str_Append(&strPom, c ); ///test pridani c
				break;
			}
			
			
			break;
		case 9: // id nebo kw
            if(isalpha(c) || (c == '_') || (isdigit(c))){
                str_Append(&strPom, c ); /// pridani c do strKwOrId
            }
            else{
                ungetc(c,stdin);
                
                if (strcmp(strPom.str, "if") == 0){
                    DLInsertLast(tListMainPtr, kwIF, NULL);
                }
                else if (strcmp(strPom.str, "else") == 0){
                    DLInsertLast(tListMainPtr, kwELSE, NULL);
                }
                else if(strcmp(strPom.str, "for") == 0){
                    DLInsertLast(tListMainPtr, kwFOR, NULL);
                }
                else if(strcmp(strPom.str, "return") == 0){
                    DLInsertLast(tListMainPtr, kwRETURN, NULL);
                }
                else if(strcmp(strPom.str, "float64") == 0){
                    DLInsertLast(tListMainPtr, kwFLOAT64, NULL);
                }
                else if(strcmp(strPom.str, "func") == 0){
                    DLInsertLast(tListMainPtr, kwFUNC, NULL);
                }
				else if(strcmp(strPom.str, "string") == 0){
                    DLInsertLast(tListMainPtr, kwSTRING, NULL);
                }
				else if (strcmp(strPom.str, "int") == 0){
                    DLInsertLast(tListMainPtr, kwINT, NULL);
                }
				else if(strcmp(strPom.str, "package") == 0){
                    DLInsertLast(tListMainPtr, kwPACKAGE, NULL);
                }
                else // is tID
				{
					tStr sendedStr;
					sendedStr.str = malloc(strPom.length * sizeof(char));
					str_Copy(strPom, sendedStr); // musi se dodelat !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					DLInsertLast(tListMainPtr, tID, &sendedStr);
				}
				
                strPom.str=""; // muzu takto?? vynulovat strPom
                
                state = SCANNER_STATE_START;
                return OK;
            }
            break;
		default:
			break;
		}
	}
	


	return;
}
