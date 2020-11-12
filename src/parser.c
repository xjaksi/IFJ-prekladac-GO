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

int parse()
{

    tokenList token;

    DLInitList(&token);

    /*
    DLInsertLast(&token, kwPACKAGE, NULL);
    DLInsertLast(&token, fMAIN, NULL);
    DLInsertLast(&token, tEOL, NULL);
    DLInsertLast(&token, kwFUNC, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tLBRACKET, NULL);
    DLInsertLast(&token, tRBRACKET, NULL);
    DLInsertLast(&token, tLBRACKET, NULL);
    DLInsertLast(&token, tRBRACKET, NULL);
    DLInsertLast(&token, tLBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);
    
    DLInsertLast(&token, kwRETURN, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tEOL, NULL);
    DLInsertLast(&token, tRBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);
    DLInsertLast(&token, tEOL, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, kwFUNC, NULL);
    DLInsertLast(&token, fMAIN, NULL);
    DLInsertLast(&token, tLBRACKET, NULL);
    DLInsertLast(&token, tRBRACKET, NULL);
    DLInsertLast(&token, tLBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, kwIF, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tLEQ, NULL);
    DLInsertLast(&token, tINT, NULL);
    DLInsertLast(&token, tLBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tDEF, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tMUL, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tEOL, NULL);
    DLInsertLast(&token, tRBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, kwELSE, NULL);
    DLInsertLast(&token, tLBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, tDEVNULL, NULL);
     DLInsertLast(&token, tASSIGN, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tLBRACKET, NULL);
        DLInsertLast(&token, tID, NULL);

     DLInsertLast(&token, tRBRACKET, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, kwFOR, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tDEF, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tSEMICOLON, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tLEQ, NULL);
    DLInsertLast(&token, tINT, NULL);
    DLInsertLast(&token, tSEMICOLON, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tASSIGN, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tSUB, NULL);
    DLInsertLast(&token, tINT, NULL);
    DLInsertLast(&token, tLBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tASSIGN, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tADD, NULL);
    DLInsertLast(&token, tID, NULL);
    DLInsertLast(&token, tEOL, NULL);
    DLInsertLast(&token, tRBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, tRBRACE, NULL);
    DLInsertLast(&token, tEOL, NULL);

    DLInsertLast(&token, tRBRACE, NULL);
    DLInsertLast(&token, tEOF, NULL);

    // 
    */

    int result;
    result = getTokensTo(&token);
    if (result != OK) return result;




    // kontrola hlavicky programu 'package main'
    if (token.First->t_type != kwPACKAGE) return ERROR_SYNTAX;
    token.Act = token.First->rptr;

    if (token.Act->t_type != fMAIN) return ERROR_SYNTAX;
    token.Act = token.Act->rptr;

    if (token.Act->t_type != tEOL) return ERROR_SYNTAX;
    token.Act = token.Act->rptr;

    // kontrola programu
    result = cScel(&token);

    DLDisposeList(&token);

    return result;

}

int cScel(tokenList *token)
{
    int result;

    // deklarovani funkci pred kontrolou
    result = funcSave(token);
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
        result = cBody(token);
        if (result != OK) return result;

        if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
    }

    return result;
}

int funcSave(tokenList *token)
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
                //result = BSTInsert(&funcTab, token->Act->value, true, createCont(ntFunc, 0, t_typeNo));
                //if (result != OK) return result;
                //result = cParams(token, &funcTab, token->Act->value);
                //if (result != OK) return result;
            }
            
        }
        
        if (token->Act == NULL) return ERROR_COMPILER;
        token->Act = token->Act->rptr;
    }
    
    if (isMain != 1) return ERROR_SYNTAX;
    
    return OK;
}

int cBody(tokenList *token)
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

            result = cId(token, dev);
            if (result != OK) return result;
            break;

        case kwIF:
            result = cIf(token);
            if (result != OK) return result;
            break;

        case kwFOR:
            result = cFor(token);
            if (result != OK) return result;
            break;

        case kwRETURN:
            token->Act = token->Act->rptr;
            if (token->Act->t_type == tEOL)
            {
                result = OK;
                break;
            }
            result = cExpr(token);
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

int cId(tokenList *token, bool dev)
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
        result = cFunc(token);
        token->Act = token->Act->rptr;
        break;

    case tASSIGN:
        token->Act = token->Act->rptr;
        if (token->Act->t_type == tID && token->Act->rptr->t_type == tLBRACKET)
        {
            token->Act = token->Act->rptr;
            result = cFunc(token);
            if (result != OK) return result;
            token->Act = token->Act->rptr;
            break;
        }
        
        result = cExpr(token);
        if (result != OK) return result;
        while (token->Act->t_type == tCOMMA)
        {
            token->Act = token->Act->rptr;
            result = cExpr(token);
            if (result != OK) return result;
            token->Act = token->Act->rptr;
        }
        break;

    case tDEF:
        if (dev) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
        result = cExpr(token);
        if (result != OK) return result;
        break;
           
    default:
        return ERROR_SYNTAX;
    }
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    return OK;
}


int cIf(tokenList *token)
{
    int result;
    token->Act = token->Act->rptr;
    result = cExpr(token);

    if (result != OK) return result;


    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;
    // telo if
    result = cBody(token);
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
    result = cBody(token);
    if (result != OK) return result;
    if (token->Act->t_type != tRBRACE) return ERROR_SYNTAX;


    if (token->Act->rptr->t_type == tEOF) return ERROR_SYNTAX;

    token->Act = token->Act->rptr->rptr;


    return OK;
}

int cFor(tokenList *token)
{
    int result;

    token->Act = token->Act->rptr;
    //definice
    if (token->Act->t_type == tID)
    {
        token->Act = token->Act->rptr;
        if (token->Act->t_type != tDEF) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
        result = cExpr(token);
        if (result != OK) return result;
    }
    if (token->Act->t_type != tSEMICOLON) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;

    // vyraz
    if (token->Act->t_type != tSEMICOLON)
    {
        result = cExpr(token);
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
        result = cExpr(token);
        if (result != OK) return result;
    }
    if (token->Act->t_type != tLBRACE) return ERROR_SYNTAX;
    token->Act = token->Act->rptr;
    if (token->Act->t_type != tEOL) return ERROR_SYNTAX;

    // telo for
    result = cBody(token);
    if (result != OK) return result;

    token->Act = token->Act->rptr;
    return OK;
}

int cFunc(tokenList *token)
{
    while (token->Act->t_type != tRBRACKET)
    {
        if (token->Act->t_type == tEOF) return ERROR_SYNTAX;
        token->Act = token->Act->rptr;
    }
    return OK;
}

int cParams(tokenList *token)
{
    // zatim preskakuji parametry po zacatek funkce
    while (token->Act->t_type != tLBRACE && token->Act != token->Last)
    {
        token->Act = token->Act->rptr;
    }
    token->Act = token->Act->rptr;
    return OK;
}

int cExpr(tokenList *token)
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