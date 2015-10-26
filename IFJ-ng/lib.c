/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   lib.c
 *
 * Popis:
 *
 *
 * Datum:    20.11.2013
 *
 * Autori:   Frantisek Kolacek	<xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel 	<xstodu06@stud.fit.vutbr.cz>
 *           Hermann Lukas 		<xherma25@stud.fit.vutbr.cz>
 *           Tran Manh Hoang 	<xtranh00@stud.fit.vutbr.cz>
 */

#include "lib.h"

ITEMPTR boolval(ITEM item)
{
	ITEMPTR new;
	new = gcMalloc(sizeof(struct ITEM));
	new->type = TYPE_BOOL;
	if(item.type == TYPE_NULL)
	{
		new->value.valBool = false;
	}
	else if(item.type == TYPE_BOOL)
	{
		new->value.valBool = item.value.valBool;
	}
	else if(item.type == TYPE_DIGIT_INT)
	{
		if(item.value.valInt == 0)
			new->value.valBool = false;
		else
			new->value.valBool = true;
	}
	else if(item.type == TYPE_DIGIT_DOUBLE)
	{
		if(item.value.valDouble == 0.0)
			new->value.valBool = false;
		else
			new->value.valBool = true;
	}
	else if(item.type == TYPE_STRING)
	{
		if(strCompare(item.value.valString,""))
			new->value.valBool = false;
		else
			new->value.valBool = true;
	}
	else
	{
		new->type = TYPE_OTHER;
	}
	return new;
}

ITEMPTR intval(ITEM item)
{
	ITEMPTR new;
	new = gcMalloc(sizeof(struct ITEM));
	new->type = TYPE_DIGIT_INT;
	if(item.type == TYPE_NULL)
	{
		new->value.valInt = 0;
	}
	else if(item.type == TYPE_BOOL)
	{
		if(item.value.valBool == true)
			new->value.valInt = 1;
		else
			new->value.valInt = 0;
	}
	else if(item.type == TYPE_DIGIT_INT)
	{
		new->value.valInt = item.value.valInt;
	}
	else if(item.type == TYPE_DIGIT_DOUBLE)
	{
		new->value.valInt = (int)item.value.valDouble;
	}
	else if(item.type == TYPE_STRING)
	{
		int i;
		int start = 0;
		int size = my_strlen(item.value.valString);
		
		for(i = 0; i < size; i++ )
		{
			if(item.value.valString.data[i] != '\n' && item.value.valString.data[i] != '\r' && item.value.valString.data[i] != '\t' && item.value.valString.data[i] != ' ')
			{
				start = i;
				break;
			}
		}
		
		STRING number;
		strInit(&number);
		
		ERROR err;
		number = get_substring(item.value.valString, start, size, &err);
		
		if(!isdigit(number.data[0]))
		{
			new->value.valInt = 0;
		}
		else
		{
			int cislo;
			sscanf(number.data, "%d", &cislo);
			
			new->value.valInt = cislo;
		}
	}
	else
	{
		new->type = TYPE_OTHER;
	}
	return new;
}

ITEMPTR doubleval(ITEM item)
{
	ITEMPTR new;
	new = gcMalloc(sizeof(struct ITEM));
	new->type = TYPE_DIGIT_DOUBLE;
	if(item.type == TYPE_NULL)
	{
		new->value.valDouble = 0.0;
	}
	else if(item.type == TYPE_BOOL)
	{
		if(item.value.valBool == true)
			new->value.valDouble = 1.0;
		else
			new->value.valDouble = 0.0;
	}
	else if(item.type == TYPE_DIGIT_INT)
	{
		new->value.valDouble = (double)item.value.valInt;
	}
	else if(item.type == TYPE_DIGIT_DOUBLE)
	{
		new->value.valDouble = item.value.valDouble;
	}
	else if(item.type == TYPE_STRING)
	{
		int i;
		int start = 0;
		int size = my_strlen(item.value.valString);
		
		for(i = 0; i < size; i++ )
		{
			if(item.value.valString.data[i] != '\n' && item.value.valString.data[i] != '\r' && item.value.valString.data[i] != '\t' && item.value.valString.data[i] != ' ')
			{
				start = i;
				break;
			}
		}
		
		STRING number;
		strInit(&number);
		
		ERROR err;
		number = get_substring(item.value.valString, start, size, &err);
		
		for(unsigned int i = 0;(i<number.used);i++)
		{
			if(!isdigit(number.data[i]))
			{
				if(number.data[i] == 'e' || number.data[i] == 'E' || number.data[i] == '.')
				{
					if(i+1 <= number.used)
					{
						if(!isdigit(number.data[i+1]))
							return NULL;
					}
				}
				else
				{
					break;
				}
			}
		}
		
		if(!isdigit(number.data[0]))
		{
			if(number.data[0] == '-' && isdigit(number.data[1]))
			{
				double cislo;
				sscanf(number.data, "%lf", &cislo);
				
				new->value.valDouble = cislo;
			}
			else
			{
				new->type = TYPE_DIGIT_INT;
				new->value.valInt = 0;
			}
		}
		else
		{
			double cislo;
			int ret = sscanf(number.data, "%lf", &cislo);
			if(ret <= 0)
				return NULL;
			
			printf("ret :%d, %lf",ret,cislo);
			new->value.valDouble = cislo;
		}
	}
	else
	{
		new->type = TYPE_OTHER;
	}
	return new;
}

ITEMPTR strval(ITEM item)
{
	ITEMPTR new;
	new = gcMalloc(sizeof(struct ITEM));
	new->type = TYPE_STRING;
	
	if(item.type == TYPE_NULL)
	{
		strInitRaw(&new->value.valString,"");
	}
	else if(item.type == TYPE_BOOL)
	{
		if(item.value.valBool == true)
			strInitRaw(&new->value.valString,"1");
		else
			strInitRaw(&new->value.valString,"");
	}
	else if(item.type == TYPE_DIGIT_INT)
	{
		char* new_string = gcMalloc(sizeof(char*));
		sprintf(new_string, "%d", item.value.valInt);
		strInitRaw(&new->value.valString, new_string);
	}
	else if(item.type == TYPE_DIGIT_DOUBLE)
	{
		char* new_string = gcMalloc(sizeof(char*));
		sprintf(new_string, "%f", item.value.valDouble);
		strInitRaw(&new->value.valString, new_string);
	}
	else if(item.type == TYPE_STRING)
	{
		strInit(&new->value.valString);
		strCopy(&new->value.valString, &item.value.valString);
	}
	else
	{
		new->type = TYPE_OTHER;
	}
	return new;
}

//funkcni - OK
STRING get_string()
{
	STRING new;

	strInit(&new);
	char znak;
	while((znak = getchar()) != EOF)
	{
		if(znak == '\n')
			break;
		strAppend(&new, znak);
    }
	
	return new;
}


void put_string(char* string)
{
    printf("%s", string);
}

//funkcni - OK
int my_strlen(STRING text)
{
	return strUsed(&text);
}

//funkcni - OK
STRING get_substring(STRING text, int a, int b, ERROR* ERR)
{
	STRING new;
	strInit(&new);
	
	*ERR=0;
	if(a < 0 || b < 0 || a > b || my_strlen(text) <= a || my_strlen(text) < b)
	{
		*ERR=13;
		return new;
	}

	int i;	
	for(i=a;i<b;i++)
		strAppend(&new,text.data[i]);
	return new;
}




// ----------- knuth moris pratt algorithm -----------
int find_string(STRING T, STRING P)
{
	// empty patern or string
	if(P.data[0] == '\0' || T.data[0] == '\0')
		return 0;
	
	int tmp = -1;
	int T_pos = 0;
	int* P_index = gcMalloc(sizeof(P.size));
	P_index[0] = tmp;

	for(unsigned int i=1;i<strlen(P.data);i++)
	{
		while((tmp > -1) && (P.data[tmp+1] != P.data[i]))
			tmp = P_index[tmp];
		if(P.data[tmp+1] == P.data[i])
			tmp++;
		P_index[i] = tmp;
	}

	tmp = -1;
	for(T_pos = 0; T_pos < (int)strlen(T.data); T_pos++)
	{
		while(tmp > -1 && P.data[tmp+1] != T.data[T_pos])
			tmp = P_index[tmp];
		if(P.data[tmp+1] == T.data[T_pos])
            tmp++;
		if(tmp == (int)strlen(P.data) - 1)
		{
            gcFree(P_index);
            return T_pos - tmp;
		}
	}
	gcFree(P_index);
	return -1;
}

void quicksort(STRING text, int left, int right)
{
	int i,j;
	int pm,tmp;
	i = left;
	j = right;
	
	pm = text.data[(i+j)/2];
	do
	{
		while(text.data[i] < pm)
			i = i + 1;
		while(text.data[j] > pm)
			j = j - 1;
		if(i <= j)
		{
			tmp = text.data[i];
			text.data[i] = text.data[j];
			text.data[j] = tmp;
			i = i + 1;
			j = j - 1;
		}
	}while(i < j);
	
	if(left < j)
		quicksort(text, left, j);
	if(i < right)
		quicksort(text, i, right);
}

void sort_string(STRING text)
{
	int left = 0;
	if(text.data != NULL)
	{
		int right = strlen(text.data)-1;
		if(right > 0)
			quicksort(text, left, right);
	}
}

