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
#include <ctype.h>


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
	
	// Pouzije se v pripade, ze dany tokenType potrebuje ATRIBUT.
	// Po predani do SEZNAMU nastavit vzdy na NULL
	tStr *p_DS = NULL;	

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
			if (c == '0'){

				c = getc(stdin);
				if (isdigit(c))
				{
					state = SCANNER_STATE_EOF;
					return ERROR_LEXICAL;
				}
				else if (c == '.')
				{
					state = 6;
					break;
				}
				else if ((c == 'E') || (c == 'e'))
				{
					state = 7;
					break;
				}
				else
				{
					ungetc(c,stdin);
					DLInsertLast(tListMainPtr, tINT, NULL);
					printf("tINT c: %c\n", c);
					state = SCANNER_STATE_START;								
					break;	
				}

			}
			if ((isdigit(c)) && (c != '0')){
				state = 10;
				break;

			}
			if ((isalpha(c)) || (c == '_'))
			{
				state = 9;	

				if(p_DS == NULL){
					int DELETE;
					p_DS = str_Init(&DELETE); //TODO nastavit errnum
				}

				str_Append(p_DS, c ); /// pridani c do dynStr
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
			if (c == '\'')
			{
				state = 4;
				break;
			}
			if (c == '\"')
			{
				state = 5;
				break;
			}

			
			
			break;
		case 9: // id nebo kw
			

            if(isalpha(c) || (c == '_') || (isdigit(c))){
				printf("TmpStr: %s\n", p_DS);
                str_Append(p_DS, c ); /// pridani c do strKwOrId
            }
            else{
                ungetc(c,stdin);
                              
				if (strcmp(p_DS->str, "if") == 0){	
                    DLInsertLast(tListMainPtr, kwIF, NULL);
					printf("kwIF: %c\n", c);
                }
                else if (strcmp(p_DS->str, "else") == 0){
                    DLInsertLast (tListMainPtr, kwELSE, NULL);
					printf("kwELSE c: %c\n", c);
                }
                else if(strcmp(p_DS->str, "for") == 0){
                    DLInsertLast(tListMainPtr, kwFOR, NULL);
					printf("kwFOR c: %c\n", c);
                }
                else if(strcmp(p_DS->str, "return") == 0){
                    DLInsertLast(tListMainPtr, kwRETURN, NULL);
					printf("kwRETURN c: %c\n", c);
                }
                else if(strcmp(p_DS->str, "float64") == 0){
                    DLInsertLast(tListMainPtr, kwFLOAT64, NULL);
					printf("kwFLOAT64 c: %c\n", c);
                }
                else if(strcmp(p_DS->str, "func") == 0){
                    DLInsertLast(tListMainPtr, kwFUNC, NULL);
					printf("kwFUNC c: %c\n", c);
                }
				else if(strcmp(p_DS->str, "string") == 0){
                    DLInsertLast(tListMainPtr, kwSTRING, NULL);
					printf("kwSTRING c: %c\n", c);
                }
				else if (strcmp(p_DS->str, "int") == 0){
                    DLInsertLast(tListMainPtr, kwINT, NULL);
					printf("kwINT c: %c\n", c);
                }
				else if(strcmp(p_DS->str, "package") == 0){
                    DLInsertLast(tListMainPtr, kwPACKAGE, NULL);
					printf("kwPACKAGE c: %c\n", c);
                }
				else if (strcmp(p_DS->str, "main") == 0){
                    DLInsertLast(tListMainPtr, fMAIN, NULL);
					printf("fMAIN c: %c\n", c);
                }			
                else // is tID
				{
					DLInsertLast(tListMainPtr, tID, p_DS);
					printf("tID: %s\n", p_DS->str);
				}	

				if(tListMainPtr->Last->t_type != tID){
					str_Dispose(p_DS);
				}			
				p_DS = NULL;
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
			
			if (c == EOF)
			{
				state = SCANNER_STATE_EOF;			
				return ERROR_LEXICAL;
			}
			if ((c == '\n') || (c == '\r')) //EOL
			{
				DLInsertLast(tListMainPtr, tEOL, NULL);
				printf("tEOL c: %c\n", c);
				state = SCANNER_STATE_START;
				break;
			}		
			
			break;

		case 4:
			if (c == EOF)
			{
				state = SCANNER_STATE_EOF;			
				return ERROR_LEXICAL;
			}
			else if ((c == '\n') || (c == '\r')) //EOL
			{
				state = SCANNER_STATE_EOF;			
				return ERROR_LEXICAL;
			}
			else if (c == '\'')
			{
				DLInsertLast(tListMainPtr, tSTRING, NULL);
				printf("tSTRING c: %c\n", c);
				state = SCANNER_STATE_START;								
				break;
			} 
			else
			{
				/* code */
			}
				

			break;

		case 5:
			if (c == EOF)
			{
				state = SCANNER_STATE_EOF;			
				return ERROR_LEXICAL;
			}
			else if ((c == '\n') || (c == '\r')) //EOL
			{
				state = SCANNER_STATE_EOF;			
				return ERROR_LEXICAL;
			}
			else if (c == '\"')
			{
				DLInsertLast(tListMainPtr, tSTRING, NULL);
				printf("tSTRING c: %c\n", c);
				state = SCANNER_STATE_START;								
				break;
			} 
			else
			{
				/* code */
			}
				

			break;

		case 10:
			if (isdigit(c))
			{
				
			}
			else if (c == '.')
			{
				state = 6;
				break;
			}
			else if ((c == 'E') || (c == 'e'))
			{
				state = 7;
				break;
			}
			else
			{
                ungetc(c,stdin);
				DLInsertLast(tListMainPtr, tINT, NULL);
				printf("tINT c: %c\n", c);
				state = SCANNER_STATE_START;								
				break;
			}
			
			break;		

		case 6:
			if (isdigit(c))
			{
				state = 11;
				break;
			}
			else
			{
				state = SCANNER_STATE_EOF;
				return ERROR_LEXICAL;
 			}


			break;

		case 11:
			if (isdigit(c))
			{

			}
			else if ((c == 'E') || (c == 'e'))
			{
				state = 7;
				break;
			}
			else
			{
				DLInsertLast(tListMainPtr, tFLOAT, NULL);
				printf("tFLOAT c: %c\n", c);
				state = SCANNER_STATE_START;								
				break;
			}
				
			break;

		case 7:
			if (isdigit(c))
			{
				state = 11;
				break;
			}
			else if((c == '+') || (c == '-'))
			{
				state = 8;
				break;
			}
			else
			{
				state = SCANNER_STATE_EOF;
				return ERROR_LEXICAL;
			}		

			break;

		case 8:
			if (isdigit(c))
			{
				state = 11;
			}
			else
			{
				state = SCANNER_STATE_EOF;
				return ERROR_LEXICAL;
			}
			
			break;		

		default:
			printf("defaultcase: %s\n", tmpStr);
			break;
		}
		
	


	
	}
	return OK;
}