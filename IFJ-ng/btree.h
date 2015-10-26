/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   btree.h
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

#ifndef BTREE_H_INCLUDED
#define BTREE_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>
#include "errors.h"
#include "gc.h"
#include "strings.h"

typedef struct struct_BTree_Node{
    struct struct_BTree_Node* left;
	struct struct_BTree_Node* right;

	STRING_PTR key;
	void *data;
} *struct_BTree_Node;


typedef struct struct_BTree{
	struct_BTree_Node root;
	struct_BTree_Node last;
} struct_BTree;

// API
void 				BT_Init(struct_BTree*);
void 				BT_Free(struct_BTree*);
ERROR				BT_Insert(struct_BTree*, STRING_PTR key, void* data);
struct_BTree_Node 	BT_Search(struct_BTree*, STRING_PTR key);

// library functions
struct_BTree_Node 	recursive_Node_Search(struct_BTree_Node, STRING_PTR key);
void 				recursive_Node_Delete(struct_BTree_Node);

#endif
