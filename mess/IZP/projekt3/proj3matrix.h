/**
 * @file	proj3matrix.h
 * @brief   Maticove vypocty (projekt c. 3, predmet IZP)
 *
 *
 *
 *
 * @date	2011/11/25
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef PROJ3MATRIX_H_INCLUDED
#define PROJ3MATRIX_H_INCLUDED

#include <string.h>
#include <stdbool.h>
#include "proj3matrixUtils.h"

/**
 * @info Funkce scitajici dve matice
 * @param MATRIX* - ukazatel na prvni matici
 * @param MATRIX* - ukazatel na druhou matici
 * @return MATRIX* - ukazatel na vyslednou matici
 */

MATRIX* addMatrix(MATRIX*, MATRIX*);

/**
 * @info Funkce nasobici dve matice
 * @param MATRIX* - ukazatel na prvni matici
 * @param MATRIX* - ukazatel na druhou matici
 * @return MATRIX* - ukazatel na vyslednou matici
 */

MATRIX* mulMatrix(MATRIX*, MATRIX*);

/**
 * @info Funkce pro transponovani matice
 * @param MATRIX* - ukazatel na vstupni matici
 * @return MATRIX* - ukazatel na vyslednou matici
 */

MATRIX* transMatrix(MATRIX*);

/**
 * @info Funkce pro pocitani maticoveho vyrazu
 * @param MATRIX* - ukazatel na prvni matici
 * @param MATRIX* - ukazatel na druhou matici
 * @return MATRIX* - ukazatel na vyslednou matici
 */

MATRIX* exprMatrix(MATRIX*, MATRIX*);

/**
 * @info Funkce vracejici prvek fieldDirections odpovidajici smeru
 * @param char* - smer z prikazove radky
 * @return int - prvek z vyctoveho typu fieldDirections
 */

int getDirection(char*);

/**
 * @info Vraci naplnenou strukturu FIELD
 * @param int - cislo radku
 * @param int - cislo sloupce
 * @return FIELD - vytvorena struktura FIELD
 */

FIELD makeField(int, int);

/**
 * @info Funkce ziska souradnice souseda v danem smeru z aktualni pozice
 * @param FIELD - aktualni pozice
 * @param int - smer hledani
 * @return FIELD - pozice souseda
 */
FIELD getNeighbor(FIELD, int);

/**
 * @info Funkce pro zjisteni platnosti souradnice
 * @param MATRIX* - ukazatel na matici
 * @param FIELD - kontrolovane pozice
 * @return bool - true pokud je pozice platna, jinak false
 */

bool isValidPos(MATRIX*, FIELD);

/**
 * @info Rekurzi volana funkce pro zjisteni vodniho toku
 * @param MATRIX* - ukazatel na vstupni matici
 * @param FIELD - aktualni pozice, odkud porovnavat
 * @param int - predchozi minimum
 */

void changeWorld(MATRIX*, FIELD, int);

/**
 * @info Funkce pro zjisteni vodniho toku
 * @param MATRIX* - ukazatel na vstupni strukturu
 * @param int - cislo radku zdroje vody
 * @param int - cislo sloupce zdroje vody
 * @return MATRIX* - ukazatel na vyslednou matici
 */

MATRIX* waterMatrix(MATRIX*, int, int);

/**
 * @info Zjisti drahu po kulecnikovem stole
 * @param int - cislo radku
 * @param int - cislo sloupce
 * @param int - smer
 * @param unsigned int - sila
 * @return MATRIX* - ukazatel na vyslednou matici
 */

MATRIX* caromMatrix(MATRIX*, int, int, int, unsigned int);

#endif // PROJ3MATRIX_H_INCLUDED
