/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   ial.c
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

#include "ial.h"

void BT_Init(struct_BTree* tree){
	if(!tree)
		return;
	
	tree->root = NULL;
	tree->last = NULL;
}

void BT_Free(struct_BTree* tree){
	if(!tree)
		return;

	if(!tree->root)
		return;
	
	recursive_Node_Delete(tree->root);
	
	BT_Init(tree);
}

void recursive_Node_Delete(struct_BTree_Node node){
	if(!node)
		return;
	
	if(node->left != NULL)
		recursive_Node_Delete(node->left);
	if(node->right != NULL)
		recursive_Node_Delete(node->right);
	
	gcFree(node->data);
	gcFree(node);
}


struct_BTree_Node BT_Search(struct_BTree* tree, STRING_PTR key)
{
	return recursive_Node_Search(tree->root, key);
}

struct_BTree_Node recursive_Node_Search(struct_BTree_Node node, STRING_PTR key)
{
	if(node == NULL)
        return NULL;
	else if(strCompareStrings(*node->key,*key))
		return node;
	else if (strSize(node->key) > strSize(key))
		return recursive_Node_Search(node->left, key);
	else
		return recursive_Node_Search(node->right, key);
}


ERROR BT_Insert(struct_BTree* tree, STRING_PTR key, void* data)
{
	if (tree->root == NULL)
	{
	    tree->root = gcMalloc(sizeof(struct struct_BTree_Node));
		tree->root->key = key;
		tree->root->left = NULL;
		tree->root->right = NULL;
		tree->root->data = data;
		tree->last = tree->root;
		return E_OK;
	}

    struct_BTree_Node temp_node = tree->root;

	while(temp_node != NULL)
	{
        if ( strSize(key) < strSize(temp_node->key))
		{
            if(temp_node->left == NULL)
			{
				temp_node->left = gcMalloc(sizeof(struct struct_BTree_Node));
				temp_node->left->key = key;
				temp_node->left->left = NULL;
				temp_node->left->right = NULL;
				temp_node->left->data = data;
				tree->last = temp_node->left;
				return E_OK;
            }
            else temp_node=temp_node->left;
        }
        else if (strSize(key) >= strSize(temp_node->key))
		{
            if (temp_node->right == NULL)
			{
				temp_node->right = gcMalloc(sizeof(struct struct_BTree_Node));
				temp_node->right->key = key;
				temp_node->right->left = NULL;
				temp_node->right->right = NULL;
				temp_node->right->data = data;
				tree->last = temp_node->right;
				return E_OK;
            }
            else temp_node = temp_node->right;
        }
        else
		{
			tree->last = temp_node;
			return E_OK;
		}
    }
    return E_OK;
}

void stInit(SYMBOL_TABLE_PTR st){
	if(!st)
		return;
	
	BT_Init(&st->start.symbols);
	BT_Init(&st->functions);
	
	listInit(&st->start.staticValues);
	listInit(&st->start.instructions);
	
	st->start.calledCount = 0;
	st->start.argumentsCount = 0;
	st->start.argumentsCalled = 0;
	
	st->curr = &(st->start);
	
	stInitLibFunctions(st);
}

void stInitLibFunctions(SYMBOL_TABLE_PTR st){

	STRING id;	

	strInit(&id);
	
	strInitRaw(&id, "boolval");
	stInsertFunction(st, id);
		
	strInitRaw(&id, "doubleval");
	stInsertFunction(st, id);
		
	strInitRaw(&id, "intval");
	stInsertFunction(st, id);
	
	strInitRaw(&id, "strval");
	stInsertFunction(st, id);
	
	strInitRaw(&id, "get_string");
	stInsertFunction(st, id);
	
	strInitRaw(&id, "put_string");
	stInsertFunction(st, id);	
	
	strInitRaw(&id, "strlen");
	stInsertFunction(st, id);	

	strInitRaw(&id, "get_substring");
	stInsertFunction(st, id);	

	strInitRaw(&id, "find_string");
	stInsertFunction(st, id);	

	strInitRaw(&id, "sort_string");
	stInsertFunction(st, id);	
}

ERROR stInsertFunction(SYMBOL_TABLE_PTR st, STRING id){
	FUNCTION_PTR function = gcMalloc(sizeof(FUNCTION));
	
	BT_Init(&function->symbols);
	listInit(&function->staticValues);
	listInit(&function->instructions);
	
	function->calledCount = 0;
	function->argumentsCount = 0;
	function->argumentsCalled = 0;
	
	strCopy(&id, &function->id);
	
	ERROR err = BT_Insert(&st->functions, &function->id, function);
	
	if(err != E_OK){
		fprintf(stderr, "Are you Chuck? insert function\n");
		
	}
	
	return err;	
}

ERROR stInsertSymbol(FUNCTION_PTR function, STRING id){
	SYMBOL_PTR symbol = gcMalloc(sizeof(SYMBOL));
	
	strCopy(&id, &symbol->id);
	symbol->type = TYPE_OTHER;
	symbol->items = NULL;
	symbol->filgy = false;
	
	ERROR err = BT_Insert(&function->symbols, &symbol->id, symbol);
	
	if(err != E_OK){
		fprintf(stderr, "Are you Chuck? insert symbol\n");
		
	}
	
	return err;
}

FUNCTION_PTR stSearchFunction(SYMBOL_TABLE_PTR st, STRING id){
	struct_BTree_Node node = BT_Search(&st->functions, &id);
	
	if(!node)
		return NULL;

	if(!node->data)
		return NULL;
	
	return (FUNCTION_PTR)node->data;	
}

SYMBOL_PTR stInsertStaticValue(FUNCTION_PTR function, STRING id, enum_RetVal retval, SYMBOL_PTR* symbol){
		
 	fprintf(stderr, "|| Static val in %s : %s\n", id.data, debugRetval(retval));
	
	bool wasEmpty = false;
	
	if(!(*symbol)){
		*symbol = gcMalloc(sizeof(SYMBOL));
		wasEmpty = true;
	}
	
	if(wasEmpty || !(*symbol)->items)
		(*symbol)->items = gcMalloc(sizeof(ITEM));
	//strCopy(&id, &(*symbol)->id); //mozno ne?
	
	
	switch(retval){
		case TTYPE_NULL:
			(*symbol)->items->type = TYPE_NULL;
			(*symbol)->filgy = true;
			break;
		case TTYPE_TRUE:
			(*symbol)->items->type = TYPE_BOOL;
			(*symbol)->items->value.valBool = true;
			(*symbol)->filgy = true;
			break;
		case TTYPE_FALSE:
			(*symbol)->items->type = TYPE_BOOL;
			(*symbol)->items->value.valBool = false;
			(*symbol)->filgy = true;
			break;
		case TTYPE_STRING:
			(*symbol)->items->type = TYPE_STRING;
			strCopy(&id, &((*symbol)->items->value.valString));
			(*symbol)->filgy = true;
			break;
		case TTYPE_NUMBER:
			(*symbol)->items->type = TYPE_DIGIT_INT;
		
			sscanf(id.data, "%d", &((*symbol)->items->value.valInt));
			(*symbol)->filgy = true;
			break;
		case TTYPE_DEC_NUMBER:
			(*symbol)->items->type = TYPE_DIGIT_DOUBLE;
			
			sscanf(id.data, "%lf", &((*symbol)->items->value.valDouble));
			(*symbol)->filgy = true;
			break;
		case TTYPE_VARIABLE:
			(*symbol)->items->type = TYPE_CALLABLE;
			(*symbol)->filgy = true;
			break;
		default:
			(*symbol)->items->type = TYPE_OTHER;
			break;
	}
	
	listInsertEnd(&function->staticValues, (*symbol));

	return (*symbol);
}

SYMBOL_PTR stInsertStaticValueEmpty(FUNCTION_PTR function){
	SYMBOL_PTR symbol = gcMalloc(sizeof(SYMBOL));
	
	symbol->id.data = NULL;
	symbol->type = TTYPE_CONSTANT;
	symbol->items = NULL;
	symbol->filgy = false;
	
	
	listInsertEnd(&function->staticValues, symbol);
	
	return symbol;
}

SYMBOL_PTR stSearchSymbol(FUNCTION_PTR function, STRING id){
	struct_BTree_Node node = BT_Search(&function->symbols, &id);
	
	if(!node)
		return NULL;
	
	if(!node->data)
		return NULL;
	
	return (SYMBOL_PTR)node->data;	
}

SYMBOL_PTR stGetLastSymbol(FUNCTION_PTR function){
	if(!function)
		return NULL;
		
	if(!function->symbols.last)
		return NULL;

	return function->symbols.last->data;
}

void stFree(SYMBOL_TABLE_PTR st){
	if(!st)
		return;
	
	BT_Free(&st->start.symbols);
	BT_Free(&st->functions);
	
	listFree(&st->start.staticValues);
	listFree(&st->start.instructions);
	
	st->start.calledCount = 0;
	
	st->curr = NULL;
}

// Helpers

INSTRUCTION_PTR makeInstruction(INSTRUCTION_TYPE type, PTR operand1, PTR operand2, PTR destionation){
	INSTRUCTION_PTR i = gcMalloc(sizeof(INSTRUCTION));
	fprintf(stderr, "Creating instruction: %s\n", debugInstruction(type));
	i->type = type;
	i->operand1 = operand1;
	i->operand2 = operand2;
	i->destionation = destionation;
	
	return i;
}

const char* debugInstruction(INSTRUCTION_TYPE i){
	switch(i){
		case INSTRUCTION_RETURN:
			return "return";
		case INSTRUCTION_POP:
			return "pop";
		case INSTRUCTION_PUSH:
			return "push";
		case INSTRUCTION_MOV:
			return "mov";
		case INSTRUCTION_LOAD_NULL:
			return "loadnull";
		case INSTRUCTION_NOP:
			return "nop";
		case INSTRUCTION_ADDITION:
			return "addition";
		case INSTRUCTION_SUBSTRACTION:
			return "substraction";
		case INSTRUCTION_MULTIPLICATION:
			return "multiplication";
		case INSTRUCTION_DIVISION:
			return "division";
		case INSTRUCTION_CONCATE:
			return "concate";
		case INSTRUCTION_EQUAL:
			return "equal";
		case INSTRUCTION_NOT_EQUAL:
			return "not equal";
		case INSTRUCTION_LESS:
			return "less?";
		case INSTRUCTION_LESS_EQUAL:
			return "less_equal";
		case INSTRUCTION_GREATER:
			return "greater";
		case INSTRUCTION_GREATER_EQUAL:
			return "greater_equal";
		case INSTRUCTION_JUMP:
			return "jump";
		case INSTRUCTION_IF_JUMP:
			return "jump_if";
		case INSTRUCTION_LABEL:
			return "label";
		case INSTRUCTION_CALL:
			return "call";
		case INSTRUCTION_BOOLVAL:
			return "boolval";
		case INSTRUCTION_DOUBLEVAL:
			return "doubleval()";
		case INSTRUCTION_INTVAL:
			return "intval";
		case INSTRUCTION_STRVAL:
			return "strval";
		case INSTRUCTION_GET_STRING:
			return "get_string";		
		case INSTRUCTION_PUT_STRING:
			return "put_string";
		case INSTRUCTION_STRLEN:
			return "strlen";
		case INSTRUCTION_GET_SUBSTRING:
			return "get_substring";
		case INSTRUCTION_FIND_STRING:
			return "find_string";
		case INSTRUCTION_SORT_STRING:
			return "sort_string";
		default:
			return "undefined";
	}
}

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