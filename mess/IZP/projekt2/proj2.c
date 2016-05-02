/**
 * @file	proj2.c
 * @brief   Iteracni vypocty (projekt c. 2, predmet IZP)
 *
 *          Program pomoci iteracnich algoritmu prubezne pocita arcsin a obecny logaritmus, delku lomene cary bez a s chybou
 *
 * @date	2011/11/11
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>
#include "proj2putils.h"
#include "proj2math.h"
#include "proj2lbl.h"

/*! Vyctovy typ pro vstup */
enum inputFlags{
    INPUT_VALID,        /*!< validni vstup */
    INPUT_INVALID,      /*!< chyba na vstupu */
    INPUT_EOF           /*!< konec vstupu */
};

/*! Vyctovy typ pro parametry */
enum paramFlags{
    PARAM_ARCSIN,				/*!< arcsin --arcsin sigdig */
	PARAM_LOGAX,				/*!< ln --logax sigdig a */
	PARAM_LBL,					/*!< lbl --lbl */
	PARAM_LBLE,					/*!< lble --lble */

	PARAM_HELP,					/*!< -h --help */
	PARAM_OVERFLOW,				/*! parametr kontrola preteceni */
	PARAM_INVALID,				/*! neznamy parametr */
	PARAM_INVALID_CONVERSION,	/*! pokud byl parametr zadan v neocekavanem formatu */
	PARAM_EMPTY,				/*! pokud nebyly zadany zadne parametry */
	PARAM_INCOMPLETE			/*! pokud nebyly zadany vsechny parametry */
};

/*! Chybova hlaseni */
const char* errorMessages[] = {
	[PARAM_OVERFLOW]			= "Byl zadan parametr mimo rozsah!",
	[PARAM_INVALID]				= "Byl zadan neznamy parametr!",
	[PARAM_INVALID_CONVERSION]	= "Byl zadan parametr v neznamem tvaru!",
	[PARAM_EMPTY]				= "Nebyl zadan zadny parametr!",
	[PARAM_INCOMPLETE]			= "Nebyly zadany vsechny parametry!",
	[INPUT_INVALID]             = "Zadan neplatny vstup!"
};

/*! Struktura pro praci a nacitani parametru */
typedef struct param{
    int flag;                   /*!< flag pro behu rizeni programu */

    unsigned int sigdig;        /*!< zadana presnost */
    double base;                /*!< zaklad matematickych funkci && chyba lomene cary */

    int (*inputLoad)(PAIR*);    /*!< ukazatel na funkci, ktera se pouzije pro nacitani hodnot */
} PARAM;

/*!
 * @info Provadi nacitani hodnoty typu double ze vstupu
 * @param PAIR* - ukazatel na strukturu PAIR, ktera se naplni
 * @return int - prvek vyctoveho typu inputFlags
 */

int inputLoadDouble(PAIR*);

/*!
 * @info Provadi nacitani paru hodnot typu double ze vstupu
 * @param PAIR* - ukazatel na strukturu PAIR, ktera se naplni
 * @return int - prvek vyctoveho typu inputFlags
 */

int inputLoadDoublePair(PAIR*);

/*!
 * @info Vraci naplnenou strukturu PARAM, podle ktere se ridi cely program
 * @param int - pocet parametru
 * @param char*[] - pole parametru
 * @return PARAM - naplnena struktura PARAM
 */

PARAM loadParam(int, char*[]);

/*!
 * @info Funkce pro prevod retezce znaku na unsigned int
 * @param PARAM* - struktura pro kontrolu chybovych stavu
 * @param char* - retezec znaku
 * @return unsigned int - prevedene cislo typu unsigned int
 */

unsigned int loadParamParseUint(PARAM*, char*);

/*!
 * @info Funkce pro prevod retezce znaku na double
 * @param PARAM* - struktura pro kontrolu chybovych stavu
 * @param char* - retezec znaku
 * @return unsigned int - prevedene cislo typu double
 */

double loadParamParseDouble(PARAM*, char*);

/*!
 * @info Funkce pro vypsani chyboveho hlaseni
 * @param int - prvek vyctovych typu paramFlags nebo inputFlags
 */

void printError(int);

/*!
 * @info Funkce pro vypsani napovedy
 */

void printHelp();

/*!
 * @info Hlavni funkce programu
 * @param int - pocet parametru
 * @param char*[] - pole parametru
 * @return int -
 */

int main(int argc, char* argv[]){
    //Nacteni parametru ze vstupu
    PARAM args = loadParam(argc, argv);

    int flag = INPUT_VALID;

    //Pokud byl parametr -h
    if(args.flag == PARAM_HELP)
        printHelp();

    //Pokud pri nacitani parametru nastala nejaka chyba, vypiseme ji
    else if(args.flag >= PARAM_OVERFLOW)
        printError(args.flag);

    //Pokud nacitani parametru probehlo bez problemu
    else{
        bool firstInput = true;

        PAIR tmp;

        //Deklarace struktury pro nacitani hodnot ze vstupu
        PAIR inputs = {
            .first = 0,
            .second = 0
        };

        //Deklarace struktury pro mezivysledky
        RESULTSET results = {
            .first = 0,
            .second = 0,
        };
        results.point.first = 0;
        results.point.second = 0;

        //Hlavni nacitaci cyklus
        while( (flag = args.inputLoad(&inputs)) != INPUT_EOF){
            //Pokud pri nacitani narazime na chybu
            if(flag == INPUT_INVALID){
                printError(INPUT_INVALID);
                break;
            }

            //Vyber funkce, pro vypocet na zaklade parametru
            switch(args.flag){
                //Vypocet arcsin
                case PARAM_ARCSIN:
                    results.first = arcsin(inputs.first, args.sigdig);
                    break;
                //Vypocet logax
                case PARAM_LOGAX:
                    results.first = logax(inputs.first, args.base,args.sigdig);
                    break;
                //Vypocet delky lomene cary
                case PARAM_LBL:
                    if(!firstInput){
                        results.first += lblPair(results.point, inputs);
                    }

                    results.point = inputs;

                    break;
                //Vypocet delky lomene cary s chybou
                case PARAM_LBLE:
                    if(!firstInput){
                        tmp = lblePair(results.point, inputs, args.base);
                        results.first += tmp.second;
                        results.second += tmp.first;
                    }

                    results.point = inputs;

                    printf("%.10e\n", results.second);

                    break;
                default:
                    break;
            }

            printf("%.10e\n", results.first);

            firstInput = false;
        }

    }

    return (args.flag >= PARAM_OVERFLOW || flag == INPUT_INVALID)? EXIT_FAILURE : EXIT_SUCCESS;
}

int inputLoadDouble(PAIR* inputs){
    int flag = scanf("%lf", &inputs->first);

    if(flag == EOF)
		return INPUT_EOF;

    else if(flag != 1){
		inputs->first = NAN;
		flag = scanf("%*s");

		return INPUT_INVALID;
	}

	else
		return INPUT_VALID;
}

int inputLoadDoublePair(PAIR* inputs){
	int flag = scanf("%lf %lf", &inputs->first, &inputs->second);

    if(flag == EOF)
		return INPUT_EOF;

	else if(flag != 2){
		inputs->first = inputs->second = NAN;
		flag = scanf("%*s");

		return INPUT_INVALID;
	}

	else
		return INPUT_VALID;
}

PARAM loadParam(int argc, char* argv[]){
    PARAM args;

    //Pokud nebyl zadan zadny parametr
    if(argc == 1 || argc == 0)
        args.flag = PARAM_EMPTY;
    //Pokud byl zadan parametr
    else{
        //-h
        if(strcmp(argv[1], "-h") == 0)
            args.flag = PARAM_HELP;

        //--arcsin <sigdig>
        else if(strcmp(argv[1], "--arcsin") == 0){
            if(argc == 3){
                args.flag = PARAM_ARCSIN;
                args.inputLoad = inputLoadDouble;

                args.sigdig = loadParamParseUint(&args, argv[2]);

                if(args.flag != PARAM_ARCSIN)
                    return args;
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }
        //--logax <sigdig> <a>
        else if(strcmp(argv[1], "--logax") == 0){
            if(argc == 4){
                args.flag = PARAM_LOGAX;
                args.inputLoad = inputLoadDouble;

                args.sigdig = loadParamParseUint(&args, argv[2]);

                if(args.flag != PARAM_LOGAX)
                    return args;

                args.base = loadParamParseDouble(&args, argv[3]);


                if(args.flag != PARAM_ARCSIN)
                    return args;
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }
        //--lbl
        else if(strcmp(argv[1], "--lbl") == 0){
            args.flag = PARAM_LBL;
            args.inputLoad = inputLoadDoublePair;
        }
        //--lble <err>
        else if(strcmp(argv[1], "--lble") == 0){
            if(argc == 3){
                args.flag = PARAM_LBLE;
                args.inputLoad = inputLoadDoublePair;

                args.base = loadParamParseDouble(&args, argv[2]);

                if(args.flag != PARAM_LBLE)
                    return args;
            }
            else
                args.flag = PARAM_INCOMPLETE;
        }

        else
            args.flag = PARAM_INVALID;
    }

    return args;
}

unsigned int loadParamParseUint(PARAM* args, char* argv){
    char* endP = NULL;

    //Globalni errno
    errno = 0;

    unsigned int result = strtoul(argv, &endP, 10);

    //Pokud konverze neprobehne
    if(endP == argv || *endP != '\0')
        args->flag = PARAM_INVALID_CONVERSION;

    //Pokud by cislo pri konverzi preteklo
    else if(errno == ERANGE)
        args->flag = PARAM_OVERFLOW;

	//Pokud se nastavila nejaka chyba, vratime false
    return result;
}

double loadParamParseDouble(PARAM* args, char* argv){
    char* endP = NULL;

    //Globalni errno
    errno = 0;

    double result = strtod(argv, &endP);

    //Pokud neprobehla konverze
	if(endP == argv || *endP != '\0')
		args->flag = PARAM_INVALID_CONVERSION;

	//Pokud cislo pri konverzi preteklo
	else if(errno == ERANGE)
		args->flag = PARAM_OVERFLOW;

	return result;
}


void printError(int errorMessagesCode){
    fprintf(stderr, "%s\n", errorMessages[errorMessagesCode]);
}

void printHelp(){
    const char message[] =
		"Program: Program pomoci iteracnich algoritmu prubezne pocita \n         arcsin a obecny logaritmus, delku lomene cary bez a s chybou\n\n"
		"Autor: Frantisek Kolacek (xkolac12@stud.fit.vutbr.cz)\n\n"
		"Pouziti: proj2 [moznosti]\n"
		" --arcsin <sigdig>     - vypocita arcsin o zadane presnosti\n"
		" --logax <sigdig> <a>  - vypocita obecny logaritmus o zadane prenosti a zakladu\n"
		" --lbl                 - vypocita delku lomene cary\n"
		" --lble <err>          - vypocita delku lomene cary s chybou\n"
		" -h                    - vypise napovedu\n";

	printf("%s\n", message);
}

/* konec proj2.c */
