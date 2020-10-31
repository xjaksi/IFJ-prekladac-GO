
# IFJ-prekladac-GO
### Povinná hlavička - vložit do každého souboru!!!
	/* ------------------------------------------------
	 * @file file.c/file.h
	 *	IFJ prekladac jazyka IFJ20
	 *	Tým 101:
	 *		@author Ty jako autor
	 * ------------------------------------------------
	 *		@author Jaksik, Ales <xjaksi01>
	 *		@author Vlasakova, Nela <xvlasa14>
	 *		@author Belohlavek, Jan <xbeloh8>
	 *		@author Mraz, Filip <xmrazf00>
	 * ------------------------------------------------
 	 * @brief kratky popis co dela tento soubor
 	 * -----------------------------------------------*/ 
### Další instrukce
1. komentáře píšeme v češtině BEZ diakritiky
	- v .h souboru bude podrobný popis funkce s využitím doxygen zápisu:
	
			/**
			 * Popis funkce.
			 * @param [název proměnné] [co proměnná dělá]
			 * @return [co vrací]
			*/
			void myFunc(int i);
	- v případě popisu proměnné na řádku budeme využívat následujícího doxygen zápisu:
	
			int i; ///< popis proměnné
2. kód píšeme v angličtině, celými slovy, výstižně
	- funkce: myFunction
	- proměnné: myVariable
4. ladící hlášky, chybové návratové hodnoty (včetně 0) jdou na stderr


