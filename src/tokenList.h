
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
 * @brief Knihovna pro tokenList.c
 * -----------------------------------------------*/

#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

#include<stdio.h>
#include<stdlib.h>
#include "dynamicString.h"
#include "scanner.h"

#define FALSE 0
#define TRUE 1



/**
 * @brief Typy tokenu a klicova slova
 */
typedef enum {
	tID,		///< typ ID
	tINT,		///< typ integer
	tFLOAT,		///< typ float
	tSTRING,	///< typ string
	tADD,		///< +
	tSUB,		///< -
	tDIV,		///< /
	tMUL,		///< *
	tDEF,		///< :=
	tASSIGN,	///< =
	tLT,		///< <
	tLEQ,		///< <=
	tGT,		///< >
	tGEQ,		///< >=
	tEQ,		///< ==
	tNEQ,		///< !=
	tLBRACKET,	///< (
	tRBRACKET,	///< )
	tLBRACE,	///< {
	tRBRACE,	///< }
	tSEMICOLON,	///< ;
	tDEVNULL,	///< _
	tEOL,		///< EOL
	tEOF,		///< EOF

	// klicove slova
	kwIF,		///< if
	kwELSE,		///< else
	kwFOR,		///< for
	kwRETURN,	///< return
	kwFLOAT64,	///< float64
	kwFUNC,		///< func
	kwSTRING,	///< string
	kwINT,		///< int
	kwPACKAGE,	///< package

	// vestavene funkce
	fINPUTS,	///< inputs
	fINPUTI,	///< inputi
	fINPUTF,	///< inputf
	fPRINT,		///< print
	fINT2FLOAT, ///< int2float
	fFLOAT2INT, ///< float2int
	fLEN,		///< len
	fSUBSTR,	///< substr
	fORD,		///< ord
	fCHR,		///< chr

	fMAIN		///< main
} TokenType;

 /**
 * @struct token reprezentujici jednu polozku v obousmerne vazanem seznamu
 * @brief obsahuje odkaz na predesly a nasledujici
 */
typedef struct Token {
	TokenType t_type;
	tStr *atribute;
	struct Token *lptr;
	struct Token *rptr;
} *TokenPtr;

/**
 * @struct struktura reprezentujici boboustranne vazany seznam
 * @brief obsahuje první, poslední a aktivní prvek
 */
typedef struct {                        
    TokenPtr First;
    TokenPtr Act;
    TokenPtr Last;
} tokenList;

 /* prototypy jednotlivých funkcí */
void DLInitList (tokenList *);
void DLDisposeList (tokenList *);
void DLInsertLast(tokenList *, TokenType, tStr *);    
void DLFirst (tokenList *);
void DLLast (tokenList *);
void DLCopy (tokenList *, TokenType *, tStr *);       
void DLSucc (tokenList *);
void DLPred (tokenList *);
int DLActive (tokenList *);

#endif //TOKEN_LIST_H
