/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   parser.h
 *
 * Popis:
 *
 *
 * Datum:    28.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#include "errors.h"
#include "gc.h"
#include "symbol_table.h"
#include "scanner.h"
#include "exp.h"

#include <assert.h>

ERROR parser(SYMBOL_TABLE_PTR st);
ERROR parserFindFunctions(SYMBOL_TABLE_PTR st);
ERROR parserParse(SYMBOL_TABLE_PTR st);
ERROR parserParseFunction(SYMBOL_TABLE_PTR st);
ERROR parserParseFunctionParam(SYMBOL_TABLE_PTR st, bool test);
ERROR parserParseFunctionParams(SYMBOL_TABLE_PTR st, bool test);
ERROR parserParseFunctionCode(SYMBOL_TABLE_PTR st);
ERROR parserParseCode(SYMBOL_TABLE_PTR st, enum_RetVal retval);
ERROR parserControlAssign(SYMBOL_TABLE_PTR st, SYMBOL_PTR symbol);
ERROR parserParseCallParam(SYMBOL_TABLE_PTR st, FUNCTION_PTR f);
ERROR parserParseCallParams(SYMBOL_TABLE_PTR st, FUNCTION_PTR f);