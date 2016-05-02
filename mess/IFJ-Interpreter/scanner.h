/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu imperativniho jazyka
 * Varianta: a/1/I
 * Soubor:   scanner.h
 *
 * Popis:
 *
 *
 * Datum:    21.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"
#include "strings.h"

extern FILE* glob_FileHandler;

typedef enum enum_RetVal
{
	TTYPE_L_SBRACKET, TTYPE_R_SBRACKET,				//	[	]
	TTYPE_L_BRACE, TTYPE_R_BRACE,						//	{	}
	TTYPE_ASSIGN,  TTYPE_COMMA,	TTYPE_POWER,			//	=	,	.
	
	//NESAHAT - VAZBA NA PRECEDENCNI TABULKU!
	TTYPE_VARIABLE,									// $blabla	
	TTYPE_L_BRACKET, TTYPE_R_BRACKET,				// (	)
	TTYPE_ADDITION, TTYPE_SUBTRACTION,				//	+	-
	TTYPE_MULTIPLICATION, TTYPE_DIVISION,			//	*	/
	TTYPE_DOT,
	TTYPE_EQUAL, TTYPE_NOT_EQUAL,					//	==	!==
	TTYPE_LESSER, TTYPE_LESSER_EQUAL,				// < <=
	TTYPE_GREATER,	TTYPE_GREATER_EQUAL,			// > >=
	TTYPE_SEMICOLON,								//	;	
	
	TTYPE_STRING, 										//	"blabla"
	TTYPE_FUNCTION,										//	blabla
	
	TTYPE_NUMBER,										// 15
	TTYPE_DEC_NUMBER,									// 15.45
// 	TTYPE_EXP_NUMBER,									// 15.45e-4
	TTYPE_PHP_START,									// <?php
	
	TTYPE_KEYWORD,
	TTYPE_RESERVED,
	TTYPE_TRUE,
	TTYPE_FALSE,
	TTYPE_NULL,
	
	TTYPE_EOF,
	TTYPE_EXPRESSION,
	TTYPE_ERROR = 66,
	TTYPE_SYNTAX = 77
	
} enum_RetVal;

typedef enum enum_State
{
	STATE_START,
	
	STATE_LESSER,
	STATE_GREATER,
	STATE_ASSIGN,
	STATE_EQUAL,
	STATE_NOT_EQUAL,
	
	STATE_SLASH,
	STATE_VARIABLE,
	STATE_STRING,
	STATE_TEXT,
	STATE_NUMBER,
	STATE_NUMBER_DOT,
	STATE_NUMBER_EXP,
	
	STATE_LINE_COMMENT,
	STATE_BLOCK_COMMENT,
	
	STATE_PHP_START
	
} enum_State;

typedef struct struct_Token
{
	int start;
	STRING data;
} struct_Token;

extern struct_Token glob_Token;

void init_Token();
void clear_Token();
void free_Token();

enum_RetVal get_Token();
//error wrapper
enum_RetVal getToken();
enum_RetVal getNextToken();
enum_RetVal get_Next_Token();


const char* debugRetval(enum_RetVal retval);

#endif
