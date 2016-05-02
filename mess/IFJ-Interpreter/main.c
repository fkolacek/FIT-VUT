/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   main.c
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

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"
#include "gc.h"
#include "scanner.h"
#include "symbol_table.h"
#include "parser.h"
#include "interpret.h"

void programAbort();

int main(int argc, char* argv[]){

	if(argc != 2)
		ChuckSaid(E_COMPILATOR, "Invalid arguments");

	gcInit();
	
	if(!(glob_FileHandler = gcFopen(argv[1], "r")))
		ChuckSaid(E_COMPILATOR, "Cannot open input file\n");
	
	SYMBOL_TABLE st;
	
	stInit(&st);
	
	ERROR err = parser(&st);
	
	if(err == E_OK)
		err = interpret(&st);
	
	stFree(&st);
	
	gcFclose(glob_FileHandler);	

	gcDispose();
	
	return err;
}