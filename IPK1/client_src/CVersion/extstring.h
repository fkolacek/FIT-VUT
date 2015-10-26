/**
 * @file	extstring.h.c
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/07
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef EXTSTRING_H_INCLUDED
#define EXTSTRING_H_INCLUDED

#include <stdlib.h>
#include <string.h>

int extStrPos(char*, char, unsigned int);

char* extSubStr(char*, unsigned int, unsigned int);

#endif // EXTSTRING_H_INCLUDED
