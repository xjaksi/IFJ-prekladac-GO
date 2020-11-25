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
#include "symtable.h"

int parse()
{
    treeNode funcTab;
    treeNode localTab;

    BSTInit(&funcTab);
    BSTInit(&localTab);


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
    result = cScel(&token, &funcTab, &localTab);

    DLDisposeList(&token);

    BSTDispose(&funcTab);
    BSTDispose(&localTab);

    return result;

}

int cScel(tokenList *token, treeNode *funcTab, treeNode *localTab)
{
    int result;

    // deklarovani funkci pred kontrolou
    result = funcSave(token, &funcTab);
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

        while (token->Act->t_type != tLBRACE)
        {
            if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
            token->Act = token->Act->rptr;
        }
        token->Act = token->Act->rptr;


        if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
        result = cBody(token, &funcTab, &localTab);
        if (result != OK) return result;

        if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
    }

    return result;
}

int funcSave(tokenList *token, treeNode *funcTab)
{
    int isMain = 0;

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
                // ulozeni nazvu funkce a kontrola parametru
                if (token->Act->t_type != tID) return ERROR_SYNTAX;

            }
            
        }
        
        if (token->Act == NULL) return ERROR_COMPILER;
        token->Act = token->Act->rptr;
    }
    
    if (isMain != 1) return ERROR_SYNTAX;
    
    return OK;
}

int cBody(tokenList *token, treeNode *funcTab, treeNode *localTab)
{
    int result = OK;
    bool dev = false;

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
            if (token->Act->t_type == tDEVNULL) dev = true;

            result = cId(token, dev, &funcTab, &localTab);
            if (result != OK) return result;
            break;

        case kwIF:
            result = cIf(token, &funcTab, &localTab);
            if (result != OK) return result;
            break;

        case kwFOR:
            result = cFor(token, &funcTab, &localTab);
            if (result != OK) return result;
            break;

        case kwRETURN:
            token->Act = token->Act->rptr;
            if (token->Act->t_type == tEOL)
            {
                result = OK;
                break;
            }
            result = cExpr(token, &funcTab, &localTab);
            if (result != OK) return result;
            break;

        case fINPUTS:
        case fINPUTI:
        case fINPUTF:
        case fPRINT:
        case fINT2FLOAT:
        case fFLOAT2INT:
        case fLEN:
        case fSUBSTR:
        case fORD:
        case fCHR:
            while (token->Act->t_type != tRBRACKET)
            {
                if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
                token->Act = token->Act->rptr;
            }
            token->Act = token->Act->rptr;
            if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
            break;
        
        default:

            return ERROR_SYNTAX;
        }

    }
    
    return result;
}

int cId(tokenList *token, bool dev, treeNode *funcTab, treeNode *localTab)
{
    int cnt = 0;
    int result;

    token->Act = token->Act->rptr;

    while (token->Act->t_type == tCOMMA)
    {
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tID) return ERROR_SYNTAX;
        cnt++;
        token->Act = token->Act->rptr;
    }
    
    switch (token->Act->t_type)
    {
    case tLBRACKET:
        // pokud je to funkce musi byt sama
        if (cnt != 0) return ERROR_SYNTAX;
        if (dev) return ERROR_SYNTAX;
        result = cFunc(token, &funcTab, &localTab);
        token->Act = token->Act->rptr;
        break;

    case tASSIGN:
        token->Act = token->Act->rptr;
        if (token->Act->t_type == tID && token->Act->rptr->t_type == tLBRACKET)
        {
            token->Act = token->Act->rptr;
            result = cFunc(token, &funcTab, &localTab);
            if (result != OK) return result;
            token->Act = token->Act->rptr;
            break;
        }

        // pokud ne vestavena funkce, pak kontrola vyrazu
        switch (token->Act->t_type)
        {
        case fINPUTS:
        case fINPUTI:
        case fINPUTF:
        case fPRINT:
        case fINT2FLOAT:
        case fFLOAT2INT:
        case fLEN:
        case fSUBSTR:
        case fORD:
        case fCHR:
            result = cFunc(token, &funcTab, &localTab);
            if (result != OK) return result;
            break;
        default:
            result = cExpr(token, &funcTab, &localTab);
            if (result != OK) return result;
            break;
        }
        
        while (token->Act->t_type == tCOMMA)
        {
            token->Act = token->Act->rptr;
            result = cExpr(token, &funcTab, &localTab);
            if (result != OK) return result;
            token->Act = token->Act->rptr;
        }
        break;

    case tDEF:
        if (dev) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
        result = cExpr(token, &funcTab, &localTab);
        if (result != OK) return result;
        break;
           
    default:
        return ERROR_SYNTAX;
    }
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    return OK;
}


int cIf(tokenList *token, treeNode *funcTab, treeNode *localTab)
{
    int result;
    token->Act = token->Act->rptr;
    result = cExpr(token, &funcTab, &localTab);

    if (result != OK) return result;


    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
    // telo if
    result = cBody(token, &funcTab, &localTab);
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
    result = cBody(token, &funcTab, &localTab);
    if (result != OK) return result;
    if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;


    if (token->Act->rptr->t_type == tEOF) return ERROR_SYNTAX;

    token->Act = token->Act->rptr->rptr;


    return OK;
}

int cFor(tokenList *token, treeNode *funcTab, treeNode *localTab)
{
    int result;

    token->Act = token->Act->rptr;
    //definice
    if (token->Act->t_type == tID)
    {
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tDEF) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
        result = cExpr(token, &funcTab, &localTab);
        if (result != OK) return result;
    }
    if (token->Act->t_type != tSEMICOLON) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;

    // vyraz
    if (token->Act->t_type != tSEMICOLON)
    {
        result = cExpr(token, &funcTab, &localTab);
        if (result != OK) return result;
    }
    if (token->Act->t_type != tSEMICOLON) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;

    // prirazeni
    if (token->Act->t_type == tID)
    {
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tASSIGN) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
        result = cExpr(token, &funcTab, &localTab);
        if (result != OK) return result;
    }
    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    // telo for
    result = cBody(token, &funcTab, &localTab);
    if (result != OK) return result;

    token->Act = token->Act->rptr;
    return OK;
}

int cFunc(tokenList *token, treeNode *funcTab, treeNode *localTab)
{
    while (token->Act->t_type != tRBRACKET)
    {
        if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
    }
    return OK;
}

int cParams(tokenList *token, treeNode *funcTab, treeNode *localTab)
{
    // zatim preskakuji parametry po zacatek funkce
    while (token->Act->t_type != tLBRACE && token->Act != token->Last)
    {
        token->Act = token->Act->rptr;
    }
    token->Act = token->Act->rptr;
    return OK;
}

int cExpr(tokenList *token, treeNode *funcTab, treeNode *localTab)
{
    int result = OK;
    tokenList newToken;

    DLInitList(&newToken);

    while (token->Act->t_type != tEOL &&
            token->Act->t_type != tSEMICOLON &&
            token->Act->t_type != tLBRACE &&
            token->Act->t_type != tEOF )
    {
        DLInsertLast(&newToken, token->Act->t_type, NULL);
        token->Act = token->Act->rptr;
    }
    if (newToken.First == NULL) {
        return ERROR_SYNTAX;
    }

    result = parseExp(&newToken);
    

    DLDisposeList(&newToken);
    return result;
}


void buidInFunc(treeNode *funcTab)
{
    // inputs
    int out[2] = {tSTRING, tINT};
    BSTInsert(&funcTab, "inputs", false, createCont(ntFunc, 0, 2, 101, out, 101));
    //inputi
    int out2[2] = {tINT, tINT};
    BSTInsert(&funcTab, "inputi", false, createCont(ntFunc, 0, 2, 101, out, 101));
    // inputf
    int out3[2] = {tFLOAT, tINT};
    BSTInsert(&funcTab, "inputf", false, createCont(ntFunc, 0, 2, 101, out, 101));
    // print
    BSTInsert(&funcTab, "print", false, createCont(ntFunc, 101, 0, 101, 101, 101));
    // int2float
    int out4[1] = {tFLOAT};
    int in4[1] = {tINT};
    BSTInsert(&funcTab, "int2float", false, createCont(ntFunc, 1, 1, in4, out4, 101));
    // float2int
    int out5[1] = {tINT};
    int in5[1] = {tFLOAT};
    BSTInsert(&funcTab, "float2int", false, createCont(ntFunc, 1, 1, in5, out5, 101));
    // len
    int out6[1] = {tINT};
    int in6[1] = {tSTRING};
    BSTInsert(&funcTab, "len", false, createCont(ntFunc, 1, 1, in6, out6, 101));
    // substr
    int out7[2] = {tSTRING, tINT};
    int in7[3] = {tSTRING, tINT, tINT};
    BSTInsert(&funcTab, "substr", false, createCont(ntFunc, 3, 2, in7, out7, 101));
    // ord
    int out8[2] = {tINT, tINT};
    int in8[2] = {tSTRING, tINT};
    BSTInsert(&funcTab, "ord", false, createCont(ntFunc, 2, 2, in8, out8, 101));
    // chr
    int out9[2] = {tSTRING, tINT};
    int in9[1] = {tINT};
    BSTInsert(&funcTab, "chr", false, createCont(ntFunc, 1, 2, in9, out9, 101));

}