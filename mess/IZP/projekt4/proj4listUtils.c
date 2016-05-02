#include "proj4listUtils.h"

#define CH_WEIGHT 20


enum strCompareNonAsciiFlags{
    STR_INVALID = -10,
    STR_EQUAL,
    STR_FIRST,
    STR_SECOND,
};

const int nonAsciiPriority[] = {
    [L'\0'] = 0,
    [L' '] = 1,

    [L'\u00c1'] = 10, [L'\u00e1'] = 10, [L'a'] = 10,    //a
    [L'b'] = 11,
    [L'c'] = 12,
    [L'\u010c'] = 13, [L'\u010d'] = 13,

    [L'\u010e'] = 14, [L'\u010f'] = 14, [L'd'] = 14,    //d
    [L'\u00c9'] = 15, [L'\u00e9'] = 15, [L'\u0114'] = 15, [L'\u0115'] = 15, [L'e'] = 15,    //e
    [L'f'] = 16,
    [L'g'] = 17,
    [L'h'] = 18,
    //Ch 20
    [L'\u00cd'] = 21, [L'\u00ed'] = 21, [L'i'] = 21,    //i
    [L'j'] = 22,
    [L'k'] = 23,
    [L'l'] = 24,
    [L'm'] = 25,
    [L'\u0147'] = 26, [L'\u0148'] = 26, [L'n'] = 26,    //n
    [L'\u00d3'] = 27, [L'\u00f3'] = 27, [L'o'] = 27,    //o
    [L'p'] = 28,
    [L'q'] = 29,
    [L'r'] = 30,
    [L'\u0158'] = 31, [L'\u0159'] = 31,
    [L's'] = 32,
    [L'\u0160'] = 33, [L'\u0161'] = 33,
    [L'\u0164'] = 34, [L'\u0165'] = 34, [L't'] = 34,    //t
    [L'\u00da'] = 35, [L'\u00fa'] = 35, [L'\u016e'] = 35, [L'\u016f'] = 35, [L'u'] = 35,    //u
    [L'v'] = 36,
    [L'w'] = 37,
    [L'x'] = 38,
    [L'\u00dd'] = 39, [L'\u00fd'] = 39, [L'y'] = 39,    //y
    [L'z'] = 41,
    [L'\u017d'] = 42, [L'\u017e'] = 42,

    [L'0'] = 100, [L'1'] = 101, [L'2'] = 102, [L'3'] = 103,
    [L'4'] = 104, [L'5'] = 105, [L'6'] = 106, [L'7'] = 107,
    [L'8'] = 108, [L'9'] = 109,

    [L'.'] = 200, [L','] = 201, [L';'] = 202, [L'?'] = 203, [L'!'] = 204,
    [L':'] = 205, [L'"'] = 206, [L'-'] = 207, [L'|'] = 208, [L'/'] = 209,
    [L'\\'] = 210, [L'('] = 211, [L')'] = 212, /*[L'//'] = 213,*/ [L'['] = 214,
    [L']'] = 215, [L'<'] = 216, [L'>'] = 217, [L'{'] = 218, [L'}'] = 218,
    [L'&'] = 219, [L'£'] = 220, [L'§'] = 221, [L'%'] = 222, /*[L'%o'] = 223,*/
    [L'$'] = 224, [L'='] = 225, [L'+'] = 226, [L'×'] = 227, [L'*'] = 228,
    [L'#'] = 229, [L'~'] = 230, /*[L'~~'] = 231, [L'~='] = 231,*/

};

const int nonAsciiDependencyPriority[] = {
    [L'\0'] = 0,
    [L' '] = 1,

    [L'a'] = 5,
    [L'\u00c1'] = 6, [L'\u00e1'] = 6,     //a
    [L'b'] = 7,
    [L'c'] = 8,
    [L'\u010c'] = 9, [L'\u010d'] = 9,
    [L'd'] = 10,    //d
    [L'\u010e'] = 11, [L'\u010f'] = 11,
    [L'e'] = 12,
    [L'\u00c9'] = 13, [L'\u00e9'] = 13,
    [L'\u0114'] = 14, [L'\u0115'] = 14,     //e
    [L'f'] = 15,
    [L'g'] = 16,
    [L'h'] = 17,
    //Ch 20
    [L'i'] = 21,
    [L'\u00cd'] = 22, [L'\u00ed'] = 22,     //i
    [L'j'] = 23,
    [L'k'] = 24,
    [L'l'] = 25,
    [L'm'] = 26,
    [L'n'] = 27,
    [L'\u0147'] = 28, [L'\u0148'] = 28,     //n
    [L'o'] = 29,
    [L'\u00d3'] = 30, [L'\u00f3'] = 30,     //o
    [L'p'] = 31,
    [L'q'] = 32,
    [L'r'] = 33,
    [L'\u0158'] = 34, [L'\u0159'] = 34,
    [L's'] = 35,
    [L'\u0160'] = 36, [L'\u0161'] = 36,
    [L't'] = 37,
    [L'\u0164'] = 38, [L'\u0165'] = 38,     //t
    [L'u'] = 39,
    [L'\u00da'] = 40, [L'\u00fa'] = 40,
    [L'\u016e'] = 41, [L'\u016f'] = 41,    //u
    [L'v'] = 42,
    [L'w'] = 43,
    [L'x'] = 44,
    [L'y'] = 45,
    [L'\u00dd'] = 46, [L'\u00fd'] = 46,     //y
    [L'z'] = 47,
    [L'\u017d'] = 48, [L'\u017e'] = 48,

    [L'0'] = 100, [L'1'] = 101, [L'2'] = 102, [L'3'] = 103,
    [L'4'] = 104, [L'5'] = 105, [L'6'] = 106, [L'7'] = 107,
    [L'8'] = 108, [L'9'] = 109,

    [L'.'] = 200, [L','] = 201, [L';'] = 202, [L'?'] = 203, [L'!'] = 204,
    [L':'] = 205, [L'"'] = 206, [L'-'] = 207, [L'|'] = 208, [L'/'] = 209,
    [L'\\'] = 210, [L'('] = 211, [L')'] = 212, /*[L'//'] = 213,*/ [L'['] = 214,
    [L']'] = 215, [L'<'] = 216, [L'>'] = 217, [L'{'] = 218, [L'}'] = 218,
    [L'&'] = 219, [L'£'] = 220, [L'§'] = 221, [L'%'] = 222, /*[L'%o'] = 223,*/
    [L'$'] = 224, [L'='] = 225, [L'+'] = 226, [L'×'] = 227, [L'*'] = 228,
    [L'#'] = 229, [L'~'] = 230, /*[L'~~'] = 231, [L'~='] = 231,*/
};

NODE* createNode(wchar_t* value){
    NODE* item = malloc(sizeof(NODE));

    if(!item)
        return NULL;

    item->value = value;
    item->prev = NULL;
    item->next = NULL;

    return item;
}

LIST* createList(){
    LIST* set = malloc(sizeof(LIST));

    if(!set)
        return NULL;

    set->count = 0;
    set->first = NULL;
    set->last = NULL;

    return set;
}

void clearNode(NODE* item){
    if(!item)
        return;

    if(item->value)
        free(item->value);

    free(item);
}

void clearList(LIST* set){
    if(!set)
        return;

    NODE* iterator = NULL;

    while(set->first){
        iterator = set->first;

        set->first = iterator->next;

        clearNode(iterator);
    }

    free(set);
}

bool insertItem(LIST* set, wchar_t* value){
    if(!set)
        return false;

    NODE* item = createNode(value);

    if(!item)
        return false;

    //Pokud seznam neni prazdny
    if(set->first){
        item->prev = set->last;
        set->last->next = item;
    }

    if(!set->first)
        set->first = item;

    set->last = item;

    set->count++;

    return true;
}

bool removeItem(LIST* set, NODE* item){
    if(!set)
        return false;

    if(!item)
        return false;

    NODE* iterator = NULL;

    if(item == set->first && set->first == set->last){
        iterator = set->first;

        set->first = NULL;
        set->last = NULL;
    }
    else if(item == set->first){
        iterator = set->first;

        set->first = iterator->next;
        set->first->prev = NULL;
    }
    else if(item == set->last){
        iterator = set->last;

        set->last = iterator->prev;
        set->last->next = NULL;
    }
    else{
        iterator = set->first;
        while(iterator){
            //Nasli jsme prvek nekde uprostred
            if(item == iterator){
                iterator->prev->next = iterator->next->prev;
                clearNode(iterator);
                return true;
            }

            iterator = iterator->next;
        }

        //Nepodarilo se najit pozadovany prvek
        return false;
    }

    clearNode(iterator);

    set->count--;

    return true;
}

NODE* findItem(LIST* set, wchar_t* value){
    NODE* iterator = set->first;

    while(iterator){
        if(strCompare(value, iterator->value))
            return iterator;

        iterator = iterator->next;
    }

    return NULL;
}

bool swapItems(NODE* first, NODE* second){
    if(!first || !second)
        return false;

    wchar_t* iterator = first->value;
    first->value = second->value;
    second->value = iterator;

    return true;
}

LIST* loadFile(char* filename){

    FILE* inputFile = fopen(filename, "r");

    if(!inputFile)
        return NULL;

    LIST* set = createList();

    if(!set){
        fclose(inputFile);
        return NULL;
    }

    wchar_t* line = NULL;

    int readed = 0;
    while((readed = readLine(&line, inputFile)) != 0){
        if(readed == STR_INVALID)
            continue;

        if(!insertItem(set, line)){
            free(line);
            clearList(set);
            fclose(inputFile);
            return NULL;
        }

        insertSort(set);

        line = NULL;
    }

    if(line)
        free(line);

    fclose(inputFile);

    return set;
}

int readLine(wchar_t** line, FILE* handler){
    if(*line != NULL || !handler)
        return 0;

    const unsigned int RESIZE_INCREMENT = 8;

    unsigned int size = 0;
    wchar_t* newLine = NULL;
    wint_t prevCharacter = '\0';
    wint_t character = '\0';

    size = RESIZE_INCREMENT;
    *line = malloc(sizeof(wchar_t) * size);

    if(!*line)
        return 0;

    unsigned int counter = 0;
    while((character = fgetwc(handler)) != WEOF && character != L'\n'){
        if(errno == EILSEQ){
            free(*line);
            *line = NULL;
            return STR_INVALID;
        }

        if(character == L' ' && prevCharacter == L' ')
            continue;

        if(counter == size - 1){
            size += RESIZE_INCREMENT;

            newLine = realloc(*line, sizeof(wchar_t) * size);
            if(!newLine){
                free(*line);
                *line = NULL;
                return 0;
            }


            *line = newLine;
            newLine = NULL;
        }

        prevCharacter = character;
        (*line)[counter] = character;
        counter++;
    }

    if(counter == 0 && character == L'\n'){
        free(*line);
        *line = NULL;
        return STR_INVALID;
    }

    (*line)[counter] = L'\0';

    newLine = realloc(*line, sizeof(wchar_t) * (counter + 1));

    if(!newLine){
        free(*line);
        *line = NULL;
        return 0;
    }


    *line = newLine;
    newLine = NULL;

    return counter;
}

bool writeFile(char* filename, LIST* set){
    if(!set)
        return false;

    FILE* outputFile = fopen(filename, "w");

    if(!outputFile)
        return false;

    NODE* iterator = set->first;

    while(iterator){
        fputws(iterator->value, outputFile);
        fputws(L"\n", outputFile);
        iterator = iterator->next;
    }

    fclose(outputFile);

    return true;
}

int strCompare(wchar_t* first, wchar_t* second){
    if(!first || !second)
        return STR_INVALID;

    unsigned int lengthFirst = strLength(first);
    unsigned int lengthSecond = strLength(second);

    int result = strCompareTable(first, second, nonAsciiPriority);

    if(result == STR_EQUAL)
        if(lengthFirst == lengthSecond)
            return strCompareTable(first, second, nonAsciiDependencyPriority);
        else
            return (lengthFirst > lengthSecond)? STR_SECOND : STR_FIRST;
    else
        return result;
}

int strCompareTable(wchar_t* first, wchar_t* second, const int table[]){
    if(!first || !second)
        return STR_INVALID;

    wchar_t firstLetter, secondLetter;
    int weightFirst, weightSecond;
    for( ; *first != L'\0' && *second != L'\0';first++, second++){
        firstLetter = *first;
        secondLetter = *second;

        //Prevedeme ASCII znaky na lowercase
        if(firstLetter >= L'A' && firstLetter <= L'Z') firstLetter += 32;
        if(secondLetter >= L'A' && secondLetter <= L'Z') secondLetter += 32;

        if(firstLetter > table[L'~'] || firstLetter > table[L'~'])
            return STR_INVALID;


        //Zjistime vahu znaku
        if(firstLetter == L'c' && (*(first + 1) == L'h' || *(first + 1) == L'H'))
            weightFirst = CH_WEIGHT;
        else
            weightFirst = table[(int)firstLetter];

        if(secondLetter == L'c' && (*(second + 1) == L'h' || *(second + 1) == L'H'))
            weightSecond = CH_WEIGHT;
        else
            weightSecond = table[(int)secondLetter];

        //Pokud nejsou vahy stejne
        if(weightFirst != weightSecond)
            return (weightFirst > weightSecond)? STR_SECOND : STR_FIRST;
    }

    return STR_EQUAL;
}

unsigned int strLength(wchar_t* value){
    unsigned int length = 0;

    while(*(value++) != L'\0')
        length++;

    return length;
}

void insertSort(LIST* set){
    if(!set || !set->last)
        return;

    NODE* iterator = set->last;
    while(iterator->prev && strCompare(iterator->value, iterator->prev->value) == STR_FIRST){
        swapItems(iterator, iterator->prev);
        iterator = iterator->prev;
    }
}

NODE* nodeAt(LIST* set, unsigned int pos){
    if(!set || !set->last)
        return NULL;

    if(pos > set->count)
        return NULL;

    NODE* iterator = set->first;
    for(unsigned int i = 0; i < pos; i++)
        iterator = iterator->next;

    return iterator;
}

void debugList(LIST* set){
    if(!set)
        return;

    NODE* iterator = set->first;

    printf("Count: %d\n", set->count);
    printf("FIRST: 0x%X, LAST: 0x%X\n", (unsigned int)set->first, (unsigned int)set->last);

    while(iterator){
        printf("NODE: 0x%X, NODE->prev: 0x%X, NODE->next: 0x%X\n", (unsigned int)iterator, (unsigned int)iterator->prev, (unsigned int)iterator->next);
        printf("VAL: %ls\n\n", iterator->value);

        iterator = iterator->next;
    }

    printf("\n");

}
