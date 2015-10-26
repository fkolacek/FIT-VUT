/*
 * XHERMA25 - Lukas Hermann		10	%
 * XSTODU06 - Dan Stodulka		75	%
 * XTRANH00 - Tran Manh Hoang		10	%
 * XMARCE02 - Andrea Marcelyova	5	%
 */


#ifndef __EXPR_LOOPINCLUDEPROTECTION
#define __EXPR_LOOPINCLUDEPROTECTION

#include "parser.h"
#include "global.h"
#include "ial.h"
#include "lexi.h"
#include "tkonst.h"



static int32_t CTable[15][15] = {
    /*              0       1       2       3       4       5       6       7       8       9      10      11      12      13      14
                    ==      !=      <=      >=      <       >       -       +       /       *       **      (       )       i       $                                                                                                                                    */
    /* == 0*/   {    EGt,    EGt,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EGt,    ELe,
},
    /* != 1*/   {    EGt,    EGt,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EGt,    ELe,
},
    /* <= 2*/   {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EGt,    ELe,
},
    /* >= 3*/   {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EGt,    ELe,
},
    /* < 4*/    {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EGt,    ELe,
},
    /* > 5*/    {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EGt,    ELe,
},
    /* - 6*/    {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EGt,    ELe,
},
    /* + 7*/    {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EGt,    ELe,
},
    /* / 8*/    {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    EGt,    ELe,    EGt,    ELe,
},
    /* * 9*/    {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    EGt,    ELe,    EGt,    ELe,
},
    /* ** */   {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ELe,    ELe,    EGt,    ELe,
},
    /* ( */    {    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    ELe,    EEq,    ELe,
},
    /* ) */    {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ERr,    EGt,    ERr,
},
    /* i */    {    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    EGt,    ERr,    EGt,    ERr,
}
};

typedef struct Stack
{
    ELogic* element;
    int32_t size;
    int32_t count;
}*Stack;

typedef struct ExprArray
{
    QVarEle* elements;
    int32_t count;
    int32_t size;
    QType type;
}*Expr;



Expr ExprInit();
int32_t ProcessExpr(table_symbol*,konst_list*,Expr);
// void PrintExpr(Expr,konst_list*);
#endif //__EXPR_LOOPINCLUDEPROTECTION
