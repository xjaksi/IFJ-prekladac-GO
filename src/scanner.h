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

struct s_token {
	enum s_type type;
	char value[50];
}s_token;

typedef enum {
	id,//identifikator nazev promene, funkce 
	int,// integer
	add,// +
	sub,// -
	div,// /
	sub,// *
	,// =
	,//<
	leq,//<=
	neco,// >
	neco2,// >=
	neco3,// (
	neco4,// )
	;,// ;
	if,
	else,
	case,
	while,
	for,
	return
}s_type;