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
	tASSIGN,	///< :=
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

	// klicove slova
	kwIF,		///< if
	kwELSE,		///< else
	kwFOR,		///< for
	kwRETURN,	///< return
	kwFLOAT64,	///< float64
	kwFUNC,		///< func
	kwSTRING,	///< string
	kwINT,		///< int

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
	TokenType type;
	char value[1000000];
	struct Token *next;
	struct Token *prev;
} *TokenPtr;

/**
 * @struct struktura reprezentujici boboustranne vazany seznam
 * @brief obsahuje první, poslední a aktivní prvek
 */
typedef struct tList{
	TokenPtr first;
	TokenPtr act;
	TokenPtr last;
	int numberTokensInList;
} *tListPtr;

void get_token(tListPtr tListMain);
tListPtr initScanner();