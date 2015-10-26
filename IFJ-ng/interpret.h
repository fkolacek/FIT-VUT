/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   interpret.h
 *
 * Popis:
 *
 *
 * Datum:    20.11.2013
 *
 * Autori:   Frantisek Kolacek	<xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel 	<xstodu06@stud.fit.vutbr.cz>
 *           Hermann Lukas 		<xherma25@stud.fit.vutbr.cz>
 *           Tran Manh Hoang 	<xtranh00@stud.fit.vutbr.cz>
 */

#ifndef INTERPRET_H_INCLUDED
#define INTERPRET_H_INCLUDED

#include <math.h>
#include "errors.h"
#include "stack.h"

#include "symbol_table.h"
#include "strings.h"
#include "lib.h"


// --- API ---
ERROR interpret(SYMBOL_TABLE*);
ERROR recursive_interpret(FUNCTION_PTR, STACK_PTR);


#endif