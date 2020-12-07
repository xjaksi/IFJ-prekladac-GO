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
	
int erno_init_DS = 0;

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
				if(DLInsertLast(tListMainPtr, tEOF, NULL) != 0);
					return ERROR_COMPILER;
				state = SCANNER_STATE_EOF;
				break;
			}
			if ((c == '\n') || (c == '\r')) //EOL
			{
				if(DLInsertLast(tListMainPtr, tEOL, NULL) != 0);
					return ERROR_COMPILER;
				break;
			}
			if (c == ','){
                if(DLInsertLast(tListMainPtr, tCOMMA, NULL) != 0);
					return ERROR_COMPILER;
                break;

			}
			if (c == '('){
                if(DLInsertLast(tListMainPtr, tLBRACKET, NULL) != 0);
					return ERROR_COMPILER;
                break;

			}
			if (c == ')'){
                if(DLInsertLast(tListMainPtr, tRBRACKET, NULL) != 0);
					return ERROR_COMPILER;
                break;

			}
			if (c == '{'){
                if(DLInsertLast(tListMainPtr, tLBRACE, NULL) != 0);
					return ERROR_COMPILER;
                break;

			}
			if (c == '}'){
                if(DLInsertLast(tListMainPtr, tRBRACE, NULL) != 0);
					return ERROR_COMPILER;
                break;

			}
			if (c == ';'){
                if(DLInsertLast(tListMainPtr, tSEMICOLON, NULL) != 0);
					return ERROR_COMPILER;
                break;

			}
			if (c == '_'){
                if(DLInsertLast(tListMainPtr, tDEVNULL, NULL) != 0);
					return ERROR_COMPILER;
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
					p_DS = str_Init(&erno_init_DS); //TODO nastavit errnum
					if(erno_init_DS != 0);
						return ERROR_COMPILER;
					}
					if(str_Append(p_DS, '0' ) != 0); /// pridani c do dynStr
						return ERROR_COMPILER;
					if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
						return ERROR_COMPILER;
					state = 6;
					break;
				}
				else if ((c == 'E') || (c == 'e'))
				{
					if(p_DS == NULL)
					{
						p_DS = str_Init(&erno_init_DS); //TODO nastavit errnum
						if(erno_init_DS != 0);
							return ERROR_COMPILER;
					}
					if(str_Append(p_DS, '0' ) != 0); /// pridani c do dynStr
						return ERROR_COMPILER;
					if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
						return ERROR_COMPILER;
					state = 7;
					break;
				}
				else if ((c == '*') || (c == '+') || (c == '-') || (c == '/') || (c == '%') || (c == ' ') || (c == ';') || (c == ',') || (c == ')') || (c == '\n') || (c == '\r'))
				{
					ungetc(c,stdin);

					if(p_DS == NULL)
					{
					p_DS = str_Init(&erno_init_DS); //TODO nastavit errnum
					if(erno_init_DS != 0);
						return ERROR_COMPILER;
					}
					if(str_Append(p_DS, '0' ) != 0); /// pridani c do dynStr
						return ERROR_COMPILER;
					if(DLInsertLast(tListMainPtr, tINT, p_DS) != 0);
						return ERROR_COMPILER;
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
					p_DS = str_Init(&erno_init_DS); //TODO nastavit errnum
					if(erno_init_DS != 0);
						return ERROR_COMPILER;
				}
				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
				state = 10;
				break;

			}
			if ((isalpha(c)) || (c == '_'))
			{
				state = 9;	

				if(p_DS == NULL){
					p_DS = str_Init(&erno_init_DS); //TODO nastavit errnum
					if(erno_init_DS != 0);
						return ERROR_COMPILER;
				}

				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
				break;
			}
			if (c == ' ')
			{
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '+')
			{
				if(DLInsertLast(tListMainPtr, tADD, NULL) != 0);
					return ERROR_COMPILER;
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '-')
			{
				if(DLInsertLast(tListMainPtr, tSUB, NULL) != 0);
					return ERROR_COMPILER;
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '*')
			{
				if(DLInsertLast(tListMainPtr, tMUL, NULL) != 0);
					return ERROR_COMPILER;
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
					if(DLInsertLast(tListMainPtr, tDIV, NULL) != 0);
						return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '=')
			{
				c = getc(stdin);
				if (c == '=')
				{
					if(DLInsertLast(tListMainPtr, tEQ, NULL) != 0);
						return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					if(DLInsertLast(tListMainPtr, tASSIGN, NULL) != 0);
						return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == ':')
			{
				c = getc(stdin);
				if (c == '=')
				{
					if(DLInsertLast(tListMainPtr, tDEF, NULL) != 0);
						return ERROR_COMPILER;
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
					if(DLInsertLast(tListMainPtr, tLEQ, NULL) != 0);
						return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					if(DLInsertLast(tListMainPtr, tLT, NULL) != 0);
						return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '>')
			{
				c = getc(stdin);
				if (c == '=')
				{
					if(DLInsertLast(tListMainPtr, tGEQ, NULL) != 0);
						return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					if(DLInsertLast(tListMainPtr, tGT, NULL) != 0);
						return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '!')
			{
				c = getc(stdin);
				if (c == '=')
				{
					if(DLInsertLast(tListMainPtr, tNEQ, NULL) != 0);
						return ERROR_COMPILER;
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
                if(str_Append(p_DS, c ) != 0); /// pridani c do strKwOrId
					return ERROR_COMPILER;
            }
            else {
                ungetc(c,stdin);
                              
				if (strcmp(p_DS->str, "if") == 0){	
                    if(DLInsertLast(tListMainPtr, kwIF, NULL) != 0);
						return ERROR_COMPILER;
                }
                else if (strcmp(p_DS->str, "else") == 0){
                    if(DLInsertLast (tListMainPtr, kwELSE, NULL) != 0);
						return ERROR_COMPILER;
                }
                else if(strcmp(p_DS->str, "for") == 0){
                    if(DLInsertLast(tListMainPtr, kwFOR, NULL) != 0);
						return ERROR_COMPILER;
                }
                else if(strcmp(p_DS->str, "return") == 0){
                    if(DLInsertLast(tListMainPtr, kwRETURN, NULL) != 0);
						return ERROR_COMPILER;
                }
                else if(strcmp(p_DS->str, "float64") == 0){
                    if(DLInsertLast(tListMainPtr, kwFLOAT64, NULL) != 0);
						return ERROR_COMPILER;
                }
                else if(strcmp(p_DS->str, "func") == 0){
                    if(DLInsertLast(tListMainPtr, kwFUNC, NULL) != 0);
						return ERROR_COMPILER;
                }
				else if(strcmp(p_DS->str, "string") == 0){
                    if(DLInsertLast(tListMainPtr, kwSTRING, NULL) != 0);
						return ERROR_COMPILER;
                }
				else if (strcmp(p_DS->str, "int") == 0){
                    if(DLInsertLast(tListMainPtr, kwINT, NULL) != 0);
						return ERROR_COMPILER;
                }
				else if(strcmp(p_DS->str, "package") == 0){
                    if(DLInsertLast(tListMainPtr, kwPACKAGE, NULL) != 0);
						return ERROR_COMPILER;
                }
				else if (strcmp(p_DS->str, "main") == 0){
                    if(DLInsertLast(tListMainPtr, fMAIN, NULL) != 0);
						return ERROR_COMPILER;
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
					if(DLInsertLast(tListMainPtr, tID, p_DS) != 0);
						return ERROR_COMPILER;
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
				if(DLInsertLast(tListMainPtr, tEOL, NULL) != 0);
					return ERROR_COMPILER;
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
				if(DLInsertLast(tListMainPtr, tSTRING, p_DS) != 0);
					return ERROR_COMPILER;
				p_DS = NULL;
				state = SCANNER_STATE_START;								
				break;
			} 
			else
			{
				if(p_DS == NULL)
				{
					p_DS = str_Init(&erno_init_DS); //TODO nastavit errnum
					if(erno_init_DS != 0);
						return ERROR_COMPILER;
				}

				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
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
				if(DLInsertLast(tListMainPtr, tSTRING, p_DS) != 0);
					return ERROR_COMPILER;
				p_DS = NULL;
				state = SCANNER_STATE_START;								
				break;
			} 
			else
			{
				if(p_DS == NULL)
				{
					p_DS = str_Init(&erno_init_DS); //TODO nastavit errnum
					if(erno_init_DS != 0);
						return ERROR_COMPILER;
				}

				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
			}
				

			break;

		case 10:
			if (isdigit(c))
			{
				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
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
				if(DLInsertLast(tListMainPtr, tINT, p_DS) != 0);
					return ERROR_COMPILER;
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
				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
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
				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
				break;
			}
			else if (((c == 'E') || (c == 'e')) && (isE == 0))
			{
				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
				state = 7;
				break;
			}
			else if ((c == '*') || (c == '+') || (c == '-') || (c == '/') || (c == '%') || (c == ' ') || (c == ';') || (c == ',') || (c == ')') || (c == '\n') || (c == '\r')) 
			{
				ungetc(c,stdin);
				if(DLInsertLast(tListMainPtr, tFLOAT, p_DS) != 0);
					return ERROR_COMPILER;
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
				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
				state = 11;
				break;
			}
			else if((c == '+') || (c == '-'))
			{
				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
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
				if(str_Append(p_DS, c ) != 0); /// pridani c do dynStr
					return ERROR_COMPILER;
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
