/*
 * XHERMA25 - Lukas Hermann		5	%
 * XSTODU06 - Dan Stodulka		75	%
 * XTRANH00 - Tran Manh Hoang		10	%
 * XMARCE02 - Andrea Marcelyova	10	%
 */
#ifndef __PARSER_LOOPINCLUDEPROTECTION
#define __PARSER_LOOPINCLUDEPROTECTION

#include "global.h"
#include "lexi.h"
#include "ial.h"
#include "expr.h"
#include "inst_array.h"
#include "tkonst.h"


#define PINITSIZE 256

typedef struct PStack
{
    QVarEle* element;
    int32_t size;
    int32_t count;
}*PStack;

PStack PStackInit();                 // Init stack
void PStackPush(PStack,QVarEle);      // Push a symbol onto the stack
QVarEle PStackTopPop(PStack);            // Pops and store a symbol from the stack

int32_t PProcess(table_symbol*,Pfunction_list*,konst_list*);
//int32_t PNumeric(table_symbol*,Pfunction_list*,int32_t);
//int32_t PTypeOf(table_symbol*,Pfunction_list*,int32_t);
//int32_t PLen(table_symbol*,Pfunction_list*,int32_t);
//int32_t PSort(table_symbol*,Pfunction_list*,int32_t);
//int32_t PFind(table_symbol*,Pfunction_list*,int32_t);
//int32_t PInput(table_symbol*,Pfunction_list*,int32_t);
// int32_t PPrint(table_symbol*,Pfunction_list*,int32_t,konst_list*);
//int32_t PFunction(table_symbol*,Pfunction_list*,int32_t);
int32_t PId(table_symbol*,Pfunction_list*,int32_t,konst_list*);
//int32_t PWhile(table_symbol*);
//int32_t PIf(table_symbol*);
//int32_t PReturn(table_symbol*);


#endif //__PARSER_LOOPINCLUDEPROTECTION
