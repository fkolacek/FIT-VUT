/*
 * XHERMA25 - Lukas Hermann		10	%
 * XSTODU06 - Dan Stodulka		60	%
 * XTRANH00 - Tran Manh Hoang		20	%
 * XMARCE02 - Andrea Marcelyova	10	%
 */
#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#include <sys/types.h>

#define STRING_INC 8

/**
 * @file	string.h
 * @brief	String.
 * hula hop
 * @date	21/11/2012
 * @author	Daniel Stodulka, xstodu06@stud.fit.vutbr.cz
 */

//+++++++++++++++++++++ STRUCT +++++++++++++++++++++|
typedef struct s_string
{
    char* input;
    int32_t length;
    int32_t size;
}s_string;
//---------------- STRUCT ----------------|

#include "global.h"

/**
 * @info Return length of string.
 * @param char* - string->input
 * @return int32_t - size of string
 */
int32_t String_length(char*);

/**
 * @info Insert string into struct, allocate required memory
 * @param char* - string to insert
 * @return s_string - return struct s_string
 */
s_string String_insert(char*);

/**
 * @info Make struct s_string from stdin, and allocate required memory
 * @return s_string - return struct s_string
 */
s_string Input(); // Zpracovani funkce input ze stdin cte a vrati jiz alokovanou strukturu

/**
 * @info Convert s_string to double.
 * @param s_string - s_string struct
 * @return double - return num from string
 */
double MyNumeric(s_string); // Prevod stringu na cislo

/**
 * @info concatenate two strings to one.
 * @param s_string - first
 * @param s_string - second
 * @return s_string - return concatenated string
 */
s_string String_concatenate(s_string, s_string);

/**
 * @info initialize
 * @param s_string - s_string to initialize.
 * @return int32_t - return 0 when succeeded
 */
int32_t String_init(s_string);

/**
 * @info compare two strings
 * @param s_string - first
 * @param s_string - second
 * @return int32_t - return 1 if first = second
 */
int32_t String_equal(s_string, s_string);

#endif // STRING_H_INCLUDED