
#include "scanner.h"
#include "dynamicString.h"
#include "tokenList.h"
#include "errors.h"
#include <string.h>

char *tokens[] = {"typ ID","typ integer","typ float","typ string","+","-","/","*",":=","=","<","<=",">",">=","==","!=","(",")","{","}",";","_",",","EOL","EOF",
					"if","else","for","return","float64","func","string","int","package",
					"inputs","inputi","inputf","print","int2float","float2int","len","substr","ord","chr","main"}; 

// prototypy func
void print_elements_of_list(tokenList TL);
int test_DLDisposeList(tokenList TL);
int test_DLCopy(tokenList TL);
int test_DLActive(tokenList TL);

int main(int argc, char *argv[])	{
	
	tokenList token;
    DLInitList(&token);

	int result;
	result = getTokensTo(&token);
	if(result != OK) 
		return result;



    printf("Dvousměrně vázaný lineární seznam\n");
    printf("=================================\n");
	
	print_elements_of_list(token);

	/*
    printf("\n[TEST11]\n");
    printf("Pokus o DLCopy při neaktivním seznamu => ošetřená chyba.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLCopy();

	

    printf("\n[TEST18]\n"); 
    printf("Seznam nakonec zrušíme.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
    test_DLDisposeList();
    test_DLActive();
	*/
	
	DLDisposeList(&token);

    return(0);
} 

/*******************************************************************************
 * Pomocné funkce usnadňující testování vlastní implementace.
 ******************************************************************************/

void print_elements_of_list(tokenList TL)	{
	
	int lines = 1;
	int CurrListLength = 0;
	printf("-----------------");
	printf("\nL:%3d  ----------", lines);
	while (TL.First!=NULL) {
		int sup = TL.First->t_type;
		printf("\n \t%10.10s",tokens[sup]);
		if(TL.First->atribute != NULL){
			printf(" \t_%s",TL.First->atribute->str);
		}
		if ((TL.First==TL.Act) && (TL.Act!=NULL))
			printf("\t <= toto je aktivní prvek ");
		TL.First=TL.First->rptr;
		if(sup == tEOL){
			printf("\nL:%3d  ----------", ++lines);
		}
		CurrListLength++;
	}
	printf("\n-----------------\n");     
}

int test_DLDisposeList(tokenList TL)	{
	DLDisposeList(&TL);
	print_elements_of_list(TL);
	return(TRUE);	
}

int test_DLCopy(tokenList TL)	{
	int temp;
	tStr sup;
	//str_Init(&sup);

  	DLCopy(&TL,&temp, NULL);				
	printf("Operace DLCopy vrací obsah %d.\n",temp);
	return(TRUE);
}

int test_DLActive(tokenList TL)	{
	int tmp=DLActive(&TL);		
	if (tmp)
		printf("Operace DLActive vrací hodnotu true.\n");
	else
		printf("Operace DLActive vrací hodnotu false.\n");	
	return(TRUE);				
}