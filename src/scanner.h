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



/**
 * @brief Typy tokenu a klicova slova
 */
typedef enum {
	tID,		// typ ID 
	tINT,		// integer
	tFLOAT,		// float
	tSTRING,	// string
	tADD,		// +
	tSUB,		// -
	tDIV,		// /
	tMUL,		// *
	tASSIGN,	// :=
	tLT,		// <
	tLEQ,		// <=
	tGT,		// >
	tGEQ,		// >=
	tEQ,		// ==
	tNEQ,		// !=
	tLBRACKET,	// (
	tRBRACKET,	// )
	tLBRACE,	// {
	tRBRACE,	// }
	tSEMICOLON,	// ;
	tDEVNULL,	// _

	// klicove slova
	kwIF,		
	kwELSE,
	kwFOR,
	kwRETURN,
	kwFLOAT64,
	kwFUNC,
	kwSTRING,
	kwINT

} TokenType;

typedef struct s_token {
	TokenType type;
	char value[50];
} s_token;

void get_token();