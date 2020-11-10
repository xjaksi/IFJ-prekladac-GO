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
#include "errors.h"

int parse()
{
    treeNode funcTab;
    treeNode localTab;
    BSTInit(&funcTab);
    BSTInit(&localTab);

/* Vytvoreni seznamu pro tokeny a ukonceni v pripade chyby !!!OVERIT!!!*/
    tListPtr token = initScanner();
    get_token(token);

    // kontrola hlavicky programu 'package main'
    if (token->act->type != kwPACKAGE)
    {
        return ERROR_SYNTAX;
    }
    token->act = token->act->next;
    if (token->act->type != fMAIN)
    {
        return ERROR_SYNTAX;
    }
    token->act = token->act->next;

    // kontrola programu
    int result = cProg(&token, &funcTab, &localTab);

    BSTDispose(&funcTab);
    BSTDispose(&localTab);

    return result;

}

int cProg(tListPtr *token, treeNode *funcTab, treeNode *localTab)
{
    return OK;
}