/**
 * @file	proj3.c
 * @brief   Prace s maticemi (projekt c. 3, predmet IZP)
 *
 *
 *
 * @date	2011/11/29
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include "proj3matrix.h"


/*! Vyctovy typ pro parametry */
enum paramFlags{
    PARAM_TEST,
    PARAM_ADD,
    PARAM_MULT,
    PARAM_TRANS,
    PARAM_EXPR,
    PARAM_WATER,
    PARAM_CAROM,

    PARAM_HELP,

    PARAM_VALID,
    PARAM_INVALID,
    PARAM_INVALID_CONVERSION,
    PARAM_OVERFLOW,
    PARAM_INCOMPLETE,
    PARAM_CANNOT_LOAD
};

/*! Struktura pro praci a nacitani parametru */
typedef struct param{
    int flag;               /*!< flag pro behu rizeni programu */

    char* inputFilePath;    /*!< jmeno a cesta k souboru */
    char* inputFilePath2;   /*!< jmeno a cesta k souboru */

    int row;                /*!< cislo radku */
    int column;             /*!< cislo sloupce */
    int direction;          /*!< smer uderu */
    int power;              /*!< sila uderu */
} PARAM;

/*! Chybova hlaseni */
const char* errorMessages[] = {
    [PARAM_INCOMPLETE]          = "Nebyly zadany vsechny parametry!",
    [PARAM_INVALID]             = "Byl zadan neplatny parametr!",
    [PARAM_INVALID_CONVERSION]  = "Byl zadan neplatny format parametru!",
    [PARAM_OVERFLOW]            = "Byl zadan parametr mimo povoleny rozsah!",
    [PARAM_CANNOT_LOAD]         = "Nepodarilo se nacist vstupni data!"
};

/*!
 * @info Vraci naplnenou strukturu PARAM, podle ktere se ridi cely program
 * @param int - pocet parametru
 * @param char*[] - pole parametru
 * @return PARAM - naplnena struktura PARAM
 */

PARAM loadParam(int, char* []);

/*!
 * @info Funkce pro prevod retezce znaku na int
 * @param char* - retezec znaku
 * @param int* - chybovy flag
 * @return int - prevedene cislo typu int
 */

int loadParamParseInt(char*, int*);

/*!
 * @info Funkce pro vypsani chyboveho hlaseni
 * @param int - prvek vyctoveho typu paramFlags
 */

void printError(int);

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

    //Nacteni parametru
    PARAM args = loadParam(argc, argv);

    MATRIX* first = NULL;
    MATRIX* second = NULL;
    MATRIX* result = NULL;

    //Pokud byl zavolan parametr -h
    if(args.flag == PARAM_HELP)
        printHelp();

    //Pokud parsovani parametru skoncilo s chybou
    else if(args.flag >= PARAM_INVALID)
        printError(args.flag);

    //Pokud vse probehlo v poradku
    else{
        switch(args.flag){
            //--test <filename>
            case PARAM_TEST:
                result = loadMatrix(args.inputFilePath);

                if(!result){
                    printError(PARAM_CANNOT_LOAD);
                    break;
                }

                printMatrix(result);
                break;
            //--add <filename> <filename>
            case PARAM_ADD:
                first = loadMatrix(args.inputFilePath);
                second = loadMatrix(args.inputFilePath2);

                if(!first || !second){
                    printError(PARAM_CANNOT_LOAD);
                    break;
                }
                result = addMatrix(first, second);

                printMatrix(result);
                break;
            //--mult <filename> <filename>
            case PARAM_MULT:
                first = loadMatrix(args.inputFilePath);
                second = loadMatrix(args.inputFilePath2);

                if(!first || !second){
                    printError(PARAM_CANNOT_LOAD);
                    break;
                }

                result = mulMatrix(first, second);

                printMatrix(result);
                break;
            //--trans <filename>
            case PARAM_TRANS:
                first = loadMatrix(args.inputFilePath);

                if(!first){
                    printError(PARAM_CANNOT_LOAD);
                    break;
                }

                result = transMatrix(first);

                printMatrix(result);
                break;
            //--expr <filename> <filename>
            case PARAM_EXPR:
                first = loadMatrix(args.inputFilePath);
                second = loadMatrix(args.inputFilePath2);

                if(!first || !second){
                    printError(PARAM_CANNOT_LOAD);
                    break;
                }

                result = exprMatrix(first, second);

                printMatrix(result);
                break;
            //--water <row> <column> <filename>
            case PARAM_WATER:
                first = loadMatrix(args.inputFilePath);

                if(!first){
                    printError(PARAM_CANNOT_LOAD);
                    break;
                }

                result = waterMatrix(first, args.row, args.column);

                printMatrix(result);
                break;
            //--carom <row> <column> <dir> <power> <filename>
            case PARAM_CAROM:
                first = loadMatrix(args.inputFilePath);

                if(!first){
                    printError(PARAM_CANNOT_LOAD);
                    break;
                }

                result = caromMatrix(first, args.row, args.column, args.direction, args.power);

                for(int i = 0; i < result->columnCount; i++)
                    printf("%d ", result->data[0][i]);

                printf("\n");



                break;
            default:
                break;
        }

    }

    //Uvolneni vsech zdroju
    clearMatrix(first);
    clearMatrix(second);
    clearMatrix(result);

    first = second = result = NULL;

    return (args.flag > PARAM_VALID)? EXIT_FAILURE : EXIT_SUCCESS;
}

PARAM loadParam(int argc, char* argv[]){
    PARAM args = {
        .flag = PARAM_VALID,

        .inputFilePath = NULL,
        .inputFilePath2 = NULL,

        .row = 0,
        .column = 0,
        .direction = DIR_INVALID,
        .power = 0
    };

    if(argc >= 2){
        //-h
        if(strcmp("-h", argv[1]) == 0)
            args.flag = PARAM_HELP;

        //--test <filename>
        else if(strcmp("--test", argv[1]) == 0){
            args.flag = PARAM_TEST;

            if(argc == 3){
                args.inputFilePath = argv[2];
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }

        //--add <filename> <filename>
        else if(strcmp("--add", argv[1]) == 0){
            args.flag = PARAM_ADD;

            if(argc == 4){
                args.inputFilePath = argv[2];
                args.inputFilePath2 = argv[3];
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }

        //--mult <filename> <filename>
        else if(strcmp("--mult", argv[1]) == 0){
            args.flag = PARAM_MULT;

            if(argc == 4){
                args.inputFilePath = argv[2];
                args.inputFilePath2 = argv[3];
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }

        //--trans <filename>
        else if(strcmp("--trans", argv[1]) == 0){
            args.flag = PARAM_TRANS;

            if(argc == 3){
                args.inputFilePath = argv[2];
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }

        //--expr <filename> <filename2>
        else if(strcmp("--expr", argv[1]) == 0){
            args.flag = PARAM_EXPR;

            if(argc == 4){
                args.inputFilePath = argv[2];
                args.inputFilePath2 = argv[3];
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }

        //--water <row> <column> <filename>
        else if(strcmp("--water", argv[1]) == 0){
            args.flag = PARAM_WATER;

            if(argc == 5){
                args.row = loadParamParseInt(argv[2], &args.flag);

                if(args.flag != PARAM_WATER)
                    return args;

                args.column = loadParamParseInt(argv[3], &args.flag);

                if(args.flag != PARAM_WATER)
                    return args;

                args.inputFilePath = argv[4];
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }

        //--carom <row> <column> <direction> <power> <filename>
        else if(strcmp("--carom", argv[1]) == 0){
            args.flag = PARAM_CAROM;

            if(argc == 7){
                args.row = loadParamParseInt(argv[2], &args.flag);

                if(args.flag != PARAM_CAROM)
                    return args;

                args.direction = getDirection(argv[4]);

                if(args.direction == DIR_INVALID){
                    args.flag = PARAM_INVALID;
                    return args;
                }


                args.column = loadParamParseInt(argv[3], &args.flag);

                if(args.flag != PARAM_CAROM)
                    return args;

                args.power = loadParamParseInt(argv[5], &args.flag);

                if(args.flag != PARAM_CAROM)
                    return args;


                args.inputFilePath = argv[6];

            }
            else
                args.flag = PARAM_INCOMPLETE;
        }

        else
            args.flag = PARAM_INVALID;
    }
    else
        args.flag = PARAM_INVALID;

    return args;
}

int loadParamParseInt(char* argv, int* flag){
    char* endP = NULL;

    //Globalni errno
    errno = 0;

    unsigned int result = strtol(argv, &endP, 10);

    //Pokud konverze neprobehne
    if(endP == argv || *endP != '\0')
        *flag = PARAM_INVALID_CONVERSION;

    //Pokud by cislo pri konverzi preteklo
    else if(errno == ERANGE)
        *flag = PARAM_OVERFLOW;

	//Pokud se nastavila nejaka chyba, vratime false
    return result;
}

void printError(int errorMessageCode){
    fprintf(stderr, "%s\n", errorMessages[errorMessageCode]);
}

void printHelp(){
    const char message[] =
		"Program:\n"
		"Autor: Frantisek Kolacek (xkolac12@stud.fit.vutbr.cz)\n\n"
		"Pouziti: proj3 [moznosti]\n"
		" -h                                                    - vypise napovedu\n"
		" --test <filename>                                     - zkontroluje format souboru\n"
		" --add <filename> <filename>                           - secte dve matice\n"
		" --mult <filename> <filename>                          - vynasobi dve matice\n"
		" --trans <filename>                                    - vypocita transpozici matice\n"
		" --expr <filename> <filename>                          - vypocita dany vyraz\n"
		" --water <row> <column> <filename>                     - oznaci hladinu vodniho toku\n"
		" --carom <row> <column> <direction> <power> <filename> - oznaci drahu koule po kulecniku\n";


	printf("%s\n", message);
}
