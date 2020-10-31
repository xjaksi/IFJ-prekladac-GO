/* ----------------------------------------------
	Předmět: IFJ
	Tým: 101
	Autor:
		Jaksik, Ales (xjaksi01)
-------------------------------------------------
        Vlasakova, Nela (xvlasa14)
		Belohlavek, Honza
		Mraz, Filip (xmrazf00)
-------------------------------------------------
 Účel: Tabulka symbolů
 Popis: Tabulka symbolů
 ----------------------------------------------*/

#include "symtable.h"

/* ---------------------------------------------------- */
/*                    Binarni strom                     */


/**
 * Inicializace stromu
 */
void BSTInit (tBSTNodePtr *RootPtr)
{
	*RootPtr = NULL; //inicializace prazdneho uzlu
}

/**
 * Vyhledani uzlu
 *
 * Funkce prochazi strom
 * pokud jsem nasel klic vratim obsah uzlu
 * jinak vracim NULL
 */
nodeInfCont BSTSearch (tBSTNodePtr *RootPtr, char* K)
{
	// pokud je root prazdny
	if (*RootPtr == NULL)
	{
		return NULL;
	}

	// pokud kodnota klice odpovida hledanemu vratim ho
	else if (strcmp(K, (*RootPtr)->Key) == 0)
	{
		return (*RootPtr)->TBSNodeCont;
	}

	// v jinem pripade cestuji stromem a hledam dale

	// pokud je hodnota klice mensi nez aktualni root
	else if (strcmp(K, (*RootPtr)->Key) < 0)
	{
		// volam funkci rekurzivne a posunu se do leveho podstromu
		return BSTSearch(&(*RootPtr)->LPtr, K);
	}
	else
	{
		// jinak priste pokracuji v pravem podstromu
		return BSTSearch(&(*RootPtr)->RPtr, K);
	}
}

/**
 * Pridani uzlu
 *
 * Funkce prida uzel na spravne misto ve strome
 * Pokud je jiz misto plne aktualizujeme hodnotu
 *
 * Posouvam se pomoci cyklu, abych nezatezoval pamet a zasobnik
 */
int BSTInsert (tBSTNodePtr* RootPtr, char* K, nodeInfCont Content)
{
	// pokud je strom prazdny vytvorime novy
	if (*RootPtr == NULL)
	{
		// alokace mista pro koren
		*RootPtr = (tBSTNodePtr) malloc(sizeof(struct tBSTNode));
		if (*RootPtr == NULL)
		{
			return ERROR_COMPILER;
		}

		// vlozeni hodnot
		(*RootPtr)->Key = K;
		(*RootPtr)->TBSNodeCont = Content;
		(*RootPtr)->LPtr = NULL;
		(*RootPtr)->RPtr = NULL;

		return OK;
	}

	// pokud se klice shoduji, redefinace odchazim
	else if (strcmp(K, ((*RootPtr)->Key)) == 0)
	{
		return ERROR_REDEFINITION;
	}

	// v jinem pripade se posunuji stromem

	// pokud je klic mensi nez aktualni root
	else if (strcmp(K, ((*RootPtr)->Key)) < 0)
	{
		// opetovne volam funkci pro levy podstrom
		BSTInsert(&((*RootPtr)->LPtr), K, Content);
		return OK;
	}

	// v jinem pripade jdu v pravo
	else
	{
		BSTInsert(&((*RootPtr)->RPtr), K, Content);
		return OK;
	}
}


/**
 * Zniceni stromu
 */
void BSTDispose (tBSTNodePtr *RootPtr)
{
	// pokud neni strom prazdny nicim
	if (*RootPtr != NULL)
	{
		// projdeme strom z leva i prava
		BSTDispose(&((*RootPtr)->LPtr));
		BSTDispose(&((*RootPtr)->RPtr));

		// uvolnovani pameti
		free(*RootPtr);

		// po inicializaci je NULL
		*RootPtr = NULL;
	}
}

/**
 * Vytvareni obsahu
 * nahraje do data obsah pro koren
 */
nodeInfCont createCont (nodeType nnType, int noParam, struct tBSTNode *localTable, bool isDefined, dataType ddType)
{
    nodeInfCont data = malloc(sizeof(struct nodeCont));
    data->nType = nnType;
    data->noParams = noParam;
	data->localFrame = &(*localTable);
    data->defined = isDefined;
    data->dType = ddType;

    return data;
}

/**
 * Aktualizace datoveho typu, poctu parametru
 */
void BSTContActual (tBSTNodePtr *RootPtr, char* K, dataType type, int params)
{
	// podle klice najdeme poren v danem binarnim stromu
	nodeInfCont upDate = BSTSearch(RootPtr, K);

	upDate->dType = type;
	upDate->noParams = params;
}

/*                Konec binarniho stromu                */
/* ---------------------------------------------------- */


/* ---------------------------------------------------- */
/*                 Vypisovani pro testy                 */


/**
 * Vypsani konkretni polozky z tabulky
 * 
 * Vypis jde na stdout
 */
void testSymtabSingle (tBSTNodePtr *RootPtr, char* K)
{
	// najiti polozky
	nodeInfCont node = BSTSearch (RootPtr, K);

	// vypsani pokud koren neexistuje
	if (node == NULL)
	{
		printf("Uzel %s nexistuje", K);
	}
	else
	{
		nodeType nnType = node->nType;
		int noParam = node->noParams;
		bool isDefined = node->defined;
		dataType ddType = node->dType;

		// Vypsani informaci funkce
		if (nnType == ntFunc)
		{
			printf("Polozka %s je funkce \n", K);
			if (isDefined)
			{
				printf("  Byla definovana \n");
				printf("  Pocet parametru je %d \n", noParam);
				if (ddType == typeNo)
				{
					printf("  Datovy typ neni");
				}
				else if (ddType == typeInt)
				{
					printf("  Datovy typ je celociselny literal");
				}
				else if (ddType == typeDouble)
				{
					printf("  Datovy typ je desitinny literal");
				}
				else if (ddType == typeString)
				{
					printf("  Datovy typ je retezovy literal");
				}
				else
				{
					printf("  !!! Chybny datovy typ !!!");
				}
			}
			else
			{
				printf("  Zatim nebyla definovana");
			}
		}
		// informace o promenne
		else if (nnType == ntVar)
		{
			printf("Polozka %s je promenna \n", K);
			if (isDefined)
			{
				printf("  Byla definovana \n");
				if (ddType == typeNo)
				{
					printf("  Datovy typ neni");
				}
				else if (ddType == typeInt)
				{
					printf("  Datovy typ je celociselny literal");
				}
				else if (ddType == typeDouble)
				{
					printf("  Datovy typ je desitinny literal");
				}
				else if (ddType == typeString)
				{
					printf("  Datovy typ je retezovy literal");
				}
				else
				{
					printf("  !!! Chybny datovy typ !!!");
				}
			}
			else
			{
				printf("  Zatim nebyla definovana");
			}
		}
		else
		{
			printf("!!! Chybny typ uzlu !!!");
		}	
	}
}



/**
 * Vypsani vsech polozek a jejich hodnot
 */






/*                Konec kontrolnich vypisu              */
/* ---------------------------------------------------- */