/**
 * @file	proj3matrixUtils.h
 * @brief   Maticove vypocty (projekt c. 3, predmet IZP)
 *
 *          Knihovna obsahujici strukturu: MATRIX, FIELD a funkce
 *          pro praci s ni: createMatrix, clearMatrix, loadMatrix, printMatrix
 *
 * @date	2011/11/25
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#ifndef PROJ3MATRIXUTILS_H_INCLUDED
#define PROJ3MATRIXUTILS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

/** */
enum inputFlags{
    INPUT_INVALID = 0,
    INPUT_VALID = 1,
};

/** */
typedef struct matrix{
    int rowCount;
    int columnCount;

    int** data;
} MATRIX;

enum fieldDirections{
    DIR_NORTH_EAST = 0,
    DIR_EAST,
    DIR_SOUTH_EAST,

    DIR_SOUTH_WEST,
    DIR_WEST,
    DIR_NORTH_WEST = 5,


    DIR_INVALID
};

typedef struct field{
    int row;
    int column;
} FIELD;

/**
 * @info Vytvori v pameti naalokovanou a spravne nastavenou matici
 * @param int - pocet radku nove matice
 * @param int - pocet sloupcu nove matice
 * @return MATRIX* - ukazatel na alokovanou matici nebo NULL pri neuspechu
 */

MATRIX* createMatrix(int, int);

/**
 *  @info Uvolni z pameti celou strukturu MATRIX
 *  @param MATRIX* - struktura, ktera se ma uvolnit z pameti
 */

void clearMatrix(MATRIX*);

/**
 *  @info Vrati alokovanou a spravne naplnenou matici
 *  @param char* - retezec obsahujici nazev souboru
 *  @return MATRIX* - ukazatel na naplnenou matici nebo NULL pri neuspechu
 */

MATRIX* loadMatrix(char*);

/**
 *  @info Vypise obsah matice
 *  @param MATRIX* - matice pro vypsani
 */

void printMatrix(MATRIX*);

#endif // PROJ3MATRIXUTILS_H_INCLUDED
