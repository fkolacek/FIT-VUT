/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   list.h
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

#ifndef TYPE_PTR_DEFINED
#define TYPE_PTR_DEFINED
typedef void* PTR;
#endif

#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include "gc.h"

typedef struct LIST_NODE{
    PTR value;
    struct LIST_NODE* next;    
} LIST_NODE, *LIST_NODE_PTR;

typedef struct LIST{
    LIST_NODE_PTR begin;
    LIST_NODE_PTR end;
    LIST_NODE_PTR curr;
} LIST, *LIST_PTR;

void listInit(LIST_PTR list);
void listInsertBegin(LIST_PTR list, PTR value);
void listInsertEnd(LIST_PTR list, PTR value);
void listInsertPost(LIST_PTR list, PTR value);
void listBegin(LIST_PTR list);
void listSucc(LIST_PTR list);
void listEnd(LIST_PTR list);
void listFree(LIST_PTR list);
LIST_NODE_PTR listAt(LIST_PTR list, int num);

#endif
