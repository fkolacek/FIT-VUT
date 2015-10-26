/**
 * @file	extstring.c
 * @brief	Projekt IPK 1 - Webovy klient
 *
 * @date	2013/02/07
 * @author	Frantisek Kolacek, xkolac12@stud.fit.vutbr.cz
 */

 #include "extstring.h"

int extStrPos(char* haystack, char needle, unsigned int startPos){
    if(!haystack)
        return -1;

    char* p = haystack;
    int pos = 0;

    while(p && *p != '\0'){
        if((unsigned int)pos >= startPos)
            if((*p) == needle)
                break;

        pos++;
        p++;
    }

    return ((unsigned int)pos == strlen(haystack))? -1 : pos;
}

char* extSubStr(char* haystack, unsigned int startPos, unsigned int length){
    //printf("extSub: %s [%d, %d]\n", haystack, startPos, length);
    if(!haystack)
        return NULL;

    if(length == 0)
        length = strlen(haystack) - startPos;

    haystack += startPos;

    char* str = malloc(sizeof(char) * (length + 1));
    strncpy(str, haystack, length);
    str[length] = '\0';

    //printf(" - %s\n", str);
    return str;
}
