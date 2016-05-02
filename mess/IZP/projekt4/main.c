/**
 * @file	proj4.c
 * @brief   Ceske razeni (projekt c. 4, predmet IZP)
 *
 *
 *
 * @date	2011/12/12
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <string.h>
#include "proj4listUtils.h"

enum paramFlags{
    PARAM_VALID = 0,
    PARAM_HELP,

    PARAM_INVALID,
    PARAM_INCOMPLETE,
};

enum IOFlags{
    IO_CANNOT_LOAD_DATA = 10,
    IO_CANNOT_SAVE_DATA,
    IO_CHARSET
};

typedef struct param{
    int flag;

    char* charset;

    char* inputFile;
    char* outputFile;

} PARAM;

const char* ERROR_MESSAGES[] = {
    [PARAM_INVALID]         = "Nebyly zadany platne parametry!",
    [PARAM_INCOMPLETE]      = "Nebyly zadany parametry ve spravnem formatu!",
    [IO_CANNOT_LOAD_DATA]   = "Nepodarilo se nacist vstupni data!",
    [IO_CANNOT_SAVE_DATA]   = "Nepodarilo se ulozit vysledna data!",
    [IO_CHARSET]            = "Nepodarilo se nastavit charset!"
};

/*!
 * @info Vraci naplnenou strukturu PARAM, podle ktere se ridi cely program
 * @param int - pocet parametru
 * @param char*[] - pole parametru
 * @return PARAM - naplnena struktura PARAM
 */

PARAM loadParam(int, char*[]);

/*!
 * @info Funkce pro vypsani chyboveho hlaseni
 * @param int - prvek vyctoveho typu paramFlags
 */

void printErrorMessage(int);

/**
 * @info Funkce pro vypsani napovedy
 */

void printHelp();

/*!
 * @info Hlavni funkce programu
 * @param int - pocet parametru
 * @param char*[] - pole parametru
 * @return int - vraci EXIT_FAILURE pri chybe, jinak EXIT_SUCCESS
 */

int main(int argc, char* argv[]){

    PARAM args = loadParam(argc, argv);

    if(args.flag == PARAM_HELP)
        printHelp();
    else if(args.flag != PARAM_VALID)
        printErrorMessage(args.flag);
    else{
        //Nastaveni lokalizace
        if(!setlocale(LC_CTYPE, args.charset)){
            printErrorMessage(IO_CHARSET);
            return EXIT_FAILURE;
        }

        //Naplneni seznamu
        LIST* set = loadFile(args.inputFile);

        //Pokud se nepodarilo seznam naplnit
        if(!set){
            printErrorMessage(IO_CANNOT_LOAD_DATA);
            return EXIT_FAILURE;
        }

        //Pokud se nepodarilo ulozit data
        if(!writeFile(args.outputFile, set)){
            printErrorMessage(IO_CANNOT_SAVE_DATA);
            clearList(set);
            return EXIT_FAILURE;
        }

        //debugList(set);

        //Uvolneni seznamu z pameti
        clearList(set);
    }

    return (args.flag > PARAM_HELP)? EXIT_FAILURE : EXIT_SUCCESS;
}

PARAM loadParam(int argc, char* argv[]){
    PARAM args = {
        .flag = PARAM_VALID,
        .charset = NULL,
        .inputFile = NULL,
        .outputFile = NULL
    };

    //-h
    if(argc == 2){
        if(strcmp("-h", argv[1]) == 0)
            args.flag = PARAM_HELP;
        else
            //args.flag = PARAM_INVALID;
            args.flag = PARAM_HELP;
    }

    //<inputFile> <outputFile>
    else if(argc == 3){
        args.charset = setlocale(LC_CTYPE, "");
        args.inputFile = argv[1];
        args.outputFile = argv[2];
    }

    //--charset <charset> <inputFile> <outputFile>
    else if(argc == 5){
        if(strcmp("--loc", argv[1]) == 0){
            args.charset = argv[2];
            args.inputFile = argv[3];
            args.outputFile = argv[4];
        }
        else
            //args.flag = PARAM_INVALID;
            args.flag = PARAM_HELP;
    }
    else
        //args.flag = PARAM_INVALID;
        args.flag = PARAM_HELP;

    return args;
}

void printErrorMessage(int errorMessageCode){
    fprintf(stderr, "%s\n", ERROR_MESSAGES[errorMessageCode]);
}

void printHelp(){
    setlocale(LC_CTYPE, setlocale(LC_CTYPE, ""));

    const char* HELP_MESSAGE =
        "Program: Ceske razeni\n"
		"Autor: Frantisek Kolacek (xkolac12@stud.fit.vutbr.cz)\n\n"
		"Pouziti: proj4 [moznosti] <inputFile> <outputFile>\n"
		" -h                                                    - vypise napovedu\n"
		" --loc <locale> <inputFile> <outputFile>               - umozni nastavit lokalizaci\n";


    printf("%s\n", HELP_MESSAGE);
}
