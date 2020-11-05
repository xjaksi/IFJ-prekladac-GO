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
	tRBRACKET,	// (
	tLBRACKET,	// )
	tRBRACE,	// {
	tLBRACE,	// }
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

/**
 * @struct struktura pro token
   @brief obsahuje udaje o tokenu a ukazatele na praveho a leveho souseda
 */
typedef struct Token{
	TokenType type;
	char value[50];
	struct Token *lptr;
	struct Token *rptr;
} token;

void get_token();