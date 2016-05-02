/**
 * @autor Lukáš Hermann
 * @date 11.12.2011
 *
 * @info Program slouzi k serazeni radku dle ceskeho razeni.
 * @param --loc = lokalizace
 * @param soubor1 = vstupni soubor
 * @param soubor2 = vystupni soubor
 *
 * @motd I used to write in C, then I took an arrow to the knee...
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <locale.h>
#include <wchar.h>
#include <assert.h>
#include <stdbool.h>
#include <errno.h>

enum enum_params
{
    help,
    NO_PARAM,
    BAD_PARAM,
    BAD_FILE,
    NO_FLAG
};

typedef struct
{
    char* locale;
    char* soubor_first;
    char* soubor_second;
    int flag;
}struct_params;

typedef struct prvek
{
    wchar_t* radek;
    struct prvek* dalsi;
    struct prvek* predchozi;
} struct_prvek;

const int CH = 250;

const int tabulka_ascii[] = {
    [L'\0'] = 0,
    [L' '] = 1,
    [L'Á'] = 10, [L'á'] = 10, [L'a'] = 10,
    [L'b'] = 11,
    [L'Č'] = 12, [L'č'] = 12, [L'c'] = 12,
    [L'Ď'] = 15, [L'ď'] = 15, [L'd'] = 15,
    [L'Ě'] = 16, [L'ě'] = 16, [L'É'] = 16, [L'é'] = 16, [L'e'] = 16,
    [L'f'] = 17,
    [L'g'] = 18,
    [L'h'] = 19,
    [L'Í'] = 21, [L'í'] = 21, [L'i'] = 21,
    [L'j'] = 22,
    [L'k'] = 23,
    [L'l'] = 24,
    [L'm'] = 25,
    [L'Ň'] = 26, [L'ň'] = 26, [L'n'] = 26,
    [L'Ó'] = 27, [L'ó'] = 27, [L'o'] = 27,
    [L'p'] = 28,
    [L'q'] = 29,
    [L'Ř'] = 31, [L'ř'] = 31,[L'r'] = 31,
    [L'Š'] = 33, [L'š'] = 33,[L's'] = 33,
    [L'Ť'] = 34, [L'ť'] = 34, [L't'] = 34,
    [L'Ú'] = 35, [L'ú'] = 35, [L'Ů'] = 35, [L'ů'] = 35, [L'u'] = 35,
    [L'v'] = 36,
    [L'w'] = 37,
    [L'x'] = 38,
    [L'Ý'] = 39, [L'ý'] = 39, [L'y'] = 39,
    [L'Ž'] = 42, [L'ž'] = 41, [L'z'] = 41,

    [L'0'] = 100, [L'1'] = 101, [L'2'] = 102, [L'3'] = 103,
    [L'4'] = 104, [L'5'] = 105, [L'6'] = 106, [L'7'] = 107,
    [L'8'] = 108, [L'9'] = 109,

    [L'.'] = 200, [L','] = 201, [L';'] = 202, [L'?'] = 203, [L'!'] = 204,
    [L':'] = 205, [L'"'] = 206, [L'-'] = 207, [L'|'] = 208, [L'/'] = 209,
    [L'\\'] = 210, [L'('] = 211, [L')'] = 212, [L'['] = 214,
    [L']'] = 215, [L'<'] = 216, [L'>'] = 217, [L'{'] = 218, [L'}'] = 218,
    [L'&'] = 219, [L'£'] = 220, [L'§'] = 221, [L'%'] = 222,
    [L'$'] = 224, [L'='] = 225, [L'+'] = 226, [L'×'] = 227, [L'*'] = 228,
    [L'#'] = 229, [L'~'] = 230,
    //CH = 250
};

const int tabulka_diakritika[] = {
    [L'\0'] = 0,
    [L' '] = 1,
    [L'a'] = 10,
    [L'Á'] = 11, [L'á'] = 11,
    [L'b'] = 12,
    [L'c'] = 13,
    [L'Č'] = 14, [L'č'] = 14,
    [L'd'] = 15,
    [L'Ď'] = 16, [L'ď'] = 16,
    [L'e'] = 17,
    [L'É'] = 18, [L'é'] = 18,
    [L'Ě'] = 19, [L'ě'] = 19,
    [L'f'] = 20,
    [L'g'] = 21,
    [L'h'] = 22,
    [L'i'] = 23,
    [L'Í'] = 24, [L'í'] = 24,
    [L'j'] = 25,
    [L'k'] = 26,
    [L'l'] = 27,
    [L'm'] = 28,
    [L'n'] = 29,
    [L'Ň'] = 30, [L'ň'] = 30,
    [L'o'] = 31,
    [L'Ó'] = 32, [L'ó'] = 32,
    [L'p'] = 33,
    [L'q'] = 34,
    [L'r'] = 35,
    [L'Ř'] = 36, [L'ř'] = 36,
    [L's'] = 37,
    [L'Š'] = 38, [L'š'] = 38,
    [L't'] = 39,
    [L'Ť'] = 40, [L'ť'] = 40,
    [L'u'] = 41,
    [L'Ú'] = 42, [L'ú'] = 42,
    [L'Ů'] = 43, [L'ů'] = 43,
    [L'v'] = 44,
    [L'w'] = 45,
    [L'x'] = 46,
    [L'y'] = 47,
    [L'Ý'] = 48, [L'ý'] = 48,
    [L'z'] = 49,
    [L'Ž'] = 50, [L'ž'] = 50,

    [L'0'] = 100, [L'1'] = 101, [L'2'] = 102, [L'3'] = 103,
    [L'4'] = 104, [L'5'] = 105, [L'6'] = 106, [L'7'] = 107,
    [L'8'] = 108, [L'9'] = 109,

    [L'.'] = 200, [L','] = 201, [L';'] = 202, [L'?'] = 203, [L'!'] = 204,
    [L':'] = 205, [L'"'] = 206, [L'-'] = 207, [L'|'] = 208, [L'/'] = 209,
    [L'\\'] = 210, [L'('] = 211, [L')'] = 212, [L'['] = 214,
    [L']'] = 215, [L'<'] = 216, [L'>'] = 217, [L'{'] = 218, [L'}'] = 218,
    [L'&'] = 219, [L'£'] = 220, [L'§'] = 221, [L'%'] = 222,
    [L'$'] = 224, [L'='] = 225, [L'+'] = 226, [L'×'] = 227, [L'*'] = 228,
    [L'#'] = 229, [L'~'] = 230,
    //CH = 250
};

enum vaha{
	Stejne,
	Prvni,
	Druhy
};

/**
 * @info Nacita zadane parametry a nastavuje chybovy flag.
 * @param int - pocet parametru.
 * @param char** - pole poli parametru.
 * @return struct_params - vraci strukturu parametru.*/
struct_params get_params(int, char**);

/**
 * @info Funkce nacte ze souboru radek, a podle promene length se v souboru posune nebo ne.
 * @param wchar_t** - odkaz na andresu do ktere se bude nacitat.
 * @param unsigned int* - odkaz na velikost bufferu.
 * @param FILE* - ukazatel na soubor ze ktereho se bude nacitat.
 * @param bool - promena podle ktere se rozhoduje o posunuti v souboru.
 * @return strukturu parametru - struct_params*/
unsigned int readLine(wchar_t **pLine, unsigned int *bufferLength, FILE *stream);

/**
 * @info Zapise seznam do souboru
 * @param struct_prvek - prvni prvek seznamu
 * @param char* - jmeno souboru*/
void write_struct(struct_prvek*, char*);

/**
 * @info Uvolni seznam z pameti.
 * @param struct_prvek* - ukazatel na prvni prvek seznamu.*/
void uvolni_seznam(struct_prvek*);

/**
 * @info Zjisti delku retezce.
 * @param wchar_t* - retezec
 * @return unsigned int - vraci delku retezce.*/
unsigned int delka_retezce(wchar_t* value);

/**
 * @info Porovnava retezce v predanych prvcich.
 * @param wchar_t* - prvni prvek.
 * @param wchar_t* - druhy prvek.
 * @return int - vraci cislo podle vyctoveho typu VAHA.*/
int porovnej(wchar_t* prvni, wchar_t* druhy);

/**
 * @info Rozhodne  ktery retezec je vetsi nebo menci
 * @param wchar_t* - prvni retezec.
 * @param wchar_t* - druhy retezec.
 * @return int - vraci cislo podle vyctoveho typu VAHA.*/
int rozhodni(wchar_t* prvni, wchar_t* druhy, int length,bool ascii);

/**
 * @info seradi seznam.
 * @param struct_prvek* - ukazatel na prvni prvek.*/
void insert_sort(struct_prvek*);

/**
 * @info vypise napovedu.*/
void write_help();


int main(int argc, char *argv[])

{
    //------------------------------------------------|
    //------------------ parametry -------------------|
    //------------------------------------------------|
    struct_params parametry = get_params(argc,argv);

    if(parametry.flag == NO_PARAM)
    {write_help();
        return 0;}
    if(parametry.flag == BAD_PARAM)
    {write_help();
        return 0;}
    if(parametry.flag == BAD_FILE)
    {write_help();
        return 0;}

    if(parametry.flag == help)
    {write_help();
        return 0;}


    //------------------------------------------------|
    //--------------- konec parametry ----------------|
    //------------------------------------------------|

    //------------------------------------------------|
    //----------------- lokalizace -------------------|
    //------------------------------------------------|

    if(parametry.locale)
    {
        if(!setlocale(LC_ALL, parametry.locale))
        {
            printf("zadana neplatna lokalizace");
            return 1;
        }
    }
    else
        setlocale(LC_ALL, "");

    //------------------------------------------------|
    //-------------- konec lokalizace ----------------|
    //------------------------------------------------|

    //------------------------------------------------|
    //------------------ nacitani --------------------|
    //------------------------------------------------|
    FILE* file1;
    file1 = fopen(parametry.soubor_first,"r");

    struct_prvek* aktualni = NULL, *predchozi = NULL, *prvni = NULL;
    int pocet = 0;
    wchar_t *radek = NULL;
    unsigned int bufflen = 0;
    predchozi = NULL;

	unsigned int length;

    while(!feof(file1))
    {
        radek = NULL;
        
        length = readLine(&radek, &bufflen, file1);

        aktualni = malloc(sizeof(struct_prvek));
        aktualni->radek = radek;
        aktualni->predchozi = predchozi;

        aktualni->dalsi = NULL;

        if(pocet != 0)
			predchozi->dalsi = aktualni;

        predchozi = aktualni;

        if(pocet == 0)
			prvni = aktualni;

        insert_sort(prvni);
        pocet++;
     }

    fclose(file1);

    //------------------------------------------------|
    //--------------- konec nacitani -----------------|
    //------------------------------------------------|


    //------------------------------------------------|
    //------------------- zapis ----------------------|
    //------------------------------------------------|
    aktualni = prvni;
    write_struct(aktualni,parametry.soubor_second);

    //------------------------------------------------|
    //------------------ uvolneni --------------------|
    //------------------------------------------------|
    //free(radek);
    aktualni = prvni;
    uvolni_seznam(aktualni);


    return 0;

}


void uvolni_seznam(struct_prvek* prvek)
{
    struct_prvek* pomocny;

    while(prvek->dalsi != NULL)
    {
        pomocny = prvek->dalsi;
        free(prvek->radek);
        free(prvek);
        prvek = pomocny;
    }
    free(prvek->radek);
    free(prvek);
}

void write_struct(struct_prvek* prvek, char* jmeno)
{
    FILE* file;
    file = fopen(jmeno,"w");
    do
    {
        fwprintf(file,L"%ls\n",prvek->radek);
        prvek = prvek->dalsi;
    }while(prvek->dalsi != NULL);
    fclose(file);
}

struct_params get_params(int argc, char *argv[])
{
    struct_params parametry;
    parametry.flag = NO_FLAG;
    parametry.soubor_first = NULL;
    parametry.soubor_second = NULL;
    parametry.locale = NULL;
    if(argc < 2)
    {
        parametry.flag = NO_PARAM;
        return parametry;
    }

    if(strcmp("-h", argv[1]) == 0 || strcmp("--help", argv[1]) == 0)
    {
        parametry.flag = help;
        return parametry;
    }

    int pomocna = 0;
    if(strcmp("--loc", argv[1]) == 0)
    {
        pomocna = 2;
        parametry.locale = argv[2];
    }
    else
        parametry.locale = NULL;

    if(argc != 3+pomocna)
    {
        parametry.flag = BAD_PARAM;
        return parametry;
    }

    parametry.soubor_first = NULL;
    parametry.soubor_second = NULL;
    parametry.soubor_first = argv[1+pomocna];
    parametry.soubor_second = argv[2+pomocna];

    FILE* file1;
    file1 = fopen(parametry.soubor_first,"r");
    if(file1 == NULL)
        parametry.flag = BAD_FILE;
    else
        fclose(file1);

    return parametry;
}

unsigned int readLine(wchar_t **pLine, unsigned int *bufferLength, FILE *stream)
{
    assert(pLine != NULL);
    assert(bufferLength != NULL);
    assert(stream != NULL);

    const int B_INCREMENT = 8;

    if (*pLine == NULL)
    {
        *bufferLength = B_INCREMENT;
        *pLine = malloc(*bufferLength*sizeof(wchar_t));
        if (pLine == NULL)
        {
            *bufferLength = 0;
            return 0;
        }
    }

    wint_t c;
    unsigned int i = 0;

    while ((c = getwc(stream)) != WEOF && c != '\n')
    {
        if(errno == EILSEQ)
        {
            *bufferLength = 1;
            return 1;
        }
        if (i == *bufferLength-1)
        {
            *bufferLength += B_INCREMENT;
            wchar_t *newBuffer = realloc(*pLine, *bufferLength*sizeof(wchar_t));
            if (newBuffer == NULL)
            {
                assert(*pLine != NULL);
                free(*pLine);
                *pLine = NULL;
                *bufferLength = 0;
                return 0;
            }
            else
            {
                *pLine = newBuffer;
            }
        }

        (*pLine)[i] = c;
        i += 1;
    }
    assert(*bufferLength >= i);

    (*pLine)[i] = L'\0';

    return i+1;
}

int porovnej(wchar_t* prvni, wchar_t* druhy)
{
    unsigned int delka_prvni = delka_retezce(prvni);
    unsigned int delka_druhy = delka_retezce(druhy);
    unsigned int length = (delka_prvni > delka_druhy)? delka_druhy : delka_prvni;

    wchar_t* prvni_zaloha = prvni;
    wchar_t* druhy_zaloha = druhy;

    for(unsigned int i=0;i<length;i++)
    {
        return rozhodni(prvni,druhy,length,false);
    }

    if(delka_prvni == delka_druhy)
    {
        prvni = prvni_zaloha;
        druhy = druhy_zaloha;

        return rozhodni(prvni,druhy,length,true);
    }
    else
        return (delka_prvni > delka_druhy)? Druhy : Prvni;
}

int rozhodni(wchar_t* prvni, wchar_t* druhy, int length,bool ascii)
{
    wchar_t prvni_znak;
    wchar_t druhy_znak;
    int prvni_vaha;
    int druhy_vaha;

    for(unsigned int i=0;i<(unsigned int)length;i++)
    {
        prvni_znak = *prvni;
        druhy_znak = *druhy;

        if(prvni_znak >= L'A' && prvni_znak <= L'Z')
            prvni_znak = prvni_znak + 32;
        if(druhy_znak >= L'A' && druhy_znak <= L'Z')
            druhy_znak = druhy_znak + 32;


        if(ascii==true)
        {
            if(prvni_znak == L'c' && (*(prvni+1) == L'h' || *(prvni+1) == L'H'))
                prvni_vaha = CH;
            else
                prvni_vaha = tabulka_ascii[(int)prvni_znak];

            if(prvni_znak == L'c' && (*(prvni+1) == L'h' || *(prvni+1) == L'H'))
                druhy_vaha = CH;
            else
                druhy_vaha = tabulka_ascii[(int)druhy_znak];
        }
        else
        {
            if(prvni_znak == L'c' && (*(prvni+1) == L'h' || *(prvni+1) == L'H'))
                prvni_vaha = CH;
            else
                prvni_vaha = tabulka_diakritika[(int)prvni_znak];

            if(prvni_znak == L'c' && (*(prvni+1) == L'h' || *(prvni+1) == L'H'))
                druhy_vaha = CH;
            else
                druhy_vaha = tabulka_diakritika[(int)druhy_znak];
        }

        if(prvni_vaha != druhy_vaha)
            return (prvni_vaha > druhy_vaha)? Druhy : Prvni;

        prvni++;
        druhy++;
    }
    return Stejne;
}

unsigned int delka_retezce(wchar_t* radek)
{
    unsigned int delka=0;

    while(*radek != L'\0')
    {
        delka++;
        radek++;
    }

    return delka;
}

void insert_sort(struct_prvek* prvni)
{
    if(!prvni)
        return;

    struct_prvek* iterator = prvni;
    wchar_t* pom = NULL;

    while(iterator->dalsi)
		iterator = iterator->dalsi;

    while(iterator->predchozi && porovnej(iterator->radek, iterator->predchozi->radek) == Prvni)
    {
        pom = iterator->radek;
        iterator->radek = iterator->predchozi->radek;
        iterator->predchozi->radek = pom;

        iterator = iterator->predchozi;
    }
}

void write_help()
{
    wprintf(L"Program slouzi k serazeni radku dle ceskeho razeni\n");
    wprintf(L"Vypracoval: Lukas Hermann\n");
    wprintf(L"Email: xherma25@stud.fit.vutbr.cz\n");
    wprintf(L"\n");
    wprintf(L"Parametry:\n");
    wprintf(L"\t--loc [lokalizace]\n");
    wprintf(L"\tvstupni soubor\n");
    wprintf(L"\tvystupni soubor\n");
}
