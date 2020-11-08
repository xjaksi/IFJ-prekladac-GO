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

typedef struct s_token {
	TokenType type;
	char value[50];
} s_token;

void get_token();