/**
 * @file	proj3matrixUtils.cpp
 * @brief   Maticove vypocty (projekt c. 3, predmet IZP)
 *
 *          Soubor obsahujici implementace funkci: createMatrix, clearMatrix, loadMatrix, printMatrix
 *
 * @date	2011/11/25
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include "proj3matrixUtils.h"

MATRIX* createMatrix(int rowCount, int columnCount){
    MATRIX* M = malloc(sizeof(MATRIX));

    //Pokud se nepodari alokovat pamet pro matici
    if(!M) return NULL;

    //Inicializace matice
    M->rowCount = rowCount;
    M->columnCount = columnCount;
    M->data = NULL;

    //Alokace radku matice
    M->data = (int**)malloc(sizeof(int*) * rowCount);

    if(!M->data){
        free(M);
        return NULL;
    }

    //Alokace sloupcu matice
    for(int i = 0; i < rowCount; i++){
        M->data[i] = (int*)malloc(sizeof(int) * columnCount);

        if(!M->data[i]){
            M->rowCount = i + 1;
            clearMatrix(M);
            return NULL;
        }
    }

    return M;
}

void clearMatrix(MATRIX* M){
    if(!M)
        return;

    for(int i = 0; i < M->rowCount; i++)
        free(M->data[i]);

    free(M->data);
    free(M);
}


MATRIX* loadMatrix(char* inputFile){
    MATRIX* result = NULL;
    FILE* fileHandler = NULL;

    int rowCount = 0, columnCount = 0;

    int flag = INPUT_VALID;
    int loadedValue = 0;

    //Pokusime se otevrit soubor pro cteni
    fileHandler = fopen(inputFile, "r");

    //Pokud se to nepodarilo vratime NULL
    if(!fileHandler)
        return NULL;

    //Pokud se nepodarilo nacist pocet radku a sloupcu matice
    if(fscanf(fileHandler, "%d %d\n", &rowCount, &columnCount) != 2){
        fclose(fileHandler);
        return NULL;
    }

    //Vytvoreni struktury o pozadovane velikosti
    result = createMatrix(rowCount, columnCount);

    //Pokud se nepodarilo z nejakeho duvodu matici alokovat
    if(!result){
        fclose(fileHandler);
        return NULL;
    }

    for(int i = 0; i < rowCount; i++){
        for(int j = 0; j < columnCount; j++){
            flag = fscanf(fileHandler, "%d", &loadedValue);

            if(flag == INPUT_INVALID || flag == EOF) break;

            result->data[i][j] = loadedValue;
        }
        //fscanf(fileHandler, "\n");

        if(flag == INPUT_INVALID || flag == EOF) break;
    }

    if(flag == INPUT_INVALID || flag == EOF){
        clearMatrix(result);
        result = NULL;
    }


    //Zavreme soubor
    fclose(fileHandler);

    //Vratime ukazatel na naplnenou strukturu
    return result;
}

void printMatrix(MATRIX* M){
    if(!M){
        printf("false\n");
        return;
    }

    printf("%d %d\n", M->rowCount, M->columnCount);

    for(int i = 0; i < M->rowCount; i++){
        for(int j = 0; j < M->columnCount; j++)
            printf("%d ", M->data[i][j]);

        printf("\n");
    }
}
