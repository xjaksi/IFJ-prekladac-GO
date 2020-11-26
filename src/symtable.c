/** -----------------------------------------------
 * @file symtable.c
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Jaksik, Ales <xjaksi01>
 * ------------------------------------------------
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief Tabulka symbolu
 * -----------------------------------------------*/

#include "symtable.h"

/* ---------------------------------------------------- */
/*                    Binarni strom                     */


/**
 * Inicializace stromu
 */
void BSTInit (treeNode *RootPtr)
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
nodeInfCont BSTSearch (treeNode *RootPtr, char* K)
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
int BSTInsert (treeNode* RootPtr, char* K, bool def, nodeInfCont Content)
{
	// pokud je strom prazdny vytvorime novy
	if (*RootPtr == NULL)
	{
		// alokace mista pro koren
		*RootPtr = (treeNode) malloc(sizeof(struct BSTNode));
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

	// pokud se klice shoduji, redefinice odchazim
	else if (strcmp(K, ((*RootPtr)->Key)) == 0)
	{

		if (((*RootPtr)->TBSNodeCont->dType == Content->dType) && def == false)
		{
			(*RootPtr)->TBSNodeCont = Content;
			return OK;
		}
		else
		{
			return ERROR_REDEFINITION;
		}
	}

	// v jinem pripade se posunuji stromem

	// pokud je klic mensi nez aktualni root
	else if (strcmp(K, ((*RootPtr)->Key)) < 0)
	{
		// opetovne volam funkci pro levy podstrom
		BSTInsert(&((*RootPtr)->LPtr), K, def, Content);
		return OK;
	}

	// v jinem pripade jdu v pravo
	else
	{
		BSTInsert(&((*RootPtr)->RPtr), K, def, Content);
		return OK;
	}
}


/**
 * Zniceni stromu
 */
void BSTDispose (treeNode *RootPtr)
{
	// pokud neni strom prazdny nicim
	if (*RootPtr != NULL)
	{
		// projdeme strom z leva i prava
		BSTDispose(&((*RootPtr)->LPtr));
		BSTDispose(&((*RootPtr)->RPtr));

		free((*RootPtr)->TBSNodeCont);
		free(*RootPtr);

		// po inicializaci je NULL
		*RootPtr = NULL;
	}
}

/**
 * Vytvareni obsahu
 * nahraje do data obsah pro koren
 */
nodeInfCont createCont (nodeType nnType, int noParam, int noRet, int arg, int out, int ddType)
{
    nodeInfCont data = malloc(sizeof(struct nodeCont));
    data->nType = nnType;
    data->noParams = noParam;
	data->noReturn = noRet;
    data->dType = ddType;
	data->paramsIn = arg;
	data->paramsOut = out;

    return data;
}

/*                Konec binarniho stromu                */
/* ---------------------------------------------------- */

/* ---------------------------------------------------- */
/*     Seznam/zasobnik pro jednotlive ramce/stromy      */



void treeListInit(treeList *l)
{
	l->act = NULL;
	l->first = NULL;
}

int treeListInsert(treeList *l, treeNode *tree)
{
	listT newList = (struct ListCont*) malloc(sizeof(struct ListCont));
	if (newList == NULL) return ERROR_COMPILER;

	newList->symtab = &(*tree);

	if (l->first == NULL)
	{
		newList->next = NULL;
		l->first = newList;
	}
	else
	{
		newList->next = l->first;
		l->first = newList;
	}
	return OK;
}

void treeListRemove(treeList *l)
{
	if (l->first != NULL)
	{
		// je jediny
		if (l->first->next == NULL)
		{
			BSTDispose(l->first->symtab);
			free(l->first);
			l->first = NULL;
			l->act = NULL;
		}

		// existuji nasledujici
		else
		{
			l->act = l->first;
			l->first = l->first->next;
			BSTDispose(l->act->symtab);
			free(l->act);
			l->act = NULL;
		}
	}
}

void treeListDestroy(treeList *l)
{
	while (l->first != NULL)
	{
		treeListRemove(&l);
	}
}