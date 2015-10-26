/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   errors.h
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

#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

typedef enum error{
	E_OK							= 0,
	E_LEXICAL						= 1,
	E_SYNTAX						= 2,
	E_SEMANTIC						= 3,
	E_SEMANTIC_MISS_PARAM		= 4,
	E_SEMANTIC_UNDECLARED		= 5,
	E_SEMANTIC_ZERO_DIV			= 10,
	E_SEMANTIC_DOUBLEVAL			= 11,
	E_SEMANTIC_TYPE_MISMATCH	= 12,
	E_SEMANTIC_OTHER				= 13,
	E_COMPILATOR					= 99	
	
/*
• 1 - chyba v programu v rámci lexikální analýzy (chybná struktura aktuálního lexému).
• 2 - chyba v programu v rámci syntaktické analýzy (chybná syntaxe struktury programu).
• 3 - sémantická chyba v programu – nedefinovaná funkce, pokus o redefinici funkce.
• 4 - sémantická/bˇehová chyba v programu – chybˇející parametr pˇri volání funkce.
• 5 - sémantická/bˇehová chyba v programu – nedeklarovaná promˇenná.
• 10 - sémantická/bˇehová chyba dˇelení nulou.
• 11 - sémantická/bˇehová chyba pˇri pˇretypování na c ˇ íslo (funkce doubleval ).
• 12 - sémantická/bˇehová chyba typové kompatibility v aritmetických a relaˇcních výrazech.
• 13 - ostatní sémantické/bˇehové chyby.
• 99 - interní chyba interpretu tj. neovlivnˇená vstupním programem (napˇr. chyba alokace pamˇeti atd.).
*/
} ERROR;


void ChuckSaid(ERROR ecode, const char* msg);
void ChuckSaidNo(ERROR ecode);

#endif