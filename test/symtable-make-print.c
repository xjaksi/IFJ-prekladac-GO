#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "symtable.h"

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
		printf("Uzel %s nexistuje\n", K);
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
			printf("Polozka - %s - je funkce \n", K);
			if (isDefined)
			{
				printf("  Byla definovana \n");
				printf("  Pocet parametru je %d \n", noParam);
				if (ddType == typeNo)
				{
					printf("  Datovy typ neni\n");
				}
				else if (ddType == typeInt)
				{
					printf("  Datovy typ je celociselny literal\n");
				}
				else if (ddType == typeDouble)
				{
					printf("  Datovy typ je desitinny literal\n");
				}
				else if (ddType == typeString)
				{
					printf("  Datovy typ je retezovy literal\n");
				}
				else
				{
					printf("  !!! Chybny datovy typ !!!\n");
				}
			}
			else
			{
				printf("  Zatim nebyla definovana\n");
			}
		}
		// informace o promenne
		else if (nnType == ntVar)
		{
			printf("Polozka - %s - je promenna \n", K);
			if (isDefined)
			{
				printf("  Byla definovana \n");
				if (ddType == typeNo)
				{
					printf("  Datovy typ neni\n");
				}
				else if (ddType == typeInt)
				{
					printf("  Datovy typ je celociselny literal\n");
				}
				else if (ddType == typeDouble)
				{
					printf("  Datovy typ je desitinny literal\n");
				}
				else if (ddType == typeString)
				{
					printf("  Datovy typ je retezovy literal\n");
				}
				else
				{
					printf("  !!! Chybny datovy typ !!!\n");
				}
			}
			else
			{
				printf("  Zatim nebyla definovana\n");
			}
		}
		else
		{
			printf("!!! Chybny typ uzlu !!!\n");
		}	
	}
}

int main() 
{
    int value = OK;

    tBSTNodePtr tab;
	tBSTNodePtr temp;

    BSTInit(&tab);
	BSTInit(&temp);

    value = BSTInsert(&tab, "klic1", createCont(ntVar, 0, NULL, true, typeInt));
	if (value != OK) 
    {
        BSTDispose(&tab);
		BSTDispose(&temp);
        return value;
    }

	value = BSTInsert(&tab, "klic2", createCont(ntVar, 0, NULL, true, typeDouble));
	if (value != OK) 
    {
        BSTDispose(&tab);
		BSTDispose(&temp);
        return value;
    }

	value = BSTInsert(&tab, "klic3", createCont(ntFunc, 0, NULL, false, typeNo));
	if (value != OK) 
    {
        BSTDispose(&tab);
		BSTDispose(&temp);
        return value;
    }

	value = BSTInsert(&temp, "klic4", createCont(ntVar, 0, NULL, true, typeInt));
	if (value != OK) 
    {
        BSTDispose(&tab);
		BSTDispose(&temp);
        return value;
    }

	value = BSTInsert(&temp, "klic5", createCont(ntVar, 0, NULL, true, typeInt));
	if (value != OK) 
    {
        BSTDispose(&tab);
		BSTDispose(&temp);
        return value;
    }

	value = BSTInsert(&tab, "klic3", createCont(ntFunc, 2, temp, true, typeInt));
	if (value != OK) 
    {
        BSTDispose(&tab);
		BSTDispose(&temp);
        return value;
    }

	testSymtabSingle(&tab, "klic1");
	testSymtabSingle(&tab, "klic2");
	testSymtabSingle(&tab, "klic3");

	testSymtabSingle(&temp, "klic4");
	testSymtabSingle(&temp, "klic5");


	BSTDispose(&temp);

	testSymtabSingle(&temp, "klic4");
	testSymtabSingle(&temp, "klic5");

	temp = BSTSearch(&tab, "klic3")->localFrame;
	
	testSymtabSingle(&temp, "klic4");
	testSymtabSingle(&temp, "klic5");


	BSTDispose(&temp);
    BSTDispose(&tab);

    return value;
}