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

    // vestavene funkce
    buidInFunc(&funcTab);


    // kontrola hlavicky programu 'package main'
    if (token.First->t_type != kwPACKAGE) return ERROR_SYNTAX;
    token.Act = token.First->rptr;

    if (token.Act->t_type != fMAIN) return ERROR_SYNTAX;
    token.Act = token.Act->rptr;

    if (token.Act->t_type != tEOL) return ERROR_SYNTAX;
    token.Act = token.Act->rptr;

    // kontrola programu
    result = cScel(&token, &funcTab, &tList);

    DLDisposeList(&token);

    BSTDispose(&funcTab);
    treeListDestroy(&tList);

    return result;

}

int cScel(tokenList *token, treeNode *funcTab, treeList *tList)
{
    int result;
    int param;
    int *retVal;
    bool paramsIN = false;
    bool returnWas = false;

    // deklarovani funkci pred kontrolou
    result = funcSave(token, funcTab);
    if (result != OK) return result;


    token->Act = token->First->rptr->rptr->rptr;

    while (token->Act->t_type != tEOF)
    {       
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
        }
        // jinak kontroluji telo funkce
        else
        {
            // tabulka pro funkci
            nodeInfCont TBSNodeCont = BSTSearch(funcTab, token->Act->atribute->str);
            if (TBSNodeCont == NULL) return ERROR_COMPILER;
            if (TBSNodeCont->noReturn != 0) 
            {
                retVal = TBSNodeCont->paramsOut;
            }
            else
            {
                retVal = NULL;
            }

            if (TBSNodeCont->noParams != 0)
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
                        result = BSTInsert(&(tList->first->symtab), token->Act->atribute->str, true, createCont(ntVar, 101,101,NULL,NULL, tINT));
                        if (result != OK) return result;
                    }
                    else if (token->Act->rptr->t_type == kwFLOAT64)
                    {
                        result = BSTInsert(&(tList->first->symtab), token->Act->atribute->str, true, createCont(ntVar, 101,101,NULL,NULL, tFLOAT));
                        if (result != OK) return result;
                    }
                    else if (token->Act->rptr->t_type == kwSTRING)
                    {
                        result = BSTInsert(&(tList->first->symtab), token->Act->atribute->str, true, createCont(ntVar, 101,101,NULL,NULL, tSTRING));
                        if (result != OK) return result;
                    }
                    else
                    {
                        return ERROR_SYNTAX;
                    }

                    token->Act = token->Act->rptr->rptr;
                    if (token->Act->t_type == tCOMMA) token->Act = token->Act->rptr; 
                }
            }
            else
            {
                paramsIN = false;
            }
        }
        



        while (token->Act->t_type != tLBRACE)
        {
            if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
            token->Act = token->Act->rptr;
        }
        token->Act = token->Act->rptr;


        if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
        result = cBody(token, funcTab, tList, retVal, &returnWas);
        if (result != OK) return result;

        if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;

        // odchazim z tela funkce
        if (returnWas == false && retVal != NULL) return ERROR_RETURN_VALUE;
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
                isMain++;
                if (token->Act->rptr->t_type != tLBRACKET && token->Act->rptr->rptr->t_type != tRBRACKET)
                        return ERROR_SYNTAX;
                token->Act = token->Act->rptr->rptr->rptr;

                if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
            }
            else
            {   
                if (isMain > 0) return ERROR_SYNTAX;

                // ulozeni nazvu funkce a kontrola parametru
                if (token->Act->t_type != tID) return ERROR_SYNTAX;

                char *idName = token->Act->atribute->str;

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
                if (noArg > 1 && noArg != noComma+1) return ERROR_SYNTAX;
                if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;

                int args[ noArg ];
                // pokud existuji argumenty kontroluji je od zacatku
                if (noArg > 0)
                {
                    while (token->Act->lptr->t_type != tLBRACKET) token->Act = token->Act->lptr;
                
                    for (int i = 0; i < noArg; i++)
                    {
                        if (token->Act->t_type != tID) return ERROR_SYNTAX;

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

                    int ret[ noRet ];
                    if (noRet > 0)
                    {
                        // posun na zacatek navratovych hodnot
                        while (token->Act->lptr->t_type != tLBRACKET) token->Act = token->Act->lptr;

                        for (int i = 0; i < noRet; i++)
                        {
                            if (token->Act->t_type != kwINT &&
                                token->Act->t_type != kwSTRING &&
                                token->Act->t_type != kwFLOAT64)
                                    return ERROR_SYNTAX;
                            ret[i] = token->Act->t_type;
                            token->Act = token->Act->rptr;

                            // za datovym typem nasleduje konec nebo dalsi
                            if (token->Act->t_type == tRBRACKET)
                            {
                                if (i+1 != noRet) return ERROR_SYNTAX; 
                                break;
                            }
                            if (token->Act->t_type != tCOMMA) return ERROR_SYNTAX;
                            token->Act = token->Act->rptr;
                        }
                    }

                    if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
                    token->Act = token->Act->rptr;

                    result = BSTInsert(funcTab, idName, true, createCont(ntFunc, noArg, noRet, args, ret, 101));
                    if (result != OK) return result;
                }
                else
                {
                    result = BSTInsert(funcTab, idName, true, createCont(ntFunc, noArg, noRet, 0, NULL, 101));
                    if (result != OK) return result;
                }
                if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
            }
            
        }
        
        if (token->Act == NULL) return ERROR_COMPILER;
        token->Act = token->Act->rptr;
    }
    
    if (isMain != 1) return ERROR_SYNTAX;
    
    return OK;
}

int cBody(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal, bool *returnWas)
{
    int result = OK;
    int type;

    // vytvoreni lokalni tabulky pro soucasne telo
    treeNode localTab;
    BSTInit(&localTab);
    treeListInsert(tList, &localTab);


    while (token->Act->t_type != tRBRACE)
    {   
        if (token->Act->t_type == tEOF) return ERROR_SYNTAX;

        while (token->Act->t_type == tEOL)
        {
            if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
            token->Act = token->Act->rptr;
        }
        if (token->Act->t_type == tRBRACE) return result;
        

        switch (token->Act->t_type)
        {
        case tDEVNULL:
        case tID :
            result = cId(token, funcTab, tList);
            if (result != OK) return result;
            break;

        case kwIF:
            result = cIf(token, funcTab, tList, retVal, returnWas);
            if (result != OK) return result;
            break;

        case kwFOR:
            result = cFor(token, funcTab, tList, retVal, returnWas);
            if (result != OK) return result;
            break;

        case kwRETURN:
            *returnWas = true;
            token->Act = token->Act->rptr;
            if (retVal != NULL)
            {
                int type;
                signed int cnt = 0;
                if (token->Act->t_type == tEOL) return ERROR_RETURN_VALUE;
                while (token->Act->t_type != tEOL)
                {
                    if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
                    if (token->Act->t_type == tID)
                    {
                        type = dataSearch(tList, token->Act->atribute->str);
                        if (type == 101) return ERROR_UNDEFINED;
                        if (type != retVal[cnt]) return ERROR_RETURN_VALUE;
                    }
                    else
                    {
                        if (token->Act->t_type == retVal[cnt]) return ERROR_RETURN_VALUE;
                    }
                }
            }
            else
            {
                return ERROR_RETURN_VALUE;
            }
            
            break;
        
        default:

            return ERROR_SYNTAX;
        }

    }

    // odebrani ramce na konci tela
    treeListRemove(tList);
    
    return result;
}

int cId(tokenList *token, treeNode *funcTab, treeList *tList)
{
    int cnt = 1;
    int result;
    int type;

    token->Act = token->Act->rptr;

    while (token->Act->t_type == tCOMMA)
    {
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tID &&
            token->Act->t_type != tDEVNULL) return ERROR_SYNTAX;
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
        token->Act = token->Act->rptr;
        // predam ukazatel na prvni token za =
        result = cAssign(token, funcTab, tList, cnt);
        if (result != OK) return result;
        break;

    case tDEF:
        if (cnt != 1) return ERROR_SYNTAX;
        if (token->Act->lptr->t_type != tID) return ERROR_SYNTAX;
        
        nodeInfCont TBSNodeCont = BSTSearch(funcTab, token->Act->lptr->atribute->str);
        if (TBSNodeCont != NULL) return ERROR_REDEFINITION;
        token->Act = token->Act->rptr;
        result = cExpr(token, tList, &type);
        if (result != OK) return result;
        // pokud je type podporovany typ ulozim
        if (type == DT_INT)
        {
            BSTInsert(&(tList->first->symtab), token->Act->lptr->lptr->atribute->str, true, createCont(ntVar, 101, 101, NULL, NULL, tINT));
        }
        else if (type == DT_STRING)
        {
            BSTInsert(&(tList->first->symtab), token->Act->lptr->lptr->atribute->str, true, createCont(ntVar, 101, 101, NULL, NULL, tSTRING));
        }
        else if (type == DT_FLOAT)
        {
            BSTInsert(&(tList->first->symtab), token->Act->lptr->lptr->atribute->str, true, createCont(ntVar, 101, 101, NULL, NULL, tFLOAT));
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
int cAssign(tokenList *token, treeNode *funcTab, treeList *tList, int item)
{
    int result;

    // prirazuji z funkce
    if (token->Act->t_type == tID && token->Act->rptr->t_type == tLBRACKET)
    {
        token->Act = token->Act->rptr;
        result = cFunc(token, funcTab, tList, item, true);
        return result;
    }

    int list[ item ];
    int type;

    // nahrani datovych typu
    for (int i = 0; i < item; i++)
    {
        result = cExpr(token, tList, &type);
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

        token->Act = token->Act->rptr;
        if (token->Act->t_type == tCOMMA) token->Act = token->Act->rptr;
    }

    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    // jinak jdu na zacatek radku a overuji
    token->Act = token->Act->lptr;
    while (token->Act->lptr->t_type != tEOL)
    {
        if (token->Act->lptr != NULL) return ERROR_COMPILER;
        token->Act = token->Act->lptr;
    }

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
    
    return OK;

}

int cIf(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal, bool *returnWas)
{
    int result;
    int type;
    token->Act = token->Act->rptr;
    result = cExpr(token, tList, &type);
    if (result != OK) return result;
    if (type != DT_BOOL) return ERROR_TYPE_COMPATIBILITY;


    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
    // telo if
    result = cBody(token, funcTab, tList, retVal, returnWas);
    if (result != OK) return result;
    if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    while (token->Act->t_type == tEOL)
    {
        token->Act = token->Act->rptr;
    }

    // prvni ok token je else
    if (token->Act->t_type != kwELSE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
    // telo else
    result = cBody(token, funcTab, tList, retVal, returnWas);
    if (result != OK) return result;
    if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;


    if (token->Act->rptr->t_type == tEOF) return ERROR_SYNTAX;

    token->Act = token->Act->rptr->rptr;


    return OK;
}

int cFor(tokenList *token, treeNode *funcTab, treeList *tList, int *retVal, bool *returnWas)
{
    int result;
    int type, dat;
    bool head = false;

    token->Act = token->Act->rptr;
    //definice
    if (token->Act->t_type == tID)
    {
        // pokud existuje jako funkce, koncim
        nodeInfCont TBSNodeCont = BSTSearch(funcTab, token->Act->atribute->str);
        if (TBSNodeCont != NULL) return ERROR_REDEFINITION;
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
        // vlozeni do tabulky
        if (type == DT_INT)
        {
            BSTInsert(&(tList->first->symtab), name, true, createCont(ntVar, 101, 101, NULL, NULL, tINT));
        }
        else if (type == DT_STRING)
        {
            BSTInsert(&(tList->first->symtab), name, true, createCont(ntVar, 101, 101, NULL, NULL, tSTRING));
        }
        else if (type == DT_FLOAT)
        {
            BSTInsert(&(tList->first->symtab), name, true, createCont(ntVar, 101, 101, NULL, NULL, tFLOAT));
        }
        else
        {
            return ERROR_TYPE_INFERENCE;
        }
        
    }
    if (token->Act->t_type != tSEMICOLON) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;

    // vyraz
    if (token->Act->t_type != tSEMICOLON)
    {
        result = cExpr(token, tList, &type);
        if (result != OK) return result;
        if (type != DT_BOOL) return ERROR_SEMANTICS;
    }
    if (token->Act->t_type != tSEMICOLON) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;

    // prirazeni
    if (token->Act->t_type != tLBRACE)
    {
        if (token->Act->t_type != tID) return ERROR_SYNTAX;
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
    }
    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    // telo for
    result = cBody(token, funcTab, tList, retVal, returnWas);
    if (result != OK) return result;

    // pokud jsem vytvarel hlavicku popnu
    if (head) treeListRemove(tList);

    token->Act = token->Act->rptr;
    return OK;
}

// kontrola funkce, ocekavam token leve zavorky
int cFunc(tokenList *token, treeNode *funcTab, treeList *tList, int noItems, bool ass)
{
    nodeInfCont TBSNodeCont = BSTSearch(funcTab, token->Act->lptr->atribute->str);
    if (TBSNodeCont == NULL) return ERROR_UNDEFINED;

    // funkce existuje kontroluji argumenty
    int noParam = TBSNodeCont->noParams;

    token->Act = token->Act->rptr; // posuny se na token za (
    // funkce nema mit argument
    if (noParam == 0)
    {
        if (token->Act->t_type != tRBRACKET) return ERROR_PARAMETERS;
    }
    // jeden argument
    else if (noParam == 1)
    {
        if (token->Act->t_type != TBSNodeCont->paramsIn[0]) return ERROR_PARAMETERS;
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tRBRACKET) return ERROR_PARAMETERS;
    }
    // vice
    else
    {
        int noC = 0;
        for (int i = 0; i < noParam; i++)
        {
            if (token->Act->t_type != TBSNodeCont->paramsIn[i]) return ERROR_PARAMETERS;
            token->Act = token->Act->rptr;
            if (token->Act->t_type == tCOMMA) noC++;
            token->Act = token->Act->rptr;
        }
        if (token->Act->t_type == tID ||
            token->Act->t_type == tINT ||
            token->Act->t_type == tSTRING ||
            token->Act->t_type == tFLOAT ||
            noC+1 != noParam)
                return ERROR_PARAMETERS;
        if (token->Act->t_type != tRBRACKET) return ERROR_SYNTAX;
    }

    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    // kontrola navratovych hodnot
    if (ass)
    {
        if (TBSNodeCont->noReturn != noItems) return ERROR_SEMANTICS;

        // jdu na zacatek radku
        token->Act = token->Act->lptr;
        while (token->Act->lptr->t_type != tEOL)
        {
            token->Act = token->Act->lptr;
            if (token->Act->lptr == NULL) return ERROR_COMPILER;
        }
        // overovani datovych typu
        int type;
        for (int i = 0; i < noItems; i++)
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
                if (type != TBSNodeCont->paramsOut[i]) return ERROR_RETURN_VALUE;
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
        if (TBSNodeCont->noReturn != 0) return ERROR_SEMANTICS;
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
            token->Act->t_type != tEOF )
    {
        DLInsertLast(&newToken, token->Act->t_type, token->Act->atribute);
        token->Act = token->Act->rptr;
    }
    if (newToken.First == NULL) {
        return ERROR_SYNTAX;
    }

    result = parseExp(&newToken, tList, type);
    

    DLDisposeList(&newToken);
    return result;
}


void buidInFunc(treeNode *funcTab)
{
    // inputs
    int out[2] = {tSTRING, tINT};
    BSTInsert(funcTab, "inputs", false, createCont(ntFunc, 0, 2, NULL, out, 101));
    //inputi
    int out2[2] = {tINT, tINT};
    BSTInsert(funcTab, "inputi", false, createCont(ntFunc, 0, 2, NULL, out2, 101));
    // inputf
    int out3[2] = {tFLOAT, tINT};
    BSTInsert(funcTab, "inputf", false, createCont(ntFunc, 0, 2, NULL, out3, 101));
    // print
    BSTInsert(funcTab, "print", false, createCont(ntFunc, 101, 0, NULL, NULL, 101));
    // int2float
    int out4[1] = {tFLOAT};
    int in4[1] = {tINT};
    BSTInsert(funcTab, "int2float", false, createCont(ntFunc, 1, 1, in4, out4, 101));
    // float2int
    int out5[1] = {tINT};
    int in5[1] = {tFLOAT};
    BSTInsert(funcTab, "float2int", false, createCont(ntFunc, 1, 1, in5, out5, 101));
    // len
    int out6[1] = {tINT};
    int in6[1] = {tSTRING};
    BSTInsert(funcTab, "len", false, createCont(ntFunc, 1, 1, in6, out6, 101));
    // substr
    int out7[2] = {tSTRING, tINT};
    int in7[3] = {tSTRING, tINT, tINT};
    BSTInsert(funcTab, "substr", false, createCont(ntFunc, 3, 2, in7, out7, 101));
    // ord
    int out8[2] = {tINT, tINT};
    int in8[2] = {tSTRING, tINT};
    BSTInsert(funcTab, "ord", false, createCont(ntFunc, 2, 2, in8, out8, 101));
    // chr
    int out9[2] = {tSTRING, tINT};
    int in9[1] = {tINT};
    BSTInsert(funcTab, "chr", false, createCont(ntFunc, 1, 2, in9, out9, 101));

}