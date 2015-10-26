/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   strings.h
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

#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gc.h"

#define SIZE_STEP 16

typedef struct string{
	char* data;
	unsigned int size;
	unsigned int used;
} STRING, *STRING_PTR;

void strInit(STRING_PTR);
void strInitRaw(STRING_PTR, char*);
void strInitString(STRING_PTR, STRING_PTR);

void strAppend(STRING_PTR, char);
void strCopy(STRING_PTR, STRING_PTR);
void strClear(STRING_PTR);
void strFree(STRING_PTR);

int strCompare(STRING, char*);
int strCompareStrings(STRING, STRING);

unsigned int strSize(STRING_PTR);
unsigned int strUsed(STRING_PTR);

void strConcatenate(STRING_PTR first,STRING_PTR second);

#endif
