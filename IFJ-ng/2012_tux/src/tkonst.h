/*
 * XHERMA25 - Lukas Hermann		10	%
 * XSTODU06 - Dan Stodulka		0	%
 * XTRANH00 - Tran Manh Hoang		80	%
 * XMARCE02 - Andrea Marcelyova	10	%
 */
#ifndef __TKONST_LOOPINCLUDEPROTECTION
#define __TKONST_LOOPINCLUDEPROTECTION

#include "global.h"
#define TINITSIZE 512

typedef struct konst_list
{
	t_var* konst;
	int32_t count;
    int32_t allocated;
}konst_list;

konst_list* konst_list_Init();
void konst_list_Free(konst_list*);
int32_t konst_list_Insert(konst_list*,t_var);
int32_t konst_list_NextIndex(konst_list*);

#endif
