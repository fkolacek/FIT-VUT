/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   stack.h
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

#ifndef TYPE_PTR_DEFINED
#define TYPE_PTR_DEFINED
typedef void* PTR;
#endif

#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "gc.h"

typedef struct STACK_NODE{
	PTR value;
	struct STACK_NODE* next;		
} STACK_NODE, *STACK_NODE_PTR;

typedef struct STACK{
	STACK_NODE_PTR begin;
	int count;	
} STACK, *STACK_PTR;


void stackInit(STACK_PTR stack);
void stackPush(STACK_PTR stack, PTR value);
PTR stackPop(STACK_PTR stack);
PTR stackBegin(STACK_PTR stack);
int stackCount(STACK_PTR stack);
void stackFree(STACK_PTR stack);


#endif