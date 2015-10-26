/*
 * XHERMA25 - Lukas Hermann		50	%
 * XSTODU06 - Dan Stodulka		0	%
 * XTRANH00 - Tran Manh Hoang		25	%
 * XMARCE02 - Andrea Marcelyova	25	%
 */
#ifndef IAL_H_INCLUDED
#define IAL_H_INCLUDED

#include "global.h"

//++++++++++++++++++++++++ DEFINITIONS ++++++++++++++++++++++++|
#define HTSIZE 101
//------------------- DEFINITIONS ------------------|

/**
 * @file	ial.h
 * @brief	IAL functions.
 * heap sort
 * Knuth Morris Pratt algorithm
 * Symbol Table
 * @date	21/11/2012
 * @author	Lukas Hermann, xherma25@stud.fit.vutbr.cz
 */


//+++++++++++++++++++++ STRUCT +++++++++++++++++++++|
 typedef struct struct_symbol{
	char* key;
	Type data;
	struct struct_symbol* ptrnext;
} struct_symbol;

typedef struct_symbol* table_symbol[HTSIZE];
//---------------- STRUCT ----------------|

/**
 * @info Make a hash from a string.
 * @param char* - string to be hashed
 * @return int - return hash value
 */
int TSymbol_Hash(char* key );

/**
 * @info Initialize hash table.
 * @return table_symbol* - pointer on hash table
 */
table_symbol* TSymbol_Init();

/**
 * @info Search item in table.
 * @param table_symbol* - pointer on hash table
 * @param char* - hash to get index
 * @return struct_symbol* - pointer to found item
 */
struct_symbol* TSymbol_Search(table_symbol* ptr, char* key );

/**
 * @info Insert new item into table.
 * @param table_symbol* - pointer on hash table
 * @param char* - hash
 * @param tData - data to insert
 * @return void
 */
int32_t TSymbol_Insert(table_symbol* ptr, char* key, Type data );

/**
 * @info Return data of specific item.
 * @param table_symbol* - pointer on hash table
 * @param char* - hash
 * @return tData* - pointer to data
 */
Type * TSymbol_Read(table_symbol* ptr, char* key );

/**
 * @info Delete item from a table.
 * @param table_symbol* - pointer to a hash table
 * @param char* - hash
 * @return void
 */
void TSymbol_Delete(table_symbol* ptr, char* key );

/**
 * @info Free the table.
 * @param table_symbol* - pointer to a hash table
 * @return void
 */
void TSymbol_Free(table_symbol* ptr );



/**
 * @info Sort a string with heap sort.
 * @param s_string - string to be sorted
 * @return void
 */
s_string Heapsort(s_string);

/**
 * @info Find a substring in a string
 * @param s_string - where to find
 * @param s_string - what to find
 * @return int32_t - index where the substring begins.
 */
int32_t Find_string(s_string, s_string);


#endif // IAL_H_INCLUDED
