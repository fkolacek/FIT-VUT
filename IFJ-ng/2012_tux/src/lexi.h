/*
 * XHERMA25 - Lukas Hermann		0	%
 * XSTODU06 - Dan Stodulka		0	%
 * XTRANH00 - Tran Manh Hoang		30	%
 * XMARCE02 - Andrea Marcelyova	70	%
 */
#ifndef __LEXI_LOOPINCLUDEPROTECTION
#define __LEXI_LOOPINCLUDEPROTECTION

#include "global.h"

#define MAXTOKENLENGHT 512
#define MY_FUNC 666
#define MY_KEY_WORD 667
#define MY_RESERVER_WORD 668

/**
 * @file	lexi.h
 * @brief	Lexical analyzator.
 * bla bla
 * @date	4/12/2012
 * @author	Lukas Hermann, xherma25@stud.fit.vutbr.cz
 */

//++++++++++++++++++++++++ ENUMS ++++++++++++++++++++++++|
enum TokenType
{
    T_Id = 1,
    T_Number,
    T_String,
    T_Reserved_Word,
    T_UndefinedState,
    T_PlusSign,
    T_MinusSign,
    T_Assign,
    T_LogicLesser,
    T_LogicLesserEqual,
    T_LogicGreater,
    T_LogicGreaterEqual,
    T_LogicEqual,
    T_LogicNotEqual,
    T_SimpleBracketLeft,
    T_SimpleBracketRight,
    T_SquareBracketLeft,
    T_SquareBracketRight,
    T_Colom,
    T_Function,
    T_EOF,
    T_Concatenate,
    T_Mul,
    T_Div,
    T_Comma,
    T_EndLine,
    T_End,
    T_False,
    T_True,
    T_If,
    T_Else,
    T_Nil,
    T_Return,
    T_While,
    T_Print,
    T_Input,
    T_Typeof,
    T_Len,
    T_Find,
    T_Sort,
    T_Numeric,
};
//------------------- ENUMS ------------------|


//++++++++++++++++++++++++ STRUCT +++++++++++++++++++++++|
typedef union DATATYPE    // Declare union type
{
	char*  string;
	double num;
} u_var;

struct __struct_token
{
	int32_t type;
	u_var data;
	int32_t line;
};

typedef struct __struct_token struct_token;
//------------------- STRUCT -----------------|

/**
 * @info Init Lexical analyzator, and set file to work with.
 * @param FILE* - Pointer to opened file.
 * @return void
 */
void Lexi_Init(FILE*);

/**
 * @info Free all allocated memmory by Lexical analyzator.
 * @return void
 */
void Lexi_Destroy();

/**
 * @info Return next token taken from file.
 * @return struct_token* - Pointer to token.
 */
struct_token* Get_Next_Token();

/**
 * @info Free token.
 * @param struct_token* - Pointer to be freed.
 * @return void
 */
void Free_Token(struct_token*);


#endif //__LEXI_LOOPINCLUDEPROTECTION
