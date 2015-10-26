/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   interpret.c
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

#include "interpret.h"

ERROR op_check(SYMBOL_PTR smb, char IO)
{
	if(smb == NULL)
		return E_COMPILATOR;
	else
	{
		if(smb->id.data == NULL)
		{
			if(smb->items == NULL)
				if(IO == 'o' || IO == 'O')
				{
					smb->items = gcMalloc(sizeof(struct ITEM));
					return E_OK;
				}
				else
					return E_COMPILATOR;
			else
			{
				if(IO == 'i' || IO == 'I')
				{
					if(smb->filgy == false)
						return E_SEMANTIC_UNDECLARED;
					else
						return E_OK;
				}
				else
					return E_OK;
			}
		}
		else
		{
			if(smb->items == NULL)
			{
				if(IO == 'o' || IO == 'O')
				{
					smb->items = gcMalloc(sizeof(struct ITEM));
					return E_OK;
				}
				else
					return E_COMPILATOR;
			}
			else
			{
				if(IO == 'i' || IO == 'I')
				{
					if(smb->filgy == false)
						return E_SEMANTIC_UNDECLARED;
					else
						return E_OK;
				}
				else
					return E_OK;
			}
		}
	}
	if(IO == 'i' || IO == 'I')
	{
		if(smb->filgy == false)
			return E_SEMANTIC_UNDECLARED;
		else
			return E_OK;
	}
	else
		return E_OK;
}

#define data_copy(data_src, data_dst) \
{\
	if((data_dst) != NULL)\
	{\
		if((data_dst)->items != NULL)\
			if((data_dst)->items->type == TYPE_STRING) \
			{\
				gcFree(&(data_dst)->items->value.valString); \
			}\
	}\
	else\
	{\
		(data_dst) = gcMalloc(sizeof(struct SYMBOL));\
	}\
	ITEMPTR item;\
	item = gcMalloc(sizeof(struct ITEM));\
	(data_dst)->items = item;\
	\
	if((data_src)->items->type == TYPE_DIGIT_INT)\
	{\
		((data_dst)->items->type) = TYPE_DIGIT_INT; \
		((data_dst)->items->value.valInt) = ((data_src)->items->value.valInt); \
	}\
	else if((data_src)->items->type == TYPE_DIGIT_DOUBLE)\
	{\
		((data_dst)->items->type) = TYPE_DIGIT_DOUBLE; \
		((data_dst)->items->value.valDouble) = ((data_src)->items->value.valDouble); \
	}\
	else if((data_src)->items->type == TYPE_BOOL)\
	{\
		((data_dst)->items->type) = TYPE_BOOL; \
		((data_dst)->items->value.valBool) = ((data_src)->items->value.valBool); \
	}\
	else if((data_src)->items->type == TYPE_STRING)\
	{\
		(data_dst)->items->type = TYPE_STRING;\
		strInit(&(data_dst)->items->value.valString);\
		strCopy(&(data_src)->items->value.valString, &(data_dst)->items->value.valString);\
	}\
	else \
	{\
		((data_dst)->items->type) = ((data_src)->items->type); \
	}\
}

ERROR interpret(SYMBOL_TABLE* table)
{
	STACK_PTR stack;
	ERROR err;
	stack = gcMalloc(sizeof(struct STACK));
	stackInit(stack);
	
	/// ----- POZNAMKA ----- 
	err = recursive_interpret(table->curr,stack);

	stackFree(stack);
	gcFree(stack);
	return err;
}

ERROR recursive_interpret(FUNCTION_PTR function, STACK_PTR stack)
{
	
// 	fprintf(stderr,"Zavolana funkce: %d\n",function);
	LIST_NODE_PTR instr_node = function->instructions.begin;
	ERROR err = E_OK;
	
	// ----- Priprava promenych ------
	INSTRUCTION_PTR instruction = NULL;
	int str_from,str_to,tmp_count;
	STRING tmp_string;
	
	STACK_PTR tmp_stack;
	tmp_stack = gcMalloc(sizeof(struct STACK));
	stackInit(tmp_stack);
	
	// NULL SYMBOL
	ITEMPTR null_item;
	null_item = gcMalloc(sizeof(struct ITEM));
	null_item->type = TYPE_NULL;
	
	SYMBOL* null_symbol;
	null_symbol = gcMalloc(sizeof(struct SYMBOL));
	null_symbol->type = TYPE_NULL;
	null_symbol->items = null_item;
	
	// TMP SYMBOL
	ITEMPTR tmp_item;
	
	SYMBOL* tmp_symbol;
	
    SYMBOL* op1 = NULL;
    SYMBOL* op2 = NULL;
    SYMBOL* op3 = NULL;
	
	FUNCTION_PTR tmp_function = NULL;
	
	while(instr_node != NULL && err == E_OK)
	{
		instruction = instr_node->value;
		
		tmp_item = gcMalloc(sizeof(struct ITEM));
		tmp_symbol = gcMalloc(sizeof(struct SYMBOL));
		tmp_symbol->items = tmp_item;
	
		op1 = (SYMBOL*) instruction->operand1;
		op2 = (SYMBOL*) instruction->operand2;
		op3 = (SYMBOL*) instruction->destionation;
		
		fprintf(stderr,"while interpret : %d (%s)\n",instruction->type, debugInstruction(instruction->type));
		
		switch(instruction->type)
		{
			case INSTRUCTION_NOP:
			break;
			
			case INSTRUCTION_MOV:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				
				data_copy(op2, op1);
				op1->filgy = true;
			break;
			
			case INSTRUCTION_LOAD_NULL:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'O')) != E_OK)
					return err;
				if ((err = op_check(op3,'O')) != E_OK)
					return err;
				
				data_copy(null_symbol,op1);
				data_copy(null_symbol,op2);
				data_copy(null_symbol,op3);
				op1->filgy = true;
				op2->filgy = true;
				op3->filgy = true;
			break;
			
			case INSTRUCTION_ADDITION:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_INT;
					tmp_symbol->items->value.valInt = op2->items->value.valInt + op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valInt + op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble + op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble + op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->type = TYPE_STRING;
					
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_DIGIT_INT)
				{
					STRING new_string;
					char* new_char_string = gcMalloc(sizeof(char*));;
					sprintf(new_char_string, "%d", op3->items->value.valInt);
					strInitRaw(&new_string, new_char_string);
					
					tmp_symbol->items->type = TYPE_STRING;
					
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					STRING new_string;
					char* new_char_string = gcMalloc(sizeof(char*));
					sprintf(new_char_string, "%lf", op3->items->value.valDouble);
					strInitRaw(&new_string, new_char_string);
					
					tmp_symbol->items->type = TYPE_STRING;
					
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					
					data_copy(tmp_symbol,op1); // from, to
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_SUBSTRACTION:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_INT;
					tmp_symbol->items->value.valInt = op2->items->value.valInt - op3->items->value.valInt;
					
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valInt - op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble - op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble - op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_MULTIPLICATION:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_INT;
					tmp_symbol->items->value.valInt = op2->items->value.valInt * op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = ((double)op2->items->value.valInt) * op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble * ((double)op3->items->value.valInt);
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble * op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_DIVISION:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					if(op3->items->value.valInt == 0)
						return E_SEMANTIC_ZERO_DIV;
					tmp_symbol->items->type = TYPE_DIGIT_INT;
					tmp_symbol->items->value.valInt = op2->items->value.valInt / op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					if(op3->items->value.valDouble == 0.0)
						return E_SEMANTIC_ZERO_DIV;
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valInt / op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_INT)
				{
					if(op3->items->value.valInt == 0)
						return E_SEMANTIC_ZERO_DIV;
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble / op3->items->value.valInt;
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					if(op3->items->value.valDouble == 0.0)
						return E_SEMANTIC_ZERO_DIV;
					tmp_symbol->items->type = TYPE_DIGIT_DOUBLE;
					tmp_symbol->items->value.valDouble = op2->items->value.valDouble / op3->items->value.valDouble;
					data_copy(tmp_symbol,op1); // from, to
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_LESS:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt < op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble < op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used < op3->items->value.valString.used;
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				data_copy(tmp_symbol,op1);
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_GREATER:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt > op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble > op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used > op3->items->value.valString.used;
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				data_copy(tmp_symbol,op1);
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_LESS_EQUAL:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt <= op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble <= op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used <= op3->items->value.valString.used;
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				data_copy(tmp_symbol,op1);
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_GREATER_EQUAL:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt >= op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble >= op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used >= op3->items->value.valString.used;
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				data_copy(tmp_symbol,op1);
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_EQUAL:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_BOOL;
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt == op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble == op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used == op3->items->value.valString.used;
				}
				else
					tmp_symbol->items->value.valBool = false;
				data_copy(tmp_symbol,op1);
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_NOT_EQUAL:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_BOOL;				
				if(op2->items->type == TYPE_DIGIT_INT && op3->items->type == TYPE_DIGIT_INT)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valInt != op3->items->value.valInt;
				}
				else if(op2->items->type == TYPE_DIGIT_DOUBLE && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valDouble != op3->items->value.valDouble;
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->value.valBool = op2->items->value.valString.used != op3->items->value.valString.used;
					if(!tmp_symbol->items->value.valBool)
					{
						tmp_symbol->items->value.valBool = !strCompare(op2->items->value.valString, op3->items->value.valString.data);
					}
				}
				else
					tmp_symbol->items->value.valBool = false;
				data_copy(tmp_symbol,op1);
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_PUSH:
// 				printf("ahoj %d\n",op1->filgy);
				if ((err = op_check(op1,'I')) != E_OK)
					return err;
				
				stackPush(stack,op1);
			break;
			
			case INSTRUCTION_POP:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				
				if(tmp_symbol == NULL){
					tmp_symbol = gcMalloc(sizeof(SYMBOL));
					tmp_symbol->items = gcMalloc(sizeof(ITEM));
					
					tmp_symbol->items->type = TYPE_NULL;
					tmp_symbol->filgy = true;
					
				}
				
				data_copy(tmp_symbol,op1);
				op1->filgy = true;
			break;
			
			case INSTRUCTION_CALL:
				if(op1 == NULL)
					return E_COMPILATOR;
				
				tmp_function = function;
				function = (FUNCTION_PTR)op1;
				
				err=recursive_interpret(function,stack);
				if (err != E_OK)
					return err;
				
				function = tmp_function;
			break;
			
			case INSTRUCTION_RETURN:
				return err;
			break;
			
			case INSTRUCTION_JUMP:
				if(op3 == NULL)
					return E_COMPILATOR;
				
				instr_node = function->instructions.begin;
				while(instr_node != (LIST_NODE_PTR)op3)
				{
					if(instr_node->next == NULL)
					{
						fprintf(stderr,"NENALEZENO!!!\n");
						fprintf(stderr,"NENALEZENO!!!\n");
						fprintf(stderr,"NENALEZENO!!!\n");
						break;
					}
					instr_node = instr_node->next;
				}
				continue;
			break;
			
			case INSTRUCTION_IF_JUMP:	
// 				if ((err = op_check(op2,'I')) != E_OK)
// 					return err;
				if(op3 == NULL)
					return E_COMPILATOR;
				
				if(op2->items->type == TYPE_BOOL)
				{
					if(!op2->items->value.valBool)
					{
						instr_node = function->instructions.begin;
						while(instr_node != (LIST_NODE_PTR)op3)
						{
							if(instr_node->next == NULL)
								break;
							instr_node = instr_node->next;
						}
						continue;						
					}
				}
// 				if(op2->items->type == TYPE_DIGIT_INT)
// 				{
// 					printf("AHOJ\n");
// 					if(!op2->items->value.valInt)
// 					{
// 						instr_node = function->instructions.begin;
// 						while(instr_node != (LIST_NODE_PTR)op3)
// 						{
// 							if(instr_node->next == NULL)
// 								break;
// 							instr_node = instr_node->next;
// 						}
// 						continue;						
// 					}
// 				}
			break;
			
			case INSTRUCTION_LABEL:
				
			break;
			
			case INSTRUCTION_BOOLVAL:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				tmp_symbol->items = boolval(*tmp_symbol->items);
				
				data_copy(tmp_symbol,op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_DOUBLEVAL:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				tmp_symbol->items = doubleval(*tmp_symbol->items);
				if(tmp_symbol->items == NULL)
					return E_SEMANTIC_DOUBLEVAL;
				
				data_copy(tmp_symbol,op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_INTVAL:	
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				op1 = stackPop(stack);
				if ((err = op_check(op1,'I')) != E_OK)
					return err;
				
				tmp_symbol->items = intval(*op1->items);
				data_copy(tmp_symbol,op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_STRVAL:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				tmp_symbol->items = strval(*tmp_symbol->items);
				
				data_copy(tmp_symbol,op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_GET_STRING:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_string = get_string();
				
				tmp_symbol->items->type = TYPE_STRING;
				strInit(&tmp_symbol->items->value.valString);
				strCopy(&tmp_string, &tmp_symbol->items->value.valString);
				
				data_copy(tmp_symbol, op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_CONCATE:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				if ((err = op_check(op3,'I')) != E_OK)
					return err;
				
				if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_STRING)
				{
					tmp_symbol->items->type = TYPE_STRING;
					strInit(&tmp_symbol->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_DIGIT_INT)
				{
					STRING new_string;
					char* new_char_string = gcMalloc(sizeof(char*));
					sprintf(new_char_string, "%d", op3->items->value.valInt);
					strInitRaw(&new_string, new_char_string);
					
					tmp_symbol->items->type = TYPE_STRING;
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					
					data_copy(tmp_symbol,op1); // from, to
				}
				else if(op2->items->type == TYPE_STRING && op3->items->type == TYPE_DIGIT_DOUBLE)
				{
					STRING new_string;
					char* new_char_string = gcMalloc(sizeof(char*));
					sprintf(new_char_string, "%lf", op3->items->value.valDouble);
					strInitRaw(&new_string, new_char_string);
					
					tmp_symbol->items->type = TYPE_STRING;
					
					strConcatenate(&tmp_symbol->items->value.valString, &op2->items->value.valString);
					strConcatenate(&tmp_symbol->items->value.valString, &op3->items->value.valString);
					
					data_copy(tmp_symbol,op1); // from, to
				}
				else
					return E_SEMANTIC_TYPE_MISMATCH;
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_PUT_STRING:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if (op2 == NULL)
					return E_COMPILATOR;
				
				// DODELAT VRACENI POCTU VYPSANYCH! DO OP1
				
				tmp_count = *((int*)op2);
				for(int i=0;i<tmp_count;i++)
				{
					tmp_symbol = stackPop(stack);
					if ((err = op_check(tmp_symbol,'I')) != E_OK)
						return err;
					stackPush(tmp_stack,tmp_symbol);
				}
				for(int i=0;i<tmp_count;i++)
				{
					tmp_symbol = stackPop(tmp_stack);
					if(tmp_symbol->items->type == 2)
						printf("%d",tmp_symbol->items->value.valInt);
					else if(tmp_symbol->items->type == 3)
						printf("%lf",tmp_symbol->items->value.valDouble);
					else if(tmp_symbol->items->type == 4)
						printf("%s",tmp_symbol->items->value.valString.data);
				}
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_STRLEN:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if (op2 == NULL)
					return E_COMPILATOR;
				
				tmp_count = *((int*)op2);
				if (tmp_count != 1)
					return E_COMPILATOR;
				
				op1 = stackPop(stack);
				if ((err = op_check(op1,'I')) != E_OK)
					return err;
				tmp_symbol->items->type = TYPE_DIGIT_INT;
				tmp_symbol->items->value.valInt = my_strlen(op1->items->value.valString);
				
				data_copy(tmp_symbol, op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_GET_SUBSTRING:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				if (op2 == NULL)
					return E_COMPILATOR;
				tmp_count = *((int*)op2);
				if (tmp_count != 3)
					return E_COMPILATOR;
				
				// TO
				tmp_symbol = stackPop(stack);
				if ((err = op_check(tmp_symbol,'I')) != E_OK)
					return err;
				str_to = tmp_symbol->items->value.valInt;
				
				// FROM
				tmp_symbol = stackPop(stack);
				if ((err = op_check(tmp_symbol,'I')) != E_OK)
					return err;
				str_from = tmp_symbol->items->value.valInt;
				
				// STRING
				tmp_symbol = stackPop(stack);
				if ((err = op_check(tmp_symbol,'I')) != E_OK)
					return err;
				
				tmp_string = get_substring(tmp_symbol->items->value.valString,str_from,str_to,&err);
				if(err != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_STRING;
				strCopy(&tmp_symbol->items->value.valString,&tmp_string);
				data_copy(tmp_symbol,op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_FIND_STRING:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				op2 = stackPop(stack);
				if ((err = op_check(op2,'I')) != E_OK)
					return err;
				
				op3 = stackPop(stack);
				if ((err = op_check(op1,'I')) != E_OK)
					return err;
				
				tmp_symbol->items->type = TYPE_DIGIT_INT;
				tmp_symbol->items->value.valInt = find_string(op3->items->value.valString, op2->items->value.valString);
				data_copy(tmp_symbol,op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			case INSTRUCTION_SORT_STRING:
				if ((err = op_check(op1,'O')) != E_OK)
					return err;
				
				tmp_symbol = stackPop(stack);
				if ((err = op_check(tmp_symbol,'I')) != E_OK)
					return err;
				
				sort_string(tmp_symbol->items->value.valString);
				data_copy(tmp_symbol,op1);
				op1->filgy = true;
				tmp_symbol = NULL;
			break;
			
			default:
				return E_OK;
			break;
			
		}
		instr_node = instr_node->next;
	}
	return err;
}
