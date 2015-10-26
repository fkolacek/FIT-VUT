/*
 * XHERMA25 - Lukas Hermann		60	%
 * XSTODU06 - Dan Stodulka		10	%
 * XTRANH00 - Tran Manh Hoang		20	%
 * XMARCE02 - Andrea Marcelyova	10	%
 */
#ifndef MEMORY_H_INCLUDED
#define MEMORY_H_INCLUDED

#include "global.h"

/**
 * @file	memory.h
 * @brief	Memory management.
 * This serves purely as interface. All "private" constants and
 * methods are only included in "memory.c". This way you ensure
 * that things are only accessible where they need to be acessible
 * and it also reduces namespace useage
 *
 * @date	21/11/2012
 * @author	Lukas Hermann, xherma25@stud.fit.vutbr.cz
 */



/**
 * @info Initialize g_memtab and fill it with NULL.
 * @return void
 */
void mem_init();

/**
 * @info Allocate requested memory, and add record in g_memtab.
 * @param size_t - size of requested memory space
 * @param int32_t - id of module requesting memory space
 * @return void* - pointer to allocated memory
 */
void* mem_alloc(size_t size, int32_t id);

/**
 * @info Realloc memory to new size.
 * @param void* - pointer to memory we want to realloc
 * @param size_t - size of requested reallocated memory 
 * space (must be bigger than previous size!)
 * @return void* - pointer to allocated memory
 */
void* mem_realloc(void* ptr, size_t size);

/**
 * @info Free allocated memory.
 * @param void* - pointer to memory we want to free
 * @return int32_t - return 0 when succeeded
 */
int32_t mem_free(void* ptr);

#endif //MEMORY_H_INCLUDED
