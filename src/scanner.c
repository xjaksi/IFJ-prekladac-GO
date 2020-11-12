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


int getTokensTo(tokenList *tListMainPtr){ //fuknce pro precteni dat ze std. vstupu a ulozeni do seznamu , DKA
	DLInitList (tListMainPtr); //// nastavime vsechny pointry na NULL
	
	int state = SCANNER_STATE_START;
	char c; // pro načítání znaku
	//tStr strPom;
	//str_Init(&strPom);
	char tmpStr[5000] = "\0";
	int pos = 0;
	

	while (state != SCANNER_STATE_EOF)
	{
		
		c = getc(stdin);
		//printf("while start c: %c\n", c);
	
		switch (state)
		{
		case SCANNER_STATE_START:
			if (c == EOF)
			{
				DLInsertLast(tListMainPtr, tEOF, NULL);
				state = SCANNER_STATE_EOF;
				printf("tEOF c: %c\n", c);
				break;
			}
			if ((c == '\n') || (c == '\r')) //EOL
			{
				DLInsertLast(tListMainPtr, tEOL, NULL);
				printf("tEOL c: %c\n", c);
				break;
			}
			if (c == ','){
                DLInsertLast(tListMainPtr, tCOMMA, NULL);
				printf("tCOMMA c: %c\n", c);
                break;

			}
			if (c == '('){
                DLInsertLast(tListMainPtr, tLBRACKET, NULL);
				printf("tLBRACKET c: %c\n", c);
                break;

			}
			if (c == ')'){
                DLInsertLast(tListMainPtr, tRBRACKET, NULL);
				printf("tRBRACKET c: %c\n", c);
                break;

			}
			if (c == '{'){
                DLInsertLast(tListMainPtr, tLBRACE, NULL);
				printf("tLBRACE c: %c\n", c);
                break;

			}
			if (c == '}'){
                DLInsertLast(tListMainPtr, tRBRACE, NULL);
				printf("tRBRACE c: %c\n", c);
                break;

			}
			if (c == ';'){
                DLInsertLast(tListMainPtr, tSEMICOLON, NULL);
				printf("tSEMICOLON c: %c\n", c);
                break;

			}
			if (c == '_'){
                DLInsertLast(tListMainPtr, tDEVNULL, NULL);
				printf("tDEVNULL c: %c\n", c);
                break;

			}
			if ((isalpha(c)) || (c == '_'))
			{
				state = 9;	
				tmpStr[pos] = c;
				pos++;
      			tmpStr[pos] = '\0';						
				//str_Append(&strPom, c ); ///test pridani c
				break;
			}
			if (c == ' ')
			{
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '+')
			{
				DLInsertLast(tListMainPtr, tADD, NULL);
				printf("tADD c: %c\n", c);
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '-')
			{
				DLInsertLast(tListMainPtr, tSUB, NULL);
				printf("tSUB c: %c\n", c);
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '*')
			{
				DLInsertLast(tListMainPtr, tMUL, NULL);
				printf("tMUL c: %c\n", c);
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '/')
			{
				c = getc(stdin);
				if (c == '*')
				{
					state = 2;
					break;
				}
				else if (c == '/')
				{
					state = 1;
					break;
				}
				else
				{
					ungetc(c,stdin);
					DLInsertLast(tListMainPtr, tDIV, NULL);
					printf("tDIV c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '=')
			{
				c = getc(stdin);
				if (c == '=')
				{
					DLInsertLast(tListMainPtr, tEQ, NULL);
					printf("tEQ c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					DLInsertLast(tListMainPtr, tASSIGN, NULL);
					printf("tASSIGN c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == ':')
			{
				c = getc(stdin);
				if (c == '=')
				{
					DLInsertLast(tListMainPtr, tDEF, NULL);
					printf("tDEF c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					return ERROR_LEXICAL;	
				}
				
			}
			if (c == '<')
			{
				c = getc(stdin);
				if (c == '=')
				{
					DLInsertLast(tListMainPtr, tLEQ, NULL);
					printf("tLEQ c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					DLInsertLast(tListMainPtr, tLT, NULL);
					printf("tLT c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '>')
			{
				c = getc(stdin);
				if (c == '=')
				{
					DLInsertLast(tListMainPtr, tGEQ, NULL);
					printf("tLEQ c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					DLInsertLast(tListMainPtr, tGT, NULL);
					printf("tLT c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '!')
			{
				c = getc(stdin);
				if (c == '=')
				{
					DLInsertLast(tListMainPtr, tNEQ, NULL);
					printf("tNEQ c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					return ERROR_LEXICAL;	
				}
				
			}

			
			
			break;
		case 9: // id nebo kw
            if(isalpha(c) || (c == '_') || (isdigit(c))){
				tmpStr[pos] = c;
				pos++;
      			tmpStr[pos] = '\0';
				//printf("TmpStr: %s\n", tmpStr);
                //str_Append(&strPom, c ); /// pridani c do strKwOrId
            }
            else{
                ungetc(c,stdin);
                
                //if (strcmp(strPom.str, "if") == 0){
				if (strcmp(tmpStr, "if") == 0){	
                    DLInsertLast(tListMainPtr, kwIF, NULL);
					printf("kwIF: %c\n", c);
                }
                else if (strcmp(tmpStr, "else") == 0){
                    DLInsertLast(tListMainPtr, kwELSE, NULL);
					printf("kwELSE c: %c\n", c);
                }
                else if(strcmp(tmpStr, "for") == 0){
                    DLInsertLast(tListMainPtr, kwFOR, NULL);
					printf("kwFOR c: %c\n", c);
                }
                else if(strcmp(tmpStr, "return") == 0){
                    DLInsertLast(tListMainPtr, kwRETURN, NULL);
					printf("kwRETURN c: %c\n", c);
                }
                else if(strcmp(tmpStr, "float64") == 0){
                    DLInsertLast(tListMainPtr, kwFLOAT64, NULL);
					printf("kwFLOAT64 c: %c\n", c);
                }
                else if(strcmp(tmpStr, "func") == 0){
                    DLInsertLast(tListMainPtr, kwFUNC, NULL);
					printf("kwELSE c: %c\n", c);
                }
				else if(strcmp(tmpStr, "string") == 0){
                    DLInsertLast(tListMainPtr, kwSTRING, NULL);
					printf("kwSTRING c: %c\n", c);
                }
				else if (strcmp(tmpStr, "int") == 0){
                    DLInsertLast(tListMainPtr, kwINT, NULL);
					printf("kwINT c: %c\n", c);
                }
				else if(strcmp(tmpStr, "package") == 0){
                    DLInsertLast(tListMainPtr, kwPACKAGE, NULL);
					printf("kwPACKAGE c: %c\n", c);
                }
                else // is tID
				{
					//tStr sendedStr;
					//sendedStr.str = malloc(strPom.length * sizeof(char));
					//str_Copy(strPom, sendedStr); // musi se dodelat !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
					DLInsertLast(tListMainPtr, tID, NULL);
					printf("tID: %s\n", tmpStr);
				}
				
      			
				for (int i = pos; i < 0; i--)
				{
					tmpStr[i]= '\0';
				}
				pos = 0;
                //strPom.str=""; // muzu takto?? vynulovat strPom
                
                state = SCANNER_STATE_START;
                
            }
            break;
		case 2:
			if (c == '*')
			{
				c = getc(stdin);
				if (c == '/')
				{
					state = SCANNER_STATE_START;															
					break;
				}
				if (c == EOF){		
					state = SCANNER_STATE_EOF;			
					return ERROR_LEXICAL;
				}
				else
				{
					ungetc(c,stdin);
				}								
				
			}
			if (c == EOF)
			{		
				state = SCANNER_STATE_EOF;			
				return ERROR_LEXICAL;
			}
			
			break;
		
		case 1:
			if ((c == '\n') || (c == '\r')) //EOL
			{
				state =  SCANNER_STATE_START;
				DLInsertLast(tListMainPtr, tEOL, NULL);
				printf("tEOL c: %c\n", c);
				break;
			}	
			if (c == EOF)
			{
				state =  SCANNER_STATE_START;
				DLInsertLast(tListMainPtr, tEOF, NULL);
				printf("tEOF c: %c\n", c);															
				break;
			}
					
			
			break;

		default:
			printf("defaultcase: %s\n", tmpStr);
			break;
		}
		
	


	
	}
	return OK;
}