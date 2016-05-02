/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   exp.c
 *
 * Popis:
 *
 *
 * Datum:    29.11.2013
 *
 * Autori:   Frantisek Kolacek   <xkolac12@stud.fit.vutbr.cz>
 *           Stodulka Daniel
 *           Hermann Lukas
 *           Tran Manh Hoang
 */

#include "exp.h"

/*
const char expressionPrecedentTable[EXPRESSION_TABLE_SIZE][EXPRESSION_TABLE_SIZE] = {
  //Tokeny									id  (   )   +   -   *   /   **  ==  !=  <   <=  >   >=
  [TTYPE_VARIABLE]			={[TTYPE_VARIABLE]= 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_L_BRACKET]			={[TTYPE_VARIABLE]='<','<','=','<','<','<','<','<','<','<','<','<','<','<',[TTYPE_SEMICOLON]= 0 },
  [TTYPE_R_BRACKET]			={[TTYPE_VARIABLE]= 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_ADDITION]			={[TTYPE_VARIABLE]='<','<','>','>','>','<','<','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_SUBTRACTION]		={[TTYPE_VARIABLE]='<','<','>','>','>','<','<','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_MULTIPLICATION]	={[TTYPE_VARIABLE]='<','<','>','>','>','>','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_DIVISION]			={[TTYPE_VARIABLE]='<','<','>','>','>','>','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_POWER]				={[TTYPE_VARIABLE]='<','<','>','>','>','>','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_EQUAL]				={[TTYPE_VARIABLE]='<','<','>','<','<','<','<','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_NOT_EQUAL]			={[TTYPE_VARIABLE]='<','<','>','<','<','<','<','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_LESSER]			={[TTYPE_VARIABLE]='<','<','>','<','<','<','<','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_LESSER_EQUAL]		={[TTYPE_VARIABLE]='<','<','>','<','<','<','<','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_GREATER]			={[TTYPE_VARIABLE]='<','<','>','<','<','<','<','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_GREATER_EQUAL]		={[TTYPE_VARIABLE]='<','<','>','<','<','<','<','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_SEMICOLON]			={[TTYPE_VARIABLE]='<','<', 0 ,'<','<','<','<','<','<','<','<','<','<','<',[TTYPE_SEMICOLON]='$'},
};
*/
const char expressionPrecedentTable[EXPRESSION_TABLE_SIZE][EXPRESSION_TABLE_SIZE] = {
  //Tokeny									id  (   )   +   -   *   /   .  ==  !=  <   <=  >   >=
  [TTYPE_VARIABLE]			={[TTYPE_VARIABLE]= 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_L_BRACKET]			={[TTYPE_VARIABLE]='<','<','=','<','<','<','=','<','<','<','<','<','<','<',[TTYPE_SEMICOLON]= 0 },
  [TTYPE_R_BRACKET]			={[TTYPE_VARIABLE]= 0 , 0 ,'>','>','>','>','>','>','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_ADDITION]			={[TTYPE_VARIABLE]='<','<','>','>','>','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_SUBTRACTION]		={[TTYPE_VARIABLE]='<','<','>','>','>','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_MULTIPLICATION]	={[TTYPE_VARIABLE]='<','<','>','>','>','>','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_DIVISION]			={[TTYPE_VARIABLE]='<','<','>','>','>','>','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_DOT]				={[TTYPE_VARIABLE]='<','<','>','>','>','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_EQUAL]				={[TTYPE_VARIABLE]='<','<','>','<','<','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_NOT_EQUAL]			={[TTYPE_VARIABLE]='<','<','>','<','<','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_LESSER]			={[TTYPE_VARIABLE]='<','<','>','<','<','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_LESSER_EQUAL]		={[TTYPE_VARIABLE]='<','<','>','<','<','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_GREATER]			={[TTYPE_VARIABLE]='<','<','>','<','<','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_GREATER_EQUAL]		={[TTYPE_VARIABLE]='<','<','>','<','<','<','>','<','>','>','>','>','>','>',[TTYPE_SEMICOLON]='>'},
  [TTYPE_SEMICOLON]			={[TTYPE_VARIABLE]='<','<', 0 ,'<','<','<', 0 ,'<','<','<','<','<','<','<',[TTYPE_SEMICOLON]='$'},
};

ERROR parserExpression(SYMBOL_TABLE_PTR st, enum_RetVal retval, SYMBOL_PTR* symbol){
	ERROR err = E_OK;
	enum_RetVal term1, term2;
	char weight;
	
	INSTRUCTION_PTR i = NULL;
	INSTRUCTION_TYPE iType = INSTRUCTION_NOP;
	
	SYMBOL_PTR destination, source1, source2;
	destination = NULL;
	source1 = NULL;
	source2 = NULL;	
	
	EXPRESSION_PTR expression;
	STACK stack;
	
	stackInit(&stack);
	stackPush(&stack, makeExpression(TTYPE_SEMICOLON, NULL));
	
	fprintf(stderr,"[exp] Parse expression, begin: %s\n", glob_Token.data.data);
	
	do{
		
		term1 = getTerm(&stack);
		fprintf(stderr,"[exp] Get term1 from stack [%s]\n", debugRetval(term1));
		term2 = retval;
		fprintf(stderr,"[exp] Get term2 from retval [%s]\n", debugRetval(term2));
		
		if(
			term2 == TTYPE_VARIABLE ||
			//term2 == TTYPE_FUNCTION ||
			term2 == TTYPE_NUMBER ||
			term2 == TTYPE_DEC_NUMBER ||
			term2 == TTYPE_TRUE ||
			term2 == TTYPE_FALSE ||
			term2 == TTYPE_NULL ||
			term2 == TTYPE_STRING			
		){
			weight = expressionPrecedentTable[term1][TTYPE_VARIABLE];
			
			if(stackBegin(&stack) && ((EXPRESSION_PTR)stackBegin(&stack))->retval == TTYPE_EXPRESSION)
				weight = '0';
		}
		else{
			weight = expressionPrecedentTable[term1][term2];
		}
		
		if(weight == '0'){
			fprintf(stderr,"[exp] Reset term2 to semicolon\n");
			term2 = TTYPE_SEMICOLON;
			weight = expressionPrecedentTable[term1][term2];
		}			
		
		fprintf(stderr,"[exp] Weight: '%c', stack size %d\n", weight, stackCount(&stack));
		
		switch(weight){
			case '$':				
				if(stackCount(&stack) == 0)
					return E_SYNTAX;
				expression = (EXPRESSION_PTR)stackPop(&stack);
				
				if(expression->retval != TTYPE_EXPRESSION){
					fprintf(stderr,"[exp] Expect expression, get: %s\n", debugRetval(expression->retval));
					return E_SYNTAX;
				}
				
				fprintf(stderr, "[exp-out] Return symbol type: %d\n", expression->symbol->type);

				*symbol = expression->symbol;					
				break;
			case '<':
			case '=':
				fprintf(stderr,"[exp] Push term2 [%s] to stack\n", debugRetval(term2));
				err = pushExpression(st, &stack, NULL, term2);
				if(err != E_OK)
					return err;
				
				retval = getToken();
				if(getNextToken() == TTYPE_L_BRACE){
					//UBER COOL SUPER MAGIC
					fprintf(stderr, "[exp] Filgy magic - forcing end of expression\n");
					
					retval = TTYPE_SEMICOLON;
					//optional
					strInitRaw(&glob_Token.data, ";");
				}
				
				fprintf(stderr,"[exp] Get token: %s\n", glob_Token.data.data);
				break;
			case '>':
				if(stackCount(&stack) == 0)
					return E_SYNTAX;
				
				expression = (EXPRESSION_PTR)stackPop(&stack);	
				fprintf(stderr,"[exp] Pop expression [%s] from stack\n", debugRetval(expression->retval));
				
				//E->(E)
				if(expression->retval == TTYPE_R_BRACKET){
					fprintf(stderr,"[exp] Type: E->(E)\n");
					if(stackCount(&stack) == 0)
						return E_SYNTAX;	
					
					iType = INSTRUCTION_NOP;
					
					expression = (EXPRESSION_PTR)stackPop(&stack);
				
					if(expression->retval != TTYPE_EXPRESSION)
						return E_SYNTAX;						
					
					//magic
					destination = expression->symbol;
						
					if(stackCount(&stack) == 0)
						return E_SYNTAX;
					
					expression = (EXPRESSION_PTR)stackPop(&stack);
				
					if(expression->retval != TTYPE_L_BRACKET)
						return E_SYNTAX;
				}
				//E-> E op E
				else if(expression->retval == TTYPE_EXPRESSION){
					fprintf(stderr,"[exp] Type: E op E\n");
					if(stackCount(&stack) == 0)
						return E_SYNTAX;
					
					source2 = expression->symbol;
					
					expression = (EXPRESSION_PTR)stackPop(&stack);
					
					if(!isop(expression->retval))
						return E_SYNTAX;
					
					iType = type(expression->retval);

					if(stackCount(&stack) == 0)
						return E_SYNTAX;
					
					expression = (EXPRESSION_PTR)stackPop(&stack);
				
					if(expression->retval != TTYPE_EXPRESSION)
						return E_SYNTAX;
					
					source1 = expression->symbol;					
				}
				else
					return E_SYNTAX;
				
				if(iType != INSTRUCTION_NOP){
						fprintf(stderr,"[exp] Creating blank constant\n");
						destination = stInsertStaticValueEmpty(st->curr);
						fprintf(stderr, "I: %s, dest: %d, src1: %d, src2: %d\n", debugInstruction(iType), destination, source1, source2);

						i = makeInstruction(iType, destination, source1, source2); // itype
						listInsertEnd(&st->curr->instructions, i);					
				}
				err = pushExpression(st, &stack, destination, TTYPE_EXPRESSION);
				fprintf(stderr,"[exp] Push expression to stack in E op E (stack size: %d)\n", stackCount(&stack));
				
				if(err != E_OK)
					return err;
				
				break;
			default:
				fprintf(stderr,"ERROR EXPRESSION\n");
				return E_SYNTAX;
		}
		
	}while( !(term1 == TTYPE_SEMICOLON && term2 == TTYPE_SEMICOLON) );

	stackFree(&stack);

	return E_OK;
}

//Helpers

EXPRESSION_PTR makeExpression(enum_RetVal retval, SYMBOL_PTR symbol){
	EXPRESSION_PTR expression = gcMalloc(sizeof(EXPRESSION));
	
	expression->retval = retval;
	expression->symbol = symbol;
	
	return expression;
}

enum_RetVal getTerm(STACK_PTR stack){
	STACK_NODE_PTR node = stack->begin;
	
	while(node && ((EXPRESSION_PTR)(node->value))->retval == TTYPE_EXPRESSION)
		node = node->next;
	
	if(!node)
		return TTYPE_EXPRESSION;
	else
		return ((EXPRESSION_PTR)(node->value))->retval;
}

ERROR pushExpression(SYMBOL_TABLE_PTR st, STACK_PTR stack, SYMBOL_PTR symbol, enum_RetVal retval){
	if(retval == TTYPE_VARIABLE){
		fprintf(stderr,"[exp-push] Retval is variable\n");
		if(!(symbol = stSearchSymbol(st->curr, glob_Token.data)))
			return E_SEMANTIC_TYPE_MISMATCH;
			
		retval = TTYPE_EXPRESSION;
	}
	else if(
		retval == TTYPE_NULL ||
		retval == TTYPE_TRUE ||
		retval == TTYPE_FALSE || 
		retval == TTYPE_STRING ||
		retval == TTYPE_NUMBER ||
		retval == TTYPE_DEC_NUMBER ||
		retval == TYPE_OTHER
	){
		
		stInsertStaticValue(st->curr, glob_Token.data, retval, &symbol);
		fprintf(stderr,"[exp-push] Insert static value: %s\n", glob_Token.data.data);

		retval = TTYPE_EXPRESSION;
	}
	
	stackPush(stack, makeExpression(retval, symbol));
	//printf("1]]][[[ %d: %d\n", symbol, symbol->items);
	
	return E_OK;
}