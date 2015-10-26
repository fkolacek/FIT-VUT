/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   ial.h
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

#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include "errors.h"
#include "gc.h"
#include "strings.h"
#include "list.h"
#include "scanner.h"


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

typedef union VALUE{
    bool valBool;
    int valInt;
    double valDouble;
    STRING valString;    
} VALUE;

typedef enum TYPE{
	TYPE_NULL = 0,
    TYPE_BOOL = 1,
    TYPE_DIGIT_INT = 2,
	TYPE_DIGIT_DOUBLE = 3,
    TYPE_STRING = 4,
    TYPE_CALLABLE = 5,
	TTYPE_CONSTANT = 6,
    TYPE_OTHER = 7
} TYPE;

typedef enum INSTRUCTION_TYPE{
    INSTRUCTION_RETURN = 1,
	
	INSTRUCTION_POP,
	INSTRUCTION_PUSH,
	
	
	INSTRUCTION_MOV,         	//	A B		R(A) := R(B)  		(konstantu move nepresouva)
    INSTRUCTION_LOAD_NULL,	//	A B     R(A) := ... := R(B) := nil       local a;
    INSTRUCTION_NOP,
	
	INSTRUCTION_ADDITION, //TTYPE_ADDITION
    INSTRUCTION_SUBSTRACTION,
	INSTRUCTION_MULTIPLICATION,
	INSTRUCTION_DIVISION,
	INSTRUCTION_CONCATE,	// puvodni power
	
	INSTRUCTION_EQUAL,
	INSTRUCTION_NOT_EQUAL,
	INSTRUCTION_LESS, // = 15
	INSTRUCTION_LESS_EQUAL,
	INSTRUCTION_GREATER,
	INSTRUCTION_GREATER_EQUAL,
	
	INSTRUCTION_JUMP,		// C - dest
	INSTRUCTION_IF_JUMP,	// B - symbol C - dest 
	INSTRUCTION_LABEL,	// 21
	
	INSTRUCTION_CALL,
	
	INSTRUCTION_BOOLVAL,
	INSTRUCTION_DOUBLEVAL,
	INSTRUCTION_INTVAL,
	INSTRUCTION_STRVAL,
	
	INSTRUCTION_GET_STRING,		// A
	INSTRUCTION_PUT_STRING,		// -neresim
	INSTRUCTION_STRLEN,			// A C
	INSTRUCTION_GET_SUBSTRING,	// A B C
	INSTRUCTION_FIND_STRING,		// A B C
	INSTRUCTION_SORT_STRING,		// A
	
} INSTRUCTION_TYPE;

typedef struct ITEM{
    TYPE type;
    VALUE value;
} ITEM, *ITEMPTR;

typedef struct SYMBOL{
    STRING id;
    TYPE type;    
    ITEMPTR items;
	bool filgy;
} SYMBOL, *SYMBOL_PTR;


typedef struct INSTRUCTION{
    INSTRUCTION_TYPE type;
    PTR operand1;
    PTR operand2;
	PTR destionation;
} INSTRUCTION, *INSTRUCTION_PTR;

typedef struct FUNCTION{
    STRING id;
    
    struct_BTree symbols;
    LIST instructions;
    LIST staticValues;
    
    int calledCount;
	
	int argumentsCount;
	int argumentsCalled;
} FUNCTION, *FUNCTION_PTR;

typedef struct SYMBOL_TABLE{
    struct_BTree functions;
    
    FUNCTION start;
    FUNCTION_PTR curr;    
} SYMBOL_TABLE, *SYMBOL_TABLE_PTR;

void stInit(SYMBOL_TABLE_PTR st);
void stInitLibFunctions(SYMBOL_TABLE_PTR st);

ERROR stInsertFunction(SYMBOL_TABLE_PTR st, STRING id);
ERROR stInsertSymbol(FUNCTION_PTR function, STRING id);

FUNCTION_PTR stSearchFunction(SYMBOL_TABLE_PTR st, STRING id);
SYMBOL_PTR stInsertStaticValue(FUNCTION_PTR function, STRING id, enum_RetVal retval, SYMBOL_PTR* symbol);
SYMBOL_PTR stInsertStaticValueEmpty(FUNCTION_PTR function);
SYMBOL_PTR stSearchSymbol(FUNCTION_PTR function, STRING id);
SYMBOL_PTR stGetLastSymbol(FUNCTION_PTR function);

void stFree(SYMBOL_TABLE_PTR st);

//Helpers
INSTRUCTION_PTR makeInstruction(INSTRUCTION_TYPE type, PTR operand1, PTR operand2, PTR destionation);

const char* debugInstruction(INSTRUCTION_TYPE i);

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