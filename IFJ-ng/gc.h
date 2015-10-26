/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   gc.h
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

#ifndef GC_H_INCLUDED
#define GC_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "errors.h"


#define GC_ITEM_MEMORY 1
#define GC_ITEM_FILE 2

typedef struct GC_ITEM{
        int type;
        struct GC_ITEM * next;
        PTR block;
} GC_ITEM, *GC_ITEM_PTR;


void gcInit();
PTR gcMalloc(int size);
PTR gcRealloc(PTR block, int size);
PTR gcCalloc(int count, int size);
PTR gcFopen(const char* fileName, const char* mode);
void gcFree(PTR);
void gcFclose(PTR);
void gcDispose();
void gcAbort();
void printList();

#endif
