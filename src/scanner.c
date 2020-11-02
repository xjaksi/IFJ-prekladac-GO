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

#include <stdio.h>

#include <scanner.h>

int main(){
	printf("Hello world!!! \n");
	
return 0;
}

int index_in_input_data = 0; //ukazatel kde se nachazim v nactenych input datech

s_token get_token(){ //fuknce pro precteni noveho tokenu z input data , DKA
	s_token loaded_token; //token ktery se bude vracet
	loaded_token.type = if;
	loaded_token.value = "";
	printf("%s", loaded_token.type);

return get_token; // vrati novy token
}
