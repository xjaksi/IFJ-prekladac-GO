/** -----------------------------------------------
 * @file parser.c
 *	IFJ prekladac jazyka IFJ20
 *	Tým 101:
 *		@author Jaksik, Ales <xjaksi01>
 * ------------------------------------------------
 *		@author Vlasakova, Nela <xvlasa14>
 *		@author Belohlavek, Jan <xbeloh8>
 *		@author Mraz, Filip <xmrazf00>
 * ------------------------------------------------
 * @brief Syntakticka a semanticka analyza
 * -----------------------------------------------*/

#include "parser.h"
#include "tokenList.h"
#include "errors.h"
#include "scanner.h"
#include "generator.h"


int returnWas;

int parse()
{

    treeNode funcTab;
    treeList tList;

    BSTInit(&funcTab);
    treeListInit(&tList);


    tokenList token;

    DLInitList(&token);


    int result;
    result = getTokensTo(&token);
    if (result != OK) return result;

    token.Act = token.First;
    while (token.Act->t_type == tEOL) 
    {
        token.Act = token.Act->rptr;
        if (token.Act->t_type == tEOF)
        {
            return ERROR_UNDEFINED;
        }
        
    }

    // vestavene funkce
    buidInFunc(&funcTab);

    // fprintf(stderr, "check me prvni token: %d\n", token.First->rptr->t_type);
    // kontrola hlavicky programu 'package main'
    if (token.Act->t_type != kwPACKAGE) return ERROR_SYNTAX;
    token.Act = token.Act->rptr;

    if (token.Act->t_type != fMAIN) return ERROR_SYNTAX;
    token.Act = token.Act->rptr;

    if (token.Act->t_type != tEOL) return ERROR_SYNTAX;
    token.Act = token.Act->rptr;

    // GENERATOR
    inception_gen();
    // GENEND

    // kontrola programu
    result = cScel(&token, &funcTab, &tList);

    // // fprintf(stderr, "final result: %d\n", result);
    DLDisposeList(&token, 1);
    BSTDispose(&funcTab);
    treeListDestroy(&tList);

    return result;

}

int cScel(tokenList *token, treeNode *funcTab, treeList *tList)
{
    int result;
    int *retVal;
    bool paramsIN = false;

    // deklarovani funkci pred kontrolou
    result = funcSave(token, funcTab);
    if (result != OK) return result;

    // fprintf(stderr, "CHECK 1\n");

    token->Act = token->First;
    while (token->Act->t_type != fMAIN) 
    {
        token->Act = token->Act->rptr;
        if (token->Act->t_type == tEOF) return ERROR_UNDEFINED;
    }
    
    token->Act = token->Act->rptr;
    
    
    while (token->Act->t_type != tEOF)
    {       
        returnWas = 0;
        while (token->Act->t_type == tEOL)
        {
            if (token->Act->t_type == tEOF) return result;
            token->Act = token->Act->rptr;
        }
        
        if (token->Act->t_type == tEOF) return result;

        if (token->Act->t_type != kwFUNC) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;

        // pokud jsem v main neresim
        if (token->Act->t_type == fMAIN)
        {
            
            retVal = NULL;
            paramsIN = false;

            // GENERATOR
            gen_main();
            // GENEND
        }
        // jinak kontroluji telo funkce
        else
        {
            // GENERATOR
            gen_func_start(token->Act->atribute->str);
            // pocitadlo parametru
            int par_num = 1;
            // GENEND

            // tabulka pro funkci
            nodeInfCont nodeCont = BSTSearch(funcTab, token->Act->atribute->str);
            if (nodeCont == NULL) return ERROR_COMPILER;
            if (nodeCont->noReturn != 0) 
            {
                retVal = nodeCont->paramsOut;
            }
            else
            {
                retVal = NULL;
            }

            if (nodeCont->noParams != 0)
            {
                paramsIN = true;
                // pokud ma funkce argumenty dam do tabulky
                treeNode localTab;
                BSTInit(&localTab);
                treeListInsert(tList, &localTab);

                // po id je ( potom zacinaji parametry
                token->Act = token->Act->rptr->rptr;

                // mam id
                while (token->Act->t_type != tRBRACKET)
                {
                    if (token->Act->t_type == tEOF) return ERROR_COMPILER;
                    if (token->Act->rptr->t_type == kwINT)
                    {
                        result = BSTInsert(&(tList->first->symtab), token->Act->atribute->str, 1, createCont(ntVar, 101,101,NULL,NULL, tINT));
                        if (result != OK) return result;
                    }
                    else if (token->Act->rptr->t_type == kwFLOAT64)
                    {
                        result = BSTInsert(&(tList->first->symtab), token->Act->atribute->str, 1, createCont(ntVar, 101,101,NULL,NULL, tFLOAT));
                        if (result != OK) return result;
                    }
                    else if (token->Act->rptr->t_type == kwSTRING)
                    {
                        result = BSTInsert(&(tList->first->symtab), token->Act->atribute->str, 1, createCont(ntVar, 101,101,NULL,NULL, tSTRING));
                        if (result != OK) return result;
                    }
                    else
                    {
                        return ERROR_SYNTAX;
                    }
                    // GENERATOR: generuje parametry uvnitr funkce
                    gen_func_param(token->Act->atribute->str, par_num);
                    par_num++;
                    // GENEND

                    token->Act = token->Act->rptr->rptr;
                    if (token->Act->t_type == tCOMMA) token->Act = token->Act->rptr; 
                }
            }
            else
            {
                paramsIN = false;
            }
        }
        
 // fprintf(stderr, "CHECK 2\n");

        // GENERATOR: pocitadlo navratovych hodnot
        int ret_num = 1;
        // GENEND

        while (token->Act->t_type != tLBRACE)
        {
            // GENERATOR: generuje jednotlive promene ret
            if(token->Act->t_type == kwINT || token->Act->t_type == kwFLOAT64 || token->Act->t_type == kwSTRING){
                gen_func_ret_start(ret_num);
                ret_num++;
            }
            // GENEND

            if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
            token->Act = token->Act->rptr;
        }
        token->Act = token->Act->rptr;


        if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
        result = cBody(token, funcTab, tList, retVal);
        // // fprintf(stderr, "cbody result: %d\n", result);
        if (result != OK) return result;

        if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;

        // odchazim z tela funkce
        fprintf(stderr, "OUT rreturn %d\n", returnWas);
        if (returnWas == 0 && retVal != NULL) return ERROR_RETURN_VALUE;
        if (paramsIN) treeListRemove(tList);
    }

    return result;
}

int funcSave(tokenList *token, treeNode *funcTab)
{

    int isMain = 0;
    int result;

    while (token->Act->t_type != tEOF)
    {
        if (token->Act->t_type == kwFUNC)
        {
            token->Act = token->Act->rptr;

            // pokud neni main pridam do tabulky
            if (token->Act->t_type == fMAIN)
            {
                // fprintf(stderr, "HELLO, MAIN \n");
                isMain++;
                int err = 0;
                token->Act = token->Act->rptr;
                if (token->Act->t_type != tLBRACKET) return ERROR_SYNTAX;
                token->Act = token->Act->rptr;
                while (token->Act->t_type != tRBRACKET)
                {
                    if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
                    err++;
                    if (token->Act->t_type == tID)
                    {
                        token->Act = token->Act->rptr;
                        if (token->Act->t_type != kwINT &&
                            token->Act->t_type != kwSTRING &&
                            token->Act->t_type != kwFLOAT64 )
                                return ERROR_SYNTAX;
                        token->Act = token->Act->rptr;
                        if (token->Act->t_type == tRBRACKET)
                        {
                            break;
                        }
                        else
                        {
                            if (token->Act->t_type != tCOMMA) return ERROR_SYNTAX;
                            token->Act = token->Act->rptr;
                        }
                    }
                    else
                    {
                        return ERROR_SYNTAX;
                    }
                    
                }
                if (err > 0) return ERROR_RETURN_VALUE;
                if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
                token->Act = token->Act->rptr;

                if (token->Act->t_type == tLBRACKET)
                {
                    token->Act = token->Act->rptr;
                    while (token->Act->t_type != tRBRACKET)
                    {
                        if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
                        err++;
                        if (token->Act->t_type != kwINT &&
                            token->Act->t_type != kwSTRING &&
                            token->Act->t_type != kwFLOAT64 )
                                return ERROR_SYNTAX;
                        token->Act = token->Act->rptr;
                        if (token->Act->t_type == tRBRACKET)
                        {
                            break;
                        }
                        else
                        {
                            if (token->Act->t_type != tCOMMA) return ERROR_SYNTAX;
                            token->Act = token->Act->rptr;
                        }
                    }
                    if (err > 0) return ERROR_RETURN_VALUE;

                    if (token->Act->t_type != tRBRACKET) return ERROR_RETURN_VALUE;
                    token->Act = token->Act->rptr;
                }

                if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
            }
            else
            {   
                if (isMain > 0) return ERROR_SYNTAX;

                // ulozeni nazvu funkce a kontrola parametru
                if (token->Act->t_type != tID) return ERROR_SYNTAX;

                char *idName = token->Act->atribute->str;
                // fprintf(stderr, "FUNC NAME: %s\n", idName);
                int noArg = 0;
                int noRet = 0;
                int noComma = 0;

                // argumenty funkce
                token->Act = token->Act->rptr;
                if (token->Act->t_type != tLBRACKET) return ERROR_SYNTAX;
                token->Act = token->Act->rptr;

                // dva pruchody argumentama
                // nejdriv zjistim pocet argumentu
                while (token->Act->t_type != tRBRACKET && token->Act->t_type != tEOF)
                {
                    noArg++;
                    token->Act = token->Act->rptr->rptr;
                    if (token->Act->t_type == tCOMMA)
                    {
                        noComma++;
                        token->Act = token->Act->rptr;
                    }
                }
                // fprintf(stderr, "noArgs: %d   noComma: %d\n", noArg, noComma);
                if (noArg > 1 && noArg != noComma+1) return ERROR_SYNTAX;
                if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
                // fprintf(stderr, "Prošel jsem \n");
                int args[ noArg ];
                // pokud existuji argumenty kontroluji je od zacatku
                if (noArg > 0)
                {
                    while (token->Act->lptr->t_type != tLBRACKET) token->Act = token->Act->lptr;
                    // fprintf(stderr, "Došel jsem na x? %d\n", token->Act->t_type);
                    for (int i = 0; i < noArg; i++)
                    {
                        if (token->Act->t_type != tID) return ERROR_SYNTAX;

                        token->Act = token->Act->rptr;

                        if (token->Act->t_type == kwINT)
                        {
                            args[i] = tINT;
                        }
                        else if (token->Act->t_type == kwFLOAT64)
                        {
                            args[i] = tFLOAT;
                        }
                        else if (token->Act->t_type == kwSTRING)
                        {
                            args[i] = tSTRING;
                        }
                        else
                        {
                            return ERROR_SYNTAX;
                        }

                        token->Act = token->Act->rptr;

                        // za datovym typem nasleduje konec argumentu nebo dalsi
                        if (token->Act->t_type == tRBRACKET) 
                        {
                            if (i+1 != noArg) return ERROR_SYNTAX;
                            break;
                        }
                        if (token->Act->t_type != tCOMMA) return ERROR_SYNTAX;
                        token->Act = token->Act->rptr;
                    }
                }
                if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
                token->Act = token->Act->rptr;

                // pokud je tabulka s navratovymi hodnotami, kontroluji
                if (token->Act->t_type == tLBRACKET)
                {
                    noComma = 0;
                    token->Act = token->Act->rptr;
                    // pocitani hodnot
                    while (token->Act->t_type != tRBRACKET && token->Act->t_type != tEOF)
                    {
                        noRet++;
                        token->Act = token->Act->rptr;
                        if (token->Act->t_type == tCOMMA) 
                        {
                            noComma++;
                            token->Act = token->Act->rptr;
                        }
                    }
                    if (noRet > 1 && noRet != noComma+1) return ERROR_SYNTAX;
                    if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
                    
                    if (noRet > 0) noRet++;
                    int ret[ noRet ];
                    if (noRet > 0)
                    {
                        // posun na zacatek navratovych hodnot
                        while (token->Act->lptr->t_type != tLBRACKET) token->Act = token->Act->lptr;

                        for (int i = 0; i < noRet; i++)
                        {
                            if (token->Act->t_type == kwINT)
                            {
                                ret[i] = tINT;
                            }
                            else if (token->Act->t_type == kwFLOAT64)
                            {
                                ret[i] = tFLOAT;
                            }
                            else if (token->Act->t_type == kwSTRING)
                            {
                                ret[i] = tSTRING;
                            }
                            else
                            {
                                return ERROR_SYNTAX;
                            }
                            token->Act = token->Act->rptr;

                            // za datovym typem nasleduje konec nebo dalsi
                            if (token->Act->t_type == tRBRACKET)
                            {
                                ret[i+1] = 101;
                                fprintf(stderr, "%d %d\n", i+1, noRet);
                                if (i+2 != noRet) return ERROR_SYNTAX; 
                                break;
                            }
                            if (token->Act->t_type != tCOMMA) return ERROR_SYNTAX;
                            token->Act = token->Act->rptr;
                        }

                    }

                    if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
                    token->Act = token->Act->rptr;

                    result = BSTInsert(funcTab, idName, 1, createCont(ntFunc, noArg, noRet, args, ret, 101));
                    if (result != OK) return result;
                }
                else
                {
                    result = BSTInsert(funcTab, idName, 1, createCont(ntFunc, noArg, 0, args, NULL, 101));
                    if (result != OK) return result;
                }
                if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
            }
            
        }
        
        if (token->Act == NULL) return ERROR_COMPILER;
        token->Act = token->Act->rptr;
    }
    
    if (isMain != 1)
    {
        int super = 0;
        token->Act = token->First;
        while (token->Act->t_type != tEOF)
        {
            if (token->Act->t_type == fMAIN) super++;
            token->Act = token->Act->rptr;
        }
        if (super == 2) return ERROR_SYNTAX;
        return ERROR_UNDEFINED;
    }
    

    return OK;
}

int cBody(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal)
{
    // // fprintf(stderr, "CHECK 3\n");
    int result = OK;
    int type;

    // vytvoreni lokalni tabulky pro soucasne telo
    treeNode localTab;
    BSTInit(&localTab);
    treeListInsert(tList, &localTab);

    // // fprintf(stderr, "CHECK 4\n");
    while (token->Act->t_type != tRBRACE)
    {   
        if (token->Act->t_type == tEOF) return ERROR_SYNTAX;

        while (token->Act->t_type == tEOL)
        {
            if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
            token->Act = token->Act->rptr;
        }
        // fprintf(stderr, "[DEBUG] prava zavorka, result: %d \n", result);
        if (token->Act->t_type == tRBRACE) 
        {
            fprintf(stderr, "END OF BODY %d\n", result);
            treeListRemove(tList);
            return result;
        }

        switch (token->Act->t_type)
        {
        case tDEVNULL:
        case tID :
            result = cId(token, funcTab, tList);
            fprintf(stderr, "[DEBUG] konec id, result: %d  token: %d\n", result, token->Act->t_type);
            if (result != OK) return result;

            break;

        case kwIF:
            result = cIf(token, funcTab, tList, retVal);
            if (result != OK) return result;
            break;

        case kwFOR:
            result = cFor(token, funcTab, tList, retVal);
            if (result != OK) return result;
            break;

        case kwRETURN:
            returnWas = 1;
            token->Act = token->Act->rptr;
            int cnt = 0;
            
            if (retVal != NULL)
            {
                if (token->Act->t_type == tEOL) return ERROR_RETURN_VALUE;
                while (token->Act->t_type != tEOL)
                {
                    if (token->Act->t_type == tEOF) return ERROR_SYNTAX;

                    result = cExpr(token, tList, &type);
                    if (result != OK) return result;
                    fprintf(stderr, "RETURN      prislo: %d\n", type);
                    fprintf(stderr, "RETURN    ocekavam: %d\n", retVal[cnt]);
                    if (type != retVal[cnt]) ERROR_RETURN_VALUE;

                    cnt++;

                    // GENERATOR
                    gen_func_ret_val(cnt);
                    // GENEND

                    if (token->Act->t_type == tCOMMA) token->Act = token->Act->rptr;
                }
            }
            else
            {
                return ERROR_RETURN_VALUE;
            }
            fprintf(stderr, "RETURN OK %d \n", retVal[3]);
            if (retVal[cnt] != 101) return ERROR_RETURN_VALUE;
            fprintf(stderr, "RETURN OK %d \n", retVal[cnt]);
            
            // GENERATOR
            gen_func_end();
             // GENEND

            break;
        
        default:

            return ERROR_SYNTAX;
        }

    }
    fprintf(stderr, "END OF BODY \n");
    // odebrani ramce na konci tela
    treeListRemove(tList);
    
    return result;
}

int cId(tokenList *token, treeNode *funcTab, treeList *tList)
{
    // // fprintf(stderr, "CHECK 5\n");
    int cnt = 1;
    int result;
    int type;

    // GENERATOR: ukladani nazvu id pro mozne pouziti pri predavani ret_value
    char *arr_of_DS[]={ [0 ... 15] = NULL};
    if(token->Act->t_type == tID) arr_of_DS[0] = token->Act->atribute->str;
    int flag_func = 0;
    // GENEND

    token->Act = token->Act->rptr;

    while (token->Act->t_type == tCOMMA)
    {
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tID &&
            token->Act->t_type != tDEVNULL) return ERROR_SYNTAX;

        // GENERATOR: nacita dalsi nazvy id do pole char *
        if(token->Act->t_type == tID) arr_of_DS[cnt] = token->Act->atribute->str;
        // GENEND

        cnt++;
        token->Act = token->Act->rptr;
    }
    
    switch (token->Act->t_type)
    {
    case tLBRACKET:
        // pokud je to funkce musi byt sama
        if (cnt != 1) return ERROR_SYNTAX;
        if (token->Act->lptr->t_type != tID) return ERROR_SYNTAX;
        result = cFunc(token, funcTab, tList, 0, false);
        if (result != OK) return result;
        // token by mel by nastaven na konec radku
        break;

    case tASSIGN:
        fprintf(stderr, "CHECK 7\n"); 
        token->Act = token->Act->rptr;
        // predam ukazatel na prvni token za =
        result = cAssign(token, funcTab, tList, cnt, &flag_func);

        // GENERATOR: pokud byla volana funkce priradi se navratove hodnoty
        if(flag_func == 1){
            for(int i = 0; i < cnt; i++){
                if(arr_of_DS[i] != NULL) gen_func_tf_ret(arr_of_DS[i], i);
            }
        }else{
            for(int i = cnt; i >= 0; i--){
                if(arr_of_DS[i] != NULL) gen_var_assign(arr_of_DS[i]);
            }
        }
        // GENEND

        fprintf(stderr, "result assign: %d token: %d\n", result, token->Act->t_type);
        if (result != OK) return result;
        break;

    case tDEF:
        if (cnt != 1) return ERROR_SYNTAX;
        if (token->Act->lptr->t_type != tID) return ERROR_SYNTAX;
        
        nodeInfCont nodeCont = BSTSearch(funcTab, token->Act->lptr->atribute->str); 
        char* idName = token->Act->lptr->atribute->str;
        if (nodeCont != NULL) return ERROR_REDEFINITION;
        token->Act = token->Act->rptr;
        result = cExpr(token, tList, &type);

        // GENERATOR
        printf("\n DEFVAR LF@$%s", idName);
        printf("\n POPS LF@$%s", idName);
        // GENEND
        
        if (result != OK) return result;
        // pokud je type podporovany typ ulozim
        if (type == DT_INT)
        { 
            result = BSTInsert(&(tList->first->symtab), idName, 1, createCont(ntVar, 101, 101, NULL, NULL, tINT));
            if (result != OK) return result;
        }
        else if (type == DT_STRING)
        {
            result = BSTInsert(&(tList->first->symtab), idName, 1, createCont(ntVar, 101, 101, NULL, NULL, tSTRING));
            if (result != OK) return result;
        }
        else if (type == DT_FLOAT)
        {
            result = BSTInsert(&(tList->first->symtab), idName, 1, createCont(ntVar, 101, 101, NULL, NULL, tFLOAT));
            if (result != OK) return result;
        }
        else
        {
            return ERROR_TYPE_INFERENCE;
        }
        break;
           
    default:
        return ERROR_SYNTAX;
    }
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    return OK;
}

// token za =
int cAssign(tokenList *token, treeNode *funcTab, treeList *tList, int item, int *flag_func)
{
    int result;
    // fprintf(stderr, "CHECK 8\n"); 
    // prirazuji z funkce
    if (token->Act->t_type == tID && token->Act->rptr->t_type == tLBRACKET)
    {
        // GENERATOR: nastavuje flag pro rozpoznani funkce po returnu
        *flag_func = 1;
        // GENEND   

        token->Act = token->Act->rptr;
        result = cFunc(token, funcTab, tList, item, true);
        return result;
    }

    int list[ item ];
    int type;

     // // fprintf(stderr, "CHECK 9 %d\n", item); 

    // nahrani datovych typu
    for (int i = 0; i < item; i++)
    {
        if (i == 0)
        {
            if (token->Act->t_type == tEOL) return ERROR_SYNTAX;
        }
        if (token->Act->t_type == tEOL) return ERROR_SEMANTICS;
        result = cExpr(token, tList, &type);
        // // fprintf(stderr, "CHECK 10 %d result: %d\n", type, result); 
        if (result != OK) return result;

        if (type == DT_INT)
        {
            list[i] = tINT;
        }
        else if (type == DT_STRING)
        {
            list[i] = tSTRING;
        }
        else if (type == DT_FLOAT)
        {
            list[i] = tFLOAT;
        }
        else
        {
            return ERROR_TYPE_INFERENCE;
        }
        
        if (token->Act->t_type == tCOMMA) token->Act = token->Act->rptr;
    }
    // // fprintf(stderr, "CHECK 11 %d\n", token->Act->t_type); 

    if (token->Act->t_type != tEOL) 
    {
        if (token->Act->lptr->t_type == tCOMMA)
        {
            if (token->Act->t_type == tID ||
                token->Act->t_type == tSTRING ||
                token->Act->t_type == tINT ||
                token->Act->t_type == tFLOAT)
                    return ERROR_SEMANTICS;
        }
        return ERROR_SYNTAX;
    }
    if (token->Act->lptr->t_type == tCOMMA) return ERROR_SYNTAX;
    
    // jinak jdu na zacatek radku a overuji
    token->Act = token->Act->lptr;
    // // fprintf(stderr, "CHECK 13 %d\n", token->Act->t_type); 
    while (token->Act->lptr->t_type != tEOL)
    {
        if (token->Act->lptr == NULL) return ERROR_COMPILER;
        // // fprintf(stderr, "CHECK 14: posunujeme se zpět v čase, opoustime %d\n", token->Act->t_type); 
        token->Act = token->Act->lptr;
    }
    // // fprintf(stderr, "CHECK 15 %d\n", token->Act->t_type); 
    for (int i = 0; i < item; i++)
    {
        if (token->Act->t_type == tDEVNULL)
        {
            token->Act = token->Act->rptr;
        }
        else
        {
            if (token->Act->t_type != tID) return ERROR_SYNTAX;

            type = dataSearch(tList, token->Act->atribute->str);
            if (type == 101) return ERROR_UNDEFINED;
            if (type != list[i]) return ERROR_SEMANTICS;
            token->Act = token->Act->rptr;
        }
        if (token->Act->t_type == tCOMMA) token->Act = token->Act->rptr;
    }
    while (token->Act->t_type != tEOL) 
    {
        token->Act = token->Act->rptr;
        if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
        
    }
    return OK;

}

int cIf(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal)
{
    int result;
    int type;
    token->Act = token->Act->rptr;
    result = cExpr(token, tList, &type);
    if (result != OK) return result;
    // fprintf(stderr, "JE TO BOOL? %d\n", type);
    if (type != DT_BOOL) return ERROR_SEMANTICS;

    // GENERATOR
    int if_id = ++token->if_cnt;
    gen_if(if_id);
    // GENEND

    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
    // telo if
    result = cBody(token, funcTab, tList, retVal);
    if (result != OK) return result;
    if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    while (token->Act->t_type == tEOL)
    {
        token->Act = token->Act->rptr;
    }

    // GENERATOR
    gen_else(if_id);
    // GENEND

    // prvni ok token je else
    if (token->Act->t_type != kwELSE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
    // telo else
    result = cBody(token, funcTab, tList, retVal);
    // fprintf(stderr, "[DEBUG] konec body, result: %d \n", result);
    if (result != OK) return result;
    if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;

    // GENERATOR
    gen_else_end(if_id);
    // GENEND

    if (token->Act->rptr->t_type == tEOF) return ERROR_SYNTAX;

    token->Act = token->Act->rptr->rptr;
    // fprintf(stderr, "[DEBUG] konec if \n");

    return OK;
}

int cFor(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal)
{
    int result;
    int type, dat;
    bool head = false;

    token->Act = token->Act->rptr;

    // GENERATOR
    int for_id = ++token->for_cnt;
    // GENEND

    //definice
    if (token->Act->t_type == tID)
    {
        // GENERATOR
        char *id_name = token->Act->atribute->str;           
        // GENEND

        // pokud existuje jako funkce, koncim
        nodeInfCont nodeCont = BSTSearch(funcTab, token->Act->atribute->str);
        if (nodeCont != NULL) return ERROR_REDEFINITION;
        // jinal vytvorim novy ramec
        treeNode localTab;
        BSTInit(&localTab);
        treeListInsert(tList, &localTab);
        head = true;
        char *name = token->Act->atribute->str;
        // :=
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tDEF) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
        // hodnota vyrazu
        result = cExpr(token, tList, &type);
        if (result != OK) return result;
        
        // GENERATOR
        gen_defvar(id_name);        
        gen_for_start(for_id);
        // GENEND

        // vlozeni do tabulky
        if (type == DT_INT)
        {
            result = BSTInsert(&(tList->first->symtab), name, 1, createCont(ntVar, 101, 101, NULL, NULL, tINT));
        }
        else if (type == DT_STRING)
        {
            result = BSTInsert(&(tList->first->symtab), name, 1, createCont(ntVar, 101, 101, NULL, NULL, tSTRING));
        }
        else if (type == DT_FLOAT)
        {
            result = BSTInsert(&(tList->first->symtab), name, 1, createCont(ntVar, 101, 101, NULL, NULL, tFLOAT));
        }
        else
        {
            return ERROR_TYPE_INFERENCE;
        }
        if (result != OK) return result;
        
    }
    if (token->Act->t_type != tSEMICOLON) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;

    // vyraz
    if (token->Act->t_type != tSEMICOLON)
    {
        result = cExpr(token, tList, &type);
        if (result != OK) return result;
        if (type != DT_BOOL) return ERROR_SEMANTICS;

        // GENERATOR
        gen_for_2(for_id);
        // GENEND
    }
    else
    {
        return ERROR_SYNTAX;
    }
    
    if (token->Act->t_type != tSEMICOLON) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;

    // prirazeni
    if (token->Act->t_type != tLBRACE)
    {
        if (token->Act->t_type != tID) return ERROR_SYNTAX;

        // GENERATOR
        char *id_name_assign = token->Act->atribute->str;
        // GENEND

        // podivam se jestli je id definovano
        dat = dataSearch(tList, token->Act->atribute->str);
        if (dat == 101) return ERROR_UNDEFINED;
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tASSIGN) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
        result = cExpr(token, tList, &type);
        if (result != OK) return result;
        if (((dat == tINT) && (type != DT_INT)) ||
            ((dat == tFLOAT) && (type != DT_FLOAT)) ||
            ((dat == tSTRING) && (type != DT_STRING)))
                return ERROR_SEMANTICS;

        // GENERATOR
        gen_var_assign(id_name_assign);
        // GENEND
    }

    // GENERATOR
    gen_for_3(for_id);
    // GENEND

    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    // telo for
    result = cBody(token, funcTab, tList, retVal);
    if (result != OK) return result;

    // GENERATOR
    gen_for_end(for_id);
    // GENEND

    // pokud jsem vytvarel hlavicku popnu
    if (head) treeListRemove(tList);

    token->Act = token->Act->rptr;
    return OK;
}

// kontrola funkce, ocekavam token leve zavorky
int cFunc(tokenList *token, treeNode *funcTab, treeList *tList, int noItems, bool ass)
{
    int result;
    nodeInfCont nodeCont = BSTSearch(funcTab, token->Act->lptr->atribute->str);
    if (nodeCont == NULL) return ERROR_UNDEFINED;

    // GENERATOR: ulozi nazev funkce do pomocne promenne pro pozdejsi volani
    char *func_name = token->Act->lptr->atribute->str;
    // GENEND

    fprintf(stderr, "Kontrola %s \n", token->Act->lptr->atribute->str);
    // specialni pripad print
    if (str_Compare_char(token->Act->lptr->atribute, "print") == 0)
    {
        if (ass) return ERROR_RETURN_VALUE;
        int noComma = 0;
        int noPrint = 0;

        token->Act = token->Act->rptr;
        while (token->Act->t_type != tRBRACKET)
        {           

            if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
            if (token->Act->t_type == tID)
            {
                result = dataSearch(tList, token->Act->atribute->str);
                if (result == 101) return ERROR_UNDEFINED;
                noPrint++;

                // GENERATOR: generuje volani funkce print pro kazdy parametr
                printf("\n CREATEFRAME");
                gen_func_tf_var(token->Act, 0);
                printf("\n CALL $print");
                // GENEND
            }
            else
            {
                if (token->Act->t_type != tINT &&
                    token->Act->t_type != tFLOAT &&
                    token->Act->t_type != tSTRING)
                {

                    return ERROR_SYNTAX;
                }
                // GENERATOR: generuje volani funkce print pro kazdy parametr
                printf("\n CREATEFRAME");
                gen_func_tf_var(token->Act, 0);
                printf("\n CALL $print");
                // GENEND

                noPrint++;
            }
            
            token->Act = token->Act->rptr;
            if (token->Act->t_type == tCOMMA) 
            {
                token->Act = token->Act->rptr;
                noComma++;
            }
            else
            {
                if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
            }
            
        }
        if (noPrint != 0)
        {
            if (noPrint != noComma+1) return ERROR_SYNTAX;
        }
        fprintf(stderr, "print OK \n");
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
        return OK;

        
        

    }
    
    // GENERATOR
    printf("\n CREATEFRAME");
    // GENEND

    // funkce existuje kontroluji argumenty
    int noParam = nodeCont->noParams;
// fprintf(stderr, "noParam: %d\n", noParam);
    token->Act = token->Act->rptr; // posuny se na token za (
    // funkce nema mit argument
    if (noParam == 0)
    {
        if (token->Act->t_type != tRBRACKET) return ERROR_PARAMETERS;
    }
    // jeden argument
    else if (noParam == 1)
    {
        int dataT;
        // fprintf(stderr, "Node cond stuff: %d\n",nodeCont->paramsIn[0]);
        // fprintf(stderr, "Token type: %d\n", token->Act->t_type);
        if (token->Act->t_type == tID)
        {
            dataT = dataSearch(tList, token->Act->atribute->str);
            // fprintf(stderr, "Data type: %d\n", dataT);
            if (dataT != nodeCont->paramsIn[0]) return ERROR_PARAMETERS;
        }
        else
        {
            if (token->Act->t_type != nodeCont->paramsIn[0]) return ERROR_PARAMETERS;
        }

        // GENERATOR
        gen_func_tf_var(token->Act, 0);
        // GENEND

        token->Act = token->Act->rptr;
        if (token->Act->t_type == tCOMMA) return ERROR_PARAMETERS;
        if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
    }
    // vice
    else
    {
        int dataT;
        int noC = 0;
        for (int i = 0; i < noParam; i++)
        {
            if (token->Act->t_type == tID)
            {
                dataT = dataSearch(tList, token->Act->atribute->str);
                if (dataT != nodeCont->paramsIn[i]) return ERROR_PARAMETERS;
            }
            else
            {

                if (token->Act->t_type != nodeCont->paramsIn[i]) return ERROR_PARAMETERS;
            }

            // GENERATOR
            gen_func_tf_var(token->Act, i);
            // GENEND

            token->Act = token->Act->rptr;
            if (token->Act->t_type == tCOMMA) 
            {
                noC++;
                token->Act = token->Act->rptr;
            }
            else
            {
                if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
            }
            
        }
        if (token->Act->t_type == tID ||
            token->Act->t_type == tINT ||
            token->Act->t_type == tSTRING ||
            token->Act->t_type == tFLOAT ||
            noC+1 != noParam)
                return ERROR_PARAMETERS;
        // fprintf(stderr, "bracket r? %d\n", token->Act->t_type);
        if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;

    }

    // GENERATOR
    gen_func_call(func_name);
    // GENEND

    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    // kontrola navratovych hodnot
    if (ass)
    {
        if (nodeCont->noReturn != noItems+1) return ERROR_RETURN_VALUE;

        // jdu na zacatek radku
        token->Act = token->Act->lptr;
        while (token->Act->lptr->t_type != tEOL)
        {
            token->Act = token->Act->lptr;
            if (token->Act->lptr == NULL) return ERROR_COMPILER;
        }
        // overovani datovych typu
        int type;
        for (int i = 0; i < noItems+1; i++)
        {
            if (token->Act->t_type == tDEVNULL)
            {
                token->Act = token->Act->rptr;
            }
            else if (token->Act->t_type == tASSIGN)
            {
                if (nodeCont->paramsOut[i] != 101)
                {
                    return ERROR_RETURN_VALUE;
                }
                break;
            }
            else
            {
                
                if (token->Act->t_type != tID) return ERROR_SYNTAX;
                type = dataSearch(tList, token->Act->atribute->str);
                if (type == 101) return ERROR_UNDEFINED;
                if (type != nodeCont->paramsOut[i]) return ERROR_RETURN_VALUE;
                token->Act = token->Act->rptr;
            }
            if (token->Act->t_type == tCOMMA)
            {
                token->Act = token->Act->rptr;
            }
        }

        // snad jsem se zastavil na =
        // radek je OK jdu na konec
        while (token->Act->t_type != tEOL)
        {
            if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
            token->Act = token->Act->rptr;
        }
    }
    else
    {
        if (nodeCont->noReturn != 0) return ERROR_SEMANTICS;
    }
    
    return OK;
}


int cExpr(tokenList *token, treeList *tList, int *type)
{
    int result = OK;
    tokenList newToken;

    DLInitList(&newToken);

    while (token->Act->t_type != tEOL &&
            token->Act->t_type != tSEMICOLON &&
            token->Act->t_type != tLBRACE &&
            token->Act->t_type != tCOMMA &&
            token->Act->t_type != tEOF &&
            token->Act->t_type != tRBRACE )
    {
        DLInsertLast(&newToken, token->Act->t_type, token->Act->atribute);
        token->Act = token->Act->rptr;
    }
    if (newToken.First == NULL) {
        return ERROR_SYNTAX;
    }

    result = parseExp(&newToken, tList, type);
    

    DLDisposeList(&newToken, 0);       // TO DO UPRAVIT
    return result;
}


void buidInFunc(treeNode *funcTab)
{
    // inputs
    int out[3] = {tSTRING, tINT, 101};
    BSTInsert(funcTab, "inputs", 0, createCont(ntFunc, 0, 3, NULL, out, 101));
    //inputi
    int out2[3] = {tINT, tINT, 101};
    BSTInsert(funcTab, "inputi", 0, createCont(ntFunc, 0, 3, NULL, out2, 101));
    // inputf
    int out3[3] = {tFLOAT, tINT, 101};
    BSTInsert(funcTab, "inputf", 0, createCont(ntFunc, 0, 3, NULL, out3, 101));
    // print
    BSTInsert(funcTab, "print", 0, createCont(ntFunc, 101, 0, NULL, NULL, 101));
    // int2float
    int out4[2] = {tFLOAT, 101};
    int in4[1] = {tINT};
    BSTInsert(funcTab, "int2float", 0, createCont(ntFunc, 1, 2, in4, out4, 101));
    // float2int
    int out5[2] = {tINT, 101};
    int in5[1] = {tFLOAT};
    BSTInsert(funcTab, "float2int", 0, createCont(ntFunc, 1, 2, in5, out5, 101));
    // len
    int out6[2] = {tINT, 101};
    int in6[1] = {tSTRING};
    BSTInsert(funcTab, "len", 0, createCont(ntFunc, 1, 2, in6, out6, 101));
   // nodeInfCont temporary = BSTSearch(funcTab, "len");
   // // fprintf(stderr, "[BUILD IN FUNC] data type: %d\n", temporary->paramsIn[0]);
    // substr
    int out7[3] = {tSTRING, tINT, 101};
    int in7[3] = {tSTRING, tINT, tINT};
    BSTInsert(funcTab, "substr", 0, createCont(ntFunc, 3, 3, in7, out7, 101));
    // ord
    int out8[3] = {tINT, tINT, 101};
    int in8[2] = {tSTRING, tINT};
    BSTInsert(funcTab, "ord", 0, createCont(ntFunc, 2, 3, in8, out8, 101));
    // chr
    int out9[3] = {tSTRING, tINT, 101};
    int in9[1] = {tINT};
    BSTInsert(funcTab, "chr", 0, createCont(ntFunc, 1, 3, in9, out9, 101));
}