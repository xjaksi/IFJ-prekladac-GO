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
	//char tmpStr[5000] = "\0";
	//int pos = 0;
	
	// Pouzije se v pripade, ze dany tokenType potrebuje ATRIBUT.
	// Po predani do SEZNAMU nastavit vzdy na NULL
	tStr *p_DS = NULL;	
	int isE = 0;

	while (state != SCANNER_STATE_EOF)
	{
		
		c = getc(stdin);

	
		switch (state)
		{
		case SCANNER_STATE_START:
			if (c == EOF)
			{
				DLInsertLast(tListMainPtr, tEOF, NULL);
				state = SCANNER_STATE_EOF;
				break;
			}
			if ((c == '\n') || (c == '\r')) //EOL
			{
				DLInsertLast(tListMainPtr, tEOL, NULL);
				break;
			}
			if (c == ','){
                DLInsertLast(tListMainPtr, tCOMMA, NULL);
                break;

			}
			if (c == '('){
                DLInsertLast(tListMainPtr, tLBRACKET, NULL);
                break;

			}
			if (c == ')'){
                DLInsertLast(tListMainPtr, tRBRACKET, NULL);
                break;

			}
			if (c == '{'){
                DLInsertLast(tListMainPtr, tLBRACE, NULL);
                break;

			}
			if (c == '}'){
                DLInsertLast(tListMainPtr, tRBRACE, NULL);
                break;

			}
			if (c == ';'){
                DLInsertLast(tListMainPtr, tSEMICOLON, NULL);
                break;

			}
			if (c == '_'){
                DLInsertLast(tListMainPtr, tDEVNULL, NULL);
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
					if(p_DS == NULL)
					{
					int DELETE;
					p_DS = str_Init(&DELETE); //TODO nastavit errnum
					}
					str_Append(p_DS, '0' ); /// pridani c do dynStr
					str_Append(p_DS, c ); /// pridani c do dynStr
					state = 6;
					break;
				}
				else if ((c == 'E') || (c == 'e'))
				{
					if(p_DS == NULL)
					{
					int DELETE;
					p_DS = str_Init(&DELETE); //TODO nastavit errnum
					}
					str_Append(p_DS, '0' ); /// pridani c do dynStr
					str_Append(p_DS, c ); /// pridani c do dynStr
					state = 7;
					break;
				}
				else if ((c == '*') || (c == '+') || (c == '-') || (c == '/') || (c == '%') || (c == ' ') || (c == ';') || (c == ',') || (c == ')') || (c == '\n') || (c == '\r'))
				{
					ungetc(c,stdin);

					if(p_DS == NULL)
					{
					int DELETE;
					p_DS = str_Init(&DELETE); //TODO nastavit errnum
					}
					str_Append(p_DS, '0' ); /// pridani c do dynStr
					DLInsertLast(tListMainPtr, tINT, p_DS);
					p_DS = NULL;
					state = SCANNER_STATE_START;								
					break;	
				}
				else
				{
					state = SCANNER_STATE_EOF;
					return ERROR_LEXICAL;
				}
				

			}
			if ((isdigit(c)) && (c != '0')){
				if(p_DS == NULL)
				{
					int DELETE;
					p_DS = str_Init(&DELETE); //TODO nastavit errnum
				}
				str_Append(p_DS, c ); /// pridani c do dynStr
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
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '-')
			{
				DLInsertLast(tListMainPtr, tSUB, NULL);
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '*')
			{
				DLInsertLast(tListMainPtr, tMUL, NULL);
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
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					DLInsertLast(tListMainPtr, tASSIGN, NULL);
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
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					DLInsertLast(tListMainPtr, tLT, NULL);
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
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					DLInsertLast(tListMainPtr, tGT, NULL);
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
                str_Append(p_DS, c ); /// pridani c do strKwOrId
            }
            else {
                ungetc(c,stdin);
                              
				if (strcmp(p_DS->str, "if") == 0){	
                    DLInsertLast(tListMainPtr, kwIF, NULL);
                }
                else if (strcmp(p_DS->str, "else") == 0){
                    DLInsertLast (tListMainPtr, kwELSE, NULL);
                }
                else if(strcmp(p_DS->str, "for") == 0){
                    DLInsertLast(tListMainPtr, kwFOR, NULL);
                }
                else if(strcmp(p_DS->str, "return") == 0){
                    DLInsertLast(tListMainPtr, kwRETURN, NULL);
                }
                else if(strcmp(p_DS->str, "float64") == 0){
                    DLInsertLast(tListMainPtr, kwFLOAT64, NULL);
                }
                else if(strcmp(p_DS->str, "func") == 0){
                    DLInsertLast(tListMainPtr, kwFUNC, NULL);
                }
				else if(strcmp(p_DS->str, "string") == 0){
                    DLInsertLast(tListMainPtr, kwSTRING, NULL);
                }
				else if (strcmp(p_DS->str, "int") == 0){
                    DLInsertLast(tListMainPtr, kwINT, NULL);
                }
				else if(strcmp(p_DS->str, "package") == 0){
                    DLInsertLast(tListMainPtr, kwPACKAGE, NULL);
                }
				else if (strcmp(p_DS->str, "main") == 0){
                    DLInsertLast(tListMainPtr, fMAIN, NULL);
                } /*
				else if (strcmp(p_DS->str, "inputs") == 0){
                    DLInsertLast(tListMainPtr, fINPUTS, NULL);
                }
				else if (strcmp(p_DS->str, "inputi") == 0){
                    DLInsertLast(tListMainPtr, fINPUTI, NULL);
                }
				else if (strcmp(p_DS->str, "inputf") == 0){
                    DLInsertLast(tListMainPtr, fINPUTF, NULL);
                }
				else if (strcmp(p_DS->str, "print") == 0){
                    DLInsertLast(tListMainPtr, fPRINT, NULL);
                }
				else if (strcmp(p_DS->str, "int2float") == 0){
                    DLInsertLast(tListMainPtr, fINT2FLOAT, NULL);
                }
				else if (strcmp(p_DS->str, "float2int") == 0){
                    DLInsertLast(tListMainPtr, fFLOAT2INT, NULL);
                }
				else if (strcmp(p_DS->str, "len") == 0){
                    DLInsertLast(tListMainPtr, fLEN, NULL);
                }
				else if (strcmp(p_DS->str, "substr") == 0){
                    DLInsertLast(tListMainPtr, fSUBSTR, NULL);
                }
				else if (strcmp(p_DS->str, "ord") == 0){
                    DLInsertLast(tListMainPtr, fORD, NULL);
                }
				else if (strcmp(p_DS->str, "chr") == 0){
                    DLInsertLast(tListMainPtr, fCHR, NULL);
                } */
                else // is tID
				{
					DLInsertLast(tListMainPtr, tID, p_DS);
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
				DLInsertLast(tListMainPtr, tSTRING, p_DS);
				p_DS = NULL;
				state = SCANNER_STATE_START;								
				break;
			} 
			else
			{
				if(p_DS == NULL)
				{
					int DELETE;
					p_DS = str_Init(&DELETE); //TODO nastavit errnum
				}

				str_Append(p_DS, c ); /// pridani c do dynStr
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
				DLInsertLast(tListMainPtr, tSTRING, p_DS);
				p_DS = NULL;
				state = SCANNER_STATE_START;								
				break;
			} 
			else
			{
				if(p_DS == NULL)
				{
					int DELETE;
					p_DS = str_Init(&DELETE); //TODO nastavit errnum
				}

				str_Append(p_DS, c ); /// pridani c do dynStr
			}
				

			break;

		case 10:
			if (isdigit(c))
			{
				str_Append(p_DS, c ); /// pridani c do dynStr
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
			else if ((c == '*') || (c == '+') || (c == '-') || (c == '/') || (c == '%') || (c == ' ') || (c == ';') || (c == ',') || (c == ')') || (c == '\n') || (c == '\r'))
			{
                ungetc(c,stdin);
				DLInsertLast(tListMainPtr, tINT, p_DS);
				p_DS = NULL;
				state = SCANNER_STATE_START;								
				break;
			}
			else
			{
				state = SCANNER_STATE_START;			
				return ERROR_LEXICAL;
			}
			
			
			break;		

		case 6:
			if (isdigit(c))
			{
				str_Append(p_DS, c ); /// pridani c do dynStr
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
				str_Append(p_DS, c ); /// pridani c do dynStr
				break;
			}
			else if (((c == 'E') || (c == 'e')) && (isE == 0))
			{
				str_Append(p_DS, c ); /// pridani c do dynStr
				state = 7;
				break;
			}
			else if ((c == '*') || (c == '+') || (c == '-') || (c == '/') || (c == '%') || (c == ' ') || (c == ';') || (c == ',') || (c == ')') || (c == '\n') || (c == '\r')) 
			{
				ungetc(c,stdin);
				DLInsertLast(tListMainPtr, tFLOAT, p_DS);
				p_DS = NULL;
				isE = 0;
				state = SCANNER_STATE_START;								
				break;
			}
			else
			{
				state = SCANNER_STATE_EOF;
				return ERROR_LEXICAL;
			}
				
			break;

		case 7:
			isE = 1;
			if (isdigit(c))
			{
				str_Append(p_DS, c ); /// pridani c do dynStr
				state = 11;
				break;
			}
			else if((c == '+') || (c == '-'))
			{
				str_Append(p_DS, c ); /// pridani c do dynStr
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
				str_Append(p_DS, c ); /// pridani c do dynStr
				state = 11;
			}
			else
			{
				state = SCANNER_STATE_EOF;
				return ERROR_LEXICAL;
			}
			
			break;		

		default:
			break;
		}
		
	


	
	}
	return OK;
}
