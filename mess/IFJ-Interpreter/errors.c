/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   errors.c
 *
 * Popis:
 *
 *
 * Datum:    12.9.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#include "errors.h"

void ChuckSaid(ERROR ecode, const char* msg){
	fprintf(stderr, "%s\n", msg);
	exit(ecode);
}

void ChuckSaidNo(ERROR ecode){
	fprintf(stderr, "Chuck rekl ne\n");
	exit(ecode);
}