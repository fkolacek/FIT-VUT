/*
 * XHERMA25 - Lukas Hermann		25	%
 * XSTODU06 - Dan Stodulka		25	%
 * XTRANH00 - Tran Manh Hoang		25	%
 * XMARCE02 - Andrea Marcelyova	25	%
 */
#ifndef __GLOBAL_LOOPINCLUDEPROTECTION
#define __GLOBAL_LOOPINCLUDEPROTECTION

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <strings.h>
#include <ctype.h>

#include "string.h"
#include "memory.h"

#define TRUE 1
#define FALSE 0

#define DATA_NULL 0
#define DATA_STRING 1
#define DATA_NUM 2
#define DATA_BOOL 3
#define DATA_FUNC 3

#define MAX_STACK_SIZE 65536

typedef enum
{
    EGt,             // > case table
    ELe,             // < case table
    EEq,             // = case table
    ERr,             // error case table
    EFunc,           // end of func
    EEqual,          // ==
    EGreater,        // >
    ELesser,         // <
    ENequal,         // !=
    EGreaterEq,      // >=
    ELesserEq,       // <=
    ESimpleLeft,     // (
    ESimpleRight,    // )
    EPlus,           // +
    EMinus,          // -
    EDivide,         // /
    EMul,            // *
    EConc,           // ** - concatenate
    EId,             // id
    ETermString,
    ETermNumb,
    EStart

}ELogic;

typedef union union_var
{
	double num;
	s_string string;
}union_var;

typedef struct t_var
{
	int32_t type;
	union_var data;
}t_var;

typedef enum IdType
{
    PVar,
    PFunc
}Type;

typedef enum Qtype
{
    QNum,
    QString,
    QOp
}QType;

typedef union QVar
{
    int32_t TKonsIndex;
    ELogic opt;
    char* TSymbolKey;
}QVar;

typedef struct QVarEle
{
    QVar var;
    QType type;
}QVarEle;

//++++++++++++++++++++++++ ENUMS ++++++++++++++++++++++++|
enum modul_id
{
  id_string,
  id_memory,
  id_ial,
  id_main,
  id_lexi,
  id_list,
  id_instr_list,
  id_parser,
  id_interpret,
  id_tkonst,
};

enum err_nums
{
  ERR_NOT_ENOUGHT_MEMORY,
  ERR_MEM_INTERNALERROR,
  err_2,
  err_3
};
//------------------- ENUMS ------------------|



#endif //__GLOBAL_LOOPINCLUDEPROTECTION

