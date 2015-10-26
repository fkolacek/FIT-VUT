/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   lib.h
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

#ifndef LIB_H_INCLUDED
#define LIB_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "strings.h"
#include "symbol_table.h"

// ChuckSaid(E_LEXICAL, "Neplatny lexem");

ITEMPTR boolval(ITEM);
ITEMPTR doubleval(ITEM);
ITEMPTR intval(ITEM);
ITEMPTR strval(ITEM);

STRING get_string();
// int put_string(int count,  ...);
void put_string(char*);
int my_strlen(STRING);
STRING get_substring(STRING, int, int, ERROR*);

int find_string(STRING, STRING);

void sort_string(STRING);


#endif