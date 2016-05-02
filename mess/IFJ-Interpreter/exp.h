/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   exp.h
 *
 * Popis:
 *
 *
 * Datum:    29.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#ifndef EXP_H_INCLUDED
#define EXP_H_INCLUDED

#include "errors.h"
#include "gc.h"
#include "symbol_table.h"
#include "scanner.h"
#include "stack.h"

#define EXPRESSION_TABLE_SIZE TTYPE_SEMICOLON+1

#define isop(r) \
( \
	r == TTYPE_ADDITION || \
	r == TTYPE_SUBTRACTION || \
	r == TTYPE_MULTIPLICATION || \
	r == TTYPE_DIVISION || \
	r == TTYPE_DOT || \
	r == TTYPE_LESSER || \
	r == TTYPE_GREATER || \
	r == TTYPE_LESSER_EQUAL || \
	r == TTYPE_GREATER_EQUAL || \
	r == TTYPE_EQUAL || \
	r == TTYPE_NOT_EQUAL \
)

#define type(r) r + (INSTRUCTION_ADDITION - TTYPE_ADDITION)


typedef struct EXPRESSION{
	enum_RetVal retval;
	SYMBOL_PTR symbol;
} EXPRESSION, *EXPRESSION_PTR;

ERROR parserExpression(SYMBOL_TABLE_PTR st, enum_RetVal retval, SYMBOL_PTR* symbol);

//helpers
EXPRESSION_PTR makeExpression(enum_RetVal retval, SYMBOL_PTR symbol);
enum_RetVal getTerm(STACK_PTR stack);
ERROR pushExpression(SYMBOL_TABLE_PTR st, STACK_PTR stack, SYMBOL_PTR symbol, enum_RetVal retval);

#endif