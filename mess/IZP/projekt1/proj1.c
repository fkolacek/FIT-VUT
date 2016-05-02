/**
 * @file	proj1.c
 * @brief	Prevod casovych udaju (projekt c. 1, predmet IZP)
 *
 *		Program pro prevod casovych udaju (sekund) nacitanych ze
 *		standartniho vstupu, ktery na zaklade parametru, predavanych
 *		programu z prikazove radky, prevadi udaje na tydny, dny,
 *		hodiny, minuty a sekundy.
 * @date	2011/10/15
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 * 
 * Todo: 	getchar vraci int
 * 			texts globalne
 */

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

/*! Tyden v sekundach */
#define WEEK 	604800
/*! Den v sekundach */
#define DAY 	86400
/*! Hodina v sekundach */
#define HOUR 	3600
/*! Minuta v sekundach */
#define MINUTE 	60

/*! Vyctovy typ pro parametry */
enum paramFlags{
	PARAM_SECOND,		/*!< sekundy -s */
	PARAM_MINUTE,		/*!< minuty -m */
	PARAM_HOUR,		/*!< hodiny -h */
	PARAM_DAY,		/*!< dny -d */
	PARAM_WEEK,		/*!< tydny -t */

	PARAM_HELP,		/*!< napoveda --help */
	PARAM_EXTRA,		/*!< neomezeny vstup --extra */
	PARAM_INVALID		/*!< neznamy parametr */
};

/*! Vyctovy typ pro vstup */
enum inputFlags{
	INPUT_VALID,		/*!< platny vstup */
	INPUT_EOF,		/*!< konec vstupu */
	INPUT_EMPTY,		/*!< prazdny vstup */

	INPUT_OVERFLOW,		/*!< hodnota by pretekla */
	INPUT_INVALID		/*!< neznamy, nebo neplatny vstup */
};

/**
 * @info Struktura reprezentujici prevedeny pocet sekund
 */
typedef struct datetime{
	unsigned long weeks;	/*!< pocet tydnu */
	unsigned long days;	/*!< pocet dni */
	unsigned long hours;	/*!< pocet hodin */
	unsigned long minutes;	/*!< pocet minut */
	unsigned long seconds;	/*!< pocet sekund */
} DATETIME;

/**
 * @info porovnani dvou retezcu (poli charu)
 * @param char* - prvni pole charu
 * @param char* - druhe pole charu
 * @return true pri shode, jinak false
 */

int strCompare(char*, char*);


/**
 * @info Nacteni a rozpoznani zadanych parametru
 * @param int - pocet parametru
 * @param char** - pole poli parametru
 * @return prvek z vyctoveho typu paramFlags (PARAM_INVALID pri neznamem parametru)
 */

int paramLoad(int, char**);

/**
 * @info Nacteni a konverze char do unsigned long (pokud je mozne)
 * @param int* - chybovy flag (prvek vyctoveho typu inputFlags)
 * @return nactena hodnota ze vstupu typu unsigned long
 */

unsigned long inputLoad(int*);

/**
 * @info Na zaklade paramFlags naplni a vrati strukturu DATETIME
 * @param int - prvek z vyctoveho typu paramFlags
 * @param unsigned long - pocet sekund zadanych na vstupu
 * @return naplnenou strukturu DATETIME
 */

DATETIME getDateTime(int, unsigned long);

/**
 * @info Vypise informace o programu a napovedu k programu pri prepinaci --help
 */

void printHelp();

/**
 * @info Vypise vysklonovanou strukturu DATETIME
 * @param DATETIME - naplnena struktura pro vypis
 */

void printDateTime(DATETIME);

/**
 * @info Hlavni funkce programu, stara se o nacitani parametru, vstupu a osetrovani chyb
 * @param int - pocet predanych parametru
 * @param char** - pole poli parametru
 * @return 0 (EXIT_SUCCESS) pri uspechu, 1 (EXIT_FAILURE) pri chybe
 */

int main(int argc, char **argv){
	int flag;

	//Nacteme parametr od uzivatele
	int mode = paramLoad(argc, argv);

	if(mode == PARAM_INVALID)
		fprintf(stderr, "Neznamy parametr, pouzijte prepinac --help pro napovedu\n");

	else if(mode == PARAM_HELP)
		printHelp();

	//Pokud byl zadan jiny (avsak znamy) parametr, zkusime nacist cislo
	else{
		unsigned long result;
		DATETIME dateTime;

		do{
			flag = INPUT_VALID;

			result = inputLoad(&flag);

			//Zpracovani nacteneho vysledku dle chyboveho flagu
			switch(flag){
				case INPUT_INVALID:
					fprintf(stderr, "Vstup musi byt cislo!\n");
					break;
				case INPUT_OVERFLOW:
					fprintf(stderr, "Prekrocena povolena velikost cisla (maximum je: %lu)!\n", ULONG_MAX);
					break;
				case INPUT_VALID:
					dateTime = getDateTime(mode, result);
					printDateTime(dateTime);
					break;
				case INPUT_EMPTY:
				case INPUT_EOF:
				default:
					break;
			}

		//Opakovat vstup pouze pokud je zadan --extra a vse probehne bez chyb
		}while(mode == PARAM_EXTRA && flag == INPUT_VALID);
	}

	//Pokud nastala chyba, vracime 1 (obdoba EXIT_FAILURE), jinak 0 (EXIT_SUCCESS)
	return (mode == PARAM_INVALID || flag >= INPUT_OVERFLOW)? 1 : 0;
}

int strCompare(char* first, char* second){
	for( ; *first == *second; ++first, ++second)
		if(*first == 0)
			return true;

	return false;
}

int paramLoad(int argc, char** argv){
	int mode;

	//Pokud nebyl zadan parametr nastavime tydny
	if(argc == 1)
		mode = PARAM_WEEK;
	//Pokud byl zadan parametr
	else if(argc == 2){
		if(strCompare(argv[1], "--help")) mode = PARAM_HELP;
		else if(strCompare(argv[1], "--extra")) mode = PARAM_EXTRA;
		else if(strCompare(argv[1], "-s")) mode = PARAM_SECOND;
		else if(strCompare(argv[1], "-m")) mode = PARAM_MINUTE;
		else if(strCompare(argv[1], "-h")) mode = PARAM_HOUR;
		else if(strCompare(argv[1], "-d")) mode = PARAM_DAY;
		else if(strCompare(argv[1], "-t")) mode = PARAM_WEEK;
		else mode = PARAM_INVALID;
	}
	//Bylo zadano vic parametru
	else
		mode = PARAM_INVALID;

	return mode;
}

unsigned long inputLoad(int* flag){
	char c;
	bool firstInput = true;
	unsigned long result = 0;

	while((c = getchar())){
		//Pokud jsou vstupem pouze cisla
		if(c >= '0' && c <= '9'){
			firstInput = false;

			//Pokud vstup zacina sekvenci 0
			if(result == 0 && c == '0') continue;

			c -= 48;

			//Postupna kontrola preteceni
			if(ULONG_MAX / 10 < result){
				*(flag) = INPUT_OVERFLOW;
				break;
			}

			if(ULONG_MAX - c < (result * 10)){
				*(flag) = INPUT_OVERFLOW;
				break;
			}

			//Posnuneme rad a pricteme posledni cifru cisla
			result = result * 10 + c;
		}
		else{
			//Pokud byl ukoncen nekonecne dlouhy vstup
			if(c == EOF) *(flag) = INPUT_EOF;

			//Pokud byl zadan prazdny vstup - brano jako chyba pri --extra
			else if(c == '\n' && firstInput == true) *(flag) = INPUT_EMPTY;

			//Pokud byl zadan jakykoliv jiny nepovoleny vstup
			else if(c != '\n') *(flag) = INPUT_INVALID;

			break;
		}
	}

	return result;
}

DATETIME getDateTime(int mode, unsigned long result){
	DATETIME dateTime = {
		.weeks		= 0,
		.days		= 0,
		.hours		= 0,
		.minutes	= 0,
		.seconds	= 0
	};

	//Pokud je nejvetsi zobrazitelna jednotka tyden
	if(result >= WEEK && (mode >= PARAM_WEEK || mode >= PARAM_EXTRA) ){
		dateTime.weeks = result / WEEK;
		result %= WEEK;
	}

	// nej. zobraz. jednotka den
	if(result >= DAY && mode >= PARAM_DAY){
		dateTime.days = result / DAY;
		result %= DAY;
	}

	// nej. zobraz. jednotka hodina
	if(result >= HOUR && mode >= PARAM_HOUR){
		dateTime.hours = result / HOUR;
		result %= HOUR;
	}

	// nej. zobraz. jednotka minuta
	if(result >= MINUTE && mode >= PARAM_MINUTE){
		dateTime.minutes = result / MINUTE;
		result %= MINUTE;
	}

	dateTime.seconds = result;

	return dateTime;
}

void printHelp(){
		const char message[] =
		"Program: Prevod casovych udaju\n"
		"Autor: Frantisek Kolacek (xkolac12@stud.fit.vutbr.cz)\n\n"
		"Pouziti: proj1 [moznosti]\n"
		" -t      - nejvyssi zobrazena jednotka bude tyden (standartne)\n"
		" -d      - nejvyssi zobrazena jednotka bude den\n"
		" -h      - nejvyssi zobrazena jednotka bude hodina\n"
		" -m      - nejvyssi zobrazena jednotka bude minuta\n"
		" -s      - nejvyssi zobrazena jednotka bude sekunda\n"
		" --extra - neomezene dlouhy vstup\n"
		" --help  - vypise napovedu\n";

	printf("%s\n", message);
}

void printDateTime(DATETIME dateTime){
	//Prevedeme strukturu DATETIME na pole (kvuli snazsi iteraci v cyklu)
	unsigned long dates[5] =
		{
			dateTime.weeks,
			dateTime.days,
			dateTime.hours,
			dateTime.minutes,
			dateTime.seconds
		};

	//Pole poli retezcu - textovych vyjadreni jednotek
	const char* texts[5][3] =
		{
			{ "tyden", "tydny", "tydnu" },
			{ "den", "dny", "dnu" },
			{ "hodina", "hodiny", "hodin" },
			{ "minuta", "minuty", "minut" },
			{ "sekunda", "sekundy", "sekund" }
		};

	int i, j;
	bool first = true;

	//Projdeme vsechny casove udaje
	for(i = 0; i < 5; i++){
		//Pokud neni hodnota nulova - krom sekund
		if(dates[i] != 0 || i == 4){
			//Sklonovani jednotek
			if(dates[i] == 1) j = 0;
			else if(dates[i] >= 2 && dates[i] <= 4) j = 1;
			else j = 2;

			//Pred prvni jednotku mezeru davat nebudeme
			if(!first) printf(" ");

			printf("%lu %s", dates[i], texts[i][j]);

			first = false;
		}
	}
	printf("\n");
}

/* konec proj1.c */
