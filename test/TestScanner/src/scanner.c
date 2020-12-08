/** -----------------------------------------------
 * @file scanner.c
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Belohlavek, Jan <xbeloh08>
 * ------------------------------------------------
 *		@author Jaksik, Ales <xjaksi01>
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief rozpoznani a klasifikace lexemu, reprezentace lexemu pomoci tokenu, dka
 * -----------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dynamicString.h" // prototyp dynamickeho stringu
#include "tokenList.h" // protototypy tokenlistu, tokenu, tokentype
#include "scanner.h" // hlavickovy soubor
#include "errors.h"

#define SCANNER_STATE_EOF 100
#define SCANNER_STATE_START 90


int getTokensTo(tokenList *tListMainPtr){ //fuknce pro precteni dat ze std. vstupu a ulozeni do seznamu , DKA
	DLInitList (tListMainPtr); //// nastavime vsechny pointry na NULL
	int erno_init_DS = 0;
	
	int state = SCANNER_STATE_START;
	char c; // pro načítání znaku
	tStr *p_DS = NULL; // Pouzije se v pripade, ze dany tokenType potrebuje ATRIBUT. Po predani do SEZNAMU nastavit vzdy na NULL.
	int isE = 0; // bool znaci jestli ve float je již 1x E

	while (state != SCANNER_STATE_EOF)
	{
		
		c = getc(stdin);
	
		switch (state)
		{
		case SCANNER_STATE_START:
			if (c == EOF)
			{
				if(DLInsertLast(tListMainPtr, tEOF, NULL) != 0) return ERROR_COMPILER;
				state = SCANNER_STATE_EOF;
				break;
			}
			if ((c == '\n') || (c == '\r')) //EOL
			{
				if(DLInsertLast(tListMainPtr, tEOL, NULL) != 0) return ERROR_COMPILER;
				break;
			}
			if (c == ','){
                if(DLInsertLast(tListMainPtr, tCOMMA, NULL) != 0) return ERROR_COMPILER;
                break;

			}
			if (c == '('){
                if(DLInsertLast(tListMainPtr, tLBRACKET, NULL) != 0) return ERROR_COMPILER;
                break;

			}
			if (c == ')'){
                if(DLInsertLast(tListMainPtr, tRBRACKET, NULL) != 0) return ERROR_COMPILER;
                break;

			}
			if (c == '{'){
                if(DLInsertLast(tListMainPtr, tLBRACE, NULL) != 0) return ERROR_COMPILER;
                break;

			}
			if (c == '}'){
                if(DLInsertLast(tListMainPtr, tRBRACE, NULL) != 0) return ERROR_COMPILER;
                break;

			}
			if (c == ';'){
                if(DLInsertLast(tListMainPtr, tSEMICOLON, NULL) != 0) return ERROR_COMPILER;
                break;

			}
			if (c == '_'){
                if(DLInsertLast(tListMainPtr, tDEVNULL, NULL) != 0) return ERROR_COMPILER;
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
					p_DS = str_Init(&erno_init_DS);
					if(erno_init_DS != 0) return ERROR_COMPILER;
					}
					if(str_Append(p_DS, '0' ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
					if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
					state = 6;
					break;
				}
				else if ((c == 'E') || (c == 'e'))
				{
					if(p_DS == NULL)
					{
						p_DS = str_Init(&erno_init_DS);
						if(erno_init_DS != 0) return ERROR_COMPILER;
					}
					if(str_Append(p_DS, '0' ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
					if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
					state = 7;
					break;
				}
				else
				{
					ungetc(c,stdin);

					if(p_DS == NULL)
					{
					p_DS = str_Init(&erno_init_DS); 
					if(erno_init_DS != 0) return ERROR_COMPILER;
					}
					if(str_Append(p_DS, '0' ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
					if(DLInsertLast(tListMainPtr, tINT, p_DS) != 0) return ERROR_COMPILER;
					p_DS = NULL;
					state = SCANNER_STATE_START;								
					break;	
				}

			}
			if ((isdigit(c)) && (c != '0')){
				if(p_DS == NULL)
				{
					p_DS = str_Init(&erno_init_DS);
					if(erno_init_DS != 0) return ERROR_COMPILER;
				}
				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
				state = 10;
				break;

			}
			if ((isalpha(c)) || (c == '_'))
			{
				state = 9;	

				if(p_DS == NULL){
					p_DS = str_Init(&erno_init_DS); 
					if(erno_init_DS != 0) return ERROR_COMPILER;
				}

				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
				break;
			}
			if (c == ' ' || (c == '	')) // odchytava mezeru a tab
			{
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '+')
			{
				if(DLInsertLast(tListMainPtr, tADD, NULL) != 0) return ERROR_COMPILER;
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '-')
			{
				if(DLInsertLast(tListMainPtr, tSUB, NULL) != 0) return ERROR_COMPILER;
				state = SCANNER_STATE_START;								
				break;
			}
			if (c == '*')
			{
				if(DLInsertLast(tListMainPtr, tMUL, NULL) != 0) return ERROR_COMPILER;
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
					if(DLInsertLast(tListMainPtr, tDIV, NULL) != 0) return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '=')
			{
				c = getc(stdin);
				if (c == '=')
				{
					if(DLInsertLast(tListMainPtr, tEQ, NULL) != 0) return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					if(DLInsertLast(tListMainPtr, tASSIGN, NULL) != 0) return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == ':')
			{
				c = getc(stdin);
				if (c == '=')
				{
					if(DLInsertLast(tListMainPtr, tDEF, NULL) != 0) return ERROR_COMPILER;
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
					if(DLInsertLast(tListMainPtr, tLEQ, NULL) != 0) return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					if(DLInsertLast(tListMainPtr, tLT, NULL) != 0) return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '>')
			{
				c = getc(stdin);
				if (c == '=')
				{
					if(DLInsertLast(tListMainPtr, tGEQ, NULL) != 0) return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					ungetc(c,stdin);
					if(DLInsertLast(tListMainPtr, tGT, NULL) != 0) return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;	
				}
				
			}
			if (c == '!')
			{
				c = getc(stdin);
				if (c == '=')
				{
					if(DLInsertLast(tListMainPtr, tNEQ, NULL) != 0) return ERROR_COMPILER;
					state = SCANNER_STATE_START;								
					break;
				}
				else
				{
					return ERROR_LEXICAL;	
				}
				
			}
			if (c == '\"')
			{
				state = 5;
				break;
			}
			else
			{
				return ERROR_LEXICAL;
			}			
			
			break;

		case 9: // id nebo kw
            if(isalpha(c) || (c == '_') || (isdigit(c)))
			{
                if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do strKwOrId
            }
            else
			{
                ungetc(c,stdin);
                              
				if (strcmp(p_DS->str, "if") == 0)
				{	
                    if(DLInsertLast(tListMainPtr, kwIF, NULL) != 0) return ERROR_COMPILER;
                }
                else if (strcmp(p_DS->str, "else") == 0)
				{
                	if(DLInsertLast (tListMainPtr, kwELSE, NULL) != 0) return ERROR_COMPILER;
                }
                else if(strcmp(p_DS->str, "for") == 0)
				{
                    if(DLInsertLast(tListMainPtr, kwFOR, NULL) != 0) return ERROR_COMPILER;
                }
                else if(strcmp(p_DS->str, "return") == 0)
				{
                    if(DLInsertLast(tListMainPtr, kwRETURN, NULL) != 0) return ERROR_COMPILER;
				}
                else if(strcmp(p_DS->str, "float64") == 0)
				{
                    if(DLInsertLast(tListMainPtr, kwFLOAT64, NULL) != 0) return ERROR_COMPILER;
                }
                else if(strcmp(p_DS->str, "func") == 0)
				{
                    if(DLInsertLast(tListMainPtr, kwFUNC, NULL) != 0) return ERROR_COMPILER;
                }
				else if(strcmp(p_DS->str, "string") == 0)
				{
                    if(DLInsertLast(tListMainPtr, kwSTRING, NULL) != 0) return ERROR_COMPILER;
                }
				else if (strcmp(p_DS->str, "int") == 0)
				{
                    if(DLInsertLast(tListMainPtr, kwINT, NULL) != 0) return ERROR_COMPILER;
                }
				else if(strcmp(p_DS->str, "package") == 0)
				{
                    if(DLInsertLast(tListMainPtr, kwPACKAGE, NULL) != 0) return ERROR_COMPILER;
                }
				else if (strcmp(p_DS->str, "main") == 0)
				{
                    if(DLInsertLast(tListMainPtr, fMAIN, NULL) != 0) return ERROR_COMPILER;
                } 
                else // is tID
				{
					if(DLInsertLast(tListMainPtr, tID, p_DS) != 0) return ERROR_COMPILER;
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
				if(DLInsertLast(tListMainPtr, tEOL, NULL) != 0) return ERROR_COMPILER;
				state = SCANNER_STATE_START;
				break;
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
			else if (c == '\"') // ukonceni stringu
			{
				if(DLInsertLast(tListMainPtr, tSTRING, p_DS) != 0) return ERROR_COMPILER;
				p_DS = NULL;
				state = SCANNER_STATE_START;								
				break;
			} 
			else if (c == '\\') //escape sekvence
			{ 
                c = getc(stdin);
                
				if (c == '\\')
				{
					if(str_Append(p_DS, '\\' ) != 0) return ERROR_COMPILER; /// pridani c do str
                }
                else if (c == '\"')
				{
					if(str_Append(p_DS, '\"' ) != 0) return ERROR_COMPILER; /// pridani c do str
                }
                else if (c == 'n' )
				{
                    if(str_Append(p_DS, '\n' ) != 0) return ERROR_COMPILER; /// pridani c do str
                }
                else if (c == 't')
				{
					if(str_Append(p_DS, '\t' ) != 0) return ERROR_COMPILER; /// pridani c do str
                }
				
                else if (c == 'x') // Hexadecimal hodnota
				{
                    char tmp[4];
                    tmp[0] = '0';
                    tmp[1] = (char)c;
                    for(int i = 2; i < 4; i++)
					{
                        c = getc(stdin);
                        if (isdigit(c) || (c == 'A') || (c == 'B') || (c == 'C') || (c == 'D') || (c == 'E') || (c == 'F') || (c == 'a') || (c == 'b') || (c == 'c') || (c == 'd') || (c == 'e') || (c == 'f'))
                    	{
                            tmp[i] = (char)c;
                        }
                        else
						{
                            state = SCANNER_STATE_EOF;			
							return ERROR_LEXICAL;
                        }
                    }

                    char hex;
                    // Convert to hex value
                    sscanf(tmp, "%hhx", &hex);
                    if(str_Append(p_DS, hex ) != 0) return ERROR_COMPILER; /// pridani c do str
                }
                else
				{
                    state = SCANNER_STATE_EOF;			
					return ERROR_LEXICAL;
                }
			}	
			else // pokracuje string, c se prida
			{
				if(p_DS == NULL)
				{
					p_DS = str_Init(&erno_init_DS);
					if(erno_init_DS != 0) return ERROR_COMPILER;
				}

				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
			}
			
			break;

		case 10:
			if (isdigit(c))
			{
				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
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
				if(DLInsertLast(tListMainPtr, tINT, p_DS) != 0) return ERROR_COMPILER;
				p_DS = NULL;
				state = SCANNER_STATE_START;								
				break;
			}
				
			
			break;		

		case 6:
			if (isdigit(c))
			{
				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
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
				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
				break;
			}
			else if (((c == 'E') || (c == 'e')) && (isE == 0))
			{
				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
				state = 7;
				break;
			}
			else
			{
				ungetc(c,stdin);
				if(DLInsertLast(tListMainPtr, tFLOAT, p_DS) != 0) return ERROR_COMPILER;
				p_DS = NULL;
				isE = 0;
				state = SCANNER_STATE_START;								
				break;
			}
			
			break;

		case 7:
			isE = 1;
			if (isdigit(c))
			{
				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
				state = 11;
				break;
			}
			else if((c == '+') || (c == '-'))
			{
				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
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
				if(str_Append(p_DS, c ) != 0) return ERROR_COMPILER; /// pridani c do dynStr
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
