#ifndef PROJ4LISTUTILS_H_INCLUDED
#define PROJ4LISTUTILS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stdbool.h>
#include <wchar.h>
#include <errno.h>

/** Struktura reprezentujici prvek seznamu */
typedef struct node{
    wchar_t* value;

    struct node* prev;
    struct node* next;
} NODE;

/** Struktura reprezentujici seznam */
typedef struct list{
    unsigned int count;

    struct node* first;
    struct node* last;

} LIST;

/**
 * @info Vraci vytvorenou strukturu NODE
 * @param wchar_t* - retezec
 * @return NODE* - ukazatel na vytvorenou strukturu
 */

NODE* createNode(wchar_t*);

/**
 * @info Vraci vytvorenou strukturu LIST
 * @return LIST* - ukazatel na vytvorenou strukturu
 */

LIST* createList();


/**
 * @info Uvolni z pameti strukturu NODE
 * @param NODE* - ukazatel na strukturu
 */

void clearNode(NODE*);

/**
 * @info Uvolni z pameti strukturu LIST
 * @param NODE* - ukazatel na strukturu
 */

void clearList(LIST*);

/**
 * @info Pridat do seznamu prvek
 * @param LIST* - ukazatel na seznam
 * @param wchar_t* - retezec
 */

bool insertItem(LIST*, wchar_t*);

/**
 * @info Odebere ze seznamu prvek
 * @param LIST* - ukazatel na seznam
 * @param NODE* - ukazatel na prvek
 * @return bool - true pri uspechu
 */

bool removeItem(LIST*, NODE*);

/**
 * @info Vyhleda v seznamu prvek
 * @param LIST* - ukazatel na seznam
 * @param wchar_t* - retezec
 * @return bool - true pri uspechu
 */

NODE* findItem(LIST*, wchar_t*);

/**
 * @info Prohodi dva prvky v seznamu
 * @param NODE* - ukazatel na prvek
 * @param NODE* - ukazatel na prvek
 * @return bool - true pri uspechu
 */

bool swapItems(NODE*, NODE*);

/**
 * @info Níacte soubor do seznamu
 * @param char* - jmeno souboru
 * @return LIST* - ukazatel na naplneny seznam
 */

LIST* loadFile(char*);

/**
 * @info Precte radku ze souboru
 * @param wchar_t** - ukazatel na radku
 * @param FILE* - ukazatel na soubor
 * @return int - pocet znaku
 */

int readLine(wchar_t**, FILE*);

/**
 * @info Zapise seznam do souboru
 * @param char* - jmeno souboru
 * @param LIST* - ukazatel na list
 * @return bool - true pri uspechu
 */

bool writeFile(char*, LIST*);

/**
 * @info Porovna dva retezce
 * @param wchar_t* - retezec
 * @param wchar_t* - retezec
 * @return int - prvek z vyctoveho typu
 */

int strCompare(wchar_t*, wchar_t*);

int strCompareTable(wchar_t*, wchar_t*, const int[]);

/**
 * @info Zjisti delku retezece
 * @param wchar_t* - retezec
 * @return unsigned int - delka
 */

unsigned int strLength(wchar_t*);

/**
 * @info Seradi seznam
 * @param LIST* - ukazatel na seznam
 */

void insertSort(LIST*);

/**
 * @info Vrati ukazatel prvku na dane pozici
 * @param LIST* - ukazatel na seznam
 * @return unsigned int - pozice
 */

NODE* nodeAt(LIST*, unsigned int);

void debugList(LIST*);


#endif // PROJ4LISTUTILS_H_INCLUDED
