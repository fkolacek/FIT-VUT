/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   parser.c
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

/* 
 * <program> - <body_program>
 * <body_program> - <def_function><body_program>
 * <body_program> - <command>;<body_program>
 * <body_program> - eps
 * <def_function> - function idFunction (<params>) { <stat_list> }
 * <stat_list> - eps
 * <stat_list> - <command> ; <stat_list>
 * <param> - id <params>
 * <param> - eps
 * <params> - , id <params>
 * <params> - eps
 * <command> - id = <assign> ;
 * <command> - if (expression) { <stat_list> } else { <stat_list> }
 * <command> - while (expression) { <stat_list> }
 * <command> - return expression ;
 * <assign> - expression ;
 * <assign> - idFunction( <params> ) ;
 * <assign> - boolval(term) ;
 * <assign> - doubleval(term) ;
 * <assign> - intval(term) ;
 * <assign> - strval(term) ;
 * <assign> - get_string() ;
 * <assign> - put_string(term) ;
 * <assign> - strlen(term) ;
 * <assign> - get_substring(term, num, num) ;
 * <assign> - find_string(string, string) ;
 * <assign> - sort_string(string) ;
 * <num> - eps
 * <num> - num
 * <term> - id
 * <term> - <value>
 * <value> - num
 * <value> - string
 * <value> - bool
 * <value> - null
 */

#include "parser.h"

//Hlavni funkce parseru
ERROR parser(SYMBOL_TABLE_PTR st){
	
	ERROR err;	
	
	if(getToken() != TTYPE_PHP_START)
		return E_SYNTAX;
	
	err = parserFindFunctions(st);
	
	if(err != E_OK)
		return err;
	
	init_Token();
	
	if(getToken() != TTYPE_PHP_START)
		return E_SYNTAX;
	
	return parserParse(st);	
}

//Vyhleda definice funkci
ERROR parserFindFunctions(SYMBOL_TABLE_PTR st){
	fprintf(stderr,"Finding function definitions:\n");
	enum_RetVal retval;
	
	init_Token();

	while((retval = getToken()) != TTYPE_EOF){		
		//fprintf(stderr,"Token: %s\n", glob_Token.data.data);
		if(retval == TTYPE_KEYWORD && strCompare(glob_Token.data, "function")){
			retval = getToken();
			
			if(retval == TTYPE_FUNCTION){
				fprintf(stderr,"- %s\n", glob_Token.data.data);
				
				if(stSearchFunction(st, glob_Token.data))
					return E_SEMANTIC;
								
				stInsertFunction(st, glob_Token.data);
				
				st->curr = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				parserParseFunctionParam(st, true);
			}
			else if(retval == TTYPE_RESERVED)
				return E_SEMANTIC;
			else
				return E_SYNTAX;
		}
		else if(retval == TTYPE_ERROR)
			return E_SYNTAX;
	}	
	
	free_Token();
	
	return E_OK;
}

//Funkce pro druhe parsovani
ERROR parserParse(SYMBOL_TABLE_PTR st){
	ERROR err = E_OK;

	st->curr = &st->start;
	
	enum_RetVal retval = getToken();
	
	if(retval == TTYPE_EOF)
		return err;
	else if(retval == TTYPE_KEYWORD && strCompare(glob_Token.data, "function")){
		err = parserParseFunction(st);
		
		if(err != E_OK)
			return err;
		
		return parserParse(st);
	}
	else{
		err = parserParseCode(st, retval);
		
		if(err != E_OK)
			return err;
		
		return parserParse(st);
	}
}

//Parsovani definice funkce
//function id (<params>){ <code> }
ERROR parserParseFunction(SYMBOL_TABLE_PTR st){
	fprintf(stderr,"Parsing definition function\n");
	ERROR err = E_OK;
	
	if(getToken() != TTYPE_FUNCTION)
		return E_SYNTAX;
	
	st->curr = stSearchFunction(st, glob_Token.data);
	
	if(!st->curr)
		return E_COMPILATOR;
	
	fprintf(stderr,"Change current function to %s\n", st->curr->id.data);
	
	if(getToken() != TTYPE_L_BRACKET)
		return E_SYNTAX;
	
	err = parserParseFunctionParam(st, false);
	
	if(err != E_OK)
		return err;
		
	listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_NOP, NULL, NULL, NULL));

	if(getToken() != TTYPE_L_BRACE)
		return E_SYNTAX;
	
	err = parserParseFunctionCode(st);


	return err;
}

//Parsovani parametru definice funkce (0-1)
// <params> - id <params_n>
// <params> - eps
ERROR parserParseFunctionParam(SYMBOL_TABLE_PTR st, bool test){
	fprintf(stderr,"Parsing function %s definition param\n", st->curr->id.data);
	
	enum_RetVal retval = getToken();
	
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	else if(retval == TTYPE_VARIABLE){
		//Stejny nazev parametru a funkce - ale nemozne v PHP
		//if(stSearchFunction(st, glob_Token.data))
		//	return E_SEMANTIC;
		
		if(test)
			st->curr->argumentsCount++;
		else{
			stInsertSymbol(st->curr, glob_Token.data);
			
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_POP, stGetLastSymbol(st->curr), NULL, NULL));
		}
		
		return parserParseFunctionParams(st, test);
	}
	else
		return E_SYNTAX;
}

//Parsovani parametru definice funkce (2+)
// <params_n> - , id <params_n>
// <params_n> - eps
ERROR parserParseFunctionParams(SYMBOL_TABLE_PTR st, bool test){
	fprintf(stderr,"Parsing function definition params\n");
	
	enum_RetVal retval = getToken();
	
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	else if(retval == TTYPE_COMMA){
		
		retval = getToken();
		
		if(retval == TTYPE_VARIABLE){
			//Stejny nazev parametru a funkce - ale nemozne v PHP
			//if(stSearchFunction(st, glob_Token.data))
			//	return E_SEMANTIC;			
			
			if(test)
				st->curr->argumentsCount++;
			else{
				//Stejny nazev dvou parametru
				if(stSearchSymbol(st->curr, glob_Token.data))
					//return E_SEMANTIC;
					return E_SEMANTIC_OTHER;

				stInsertSymbol(st->curr, glob_Token.data);
			
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_POP, stGetLastSymbol(st->curr), NULL, NULL));
				st->curr->argumentsCount++;
			}
			
			return parserParseFunctionParams(st, test);
		}
		else
			return E_SYNTAX;
	}
	else
		return E_SYNTAX;	
}
//Parsovani tela definice funkce
// <code> - eps
// <code> - <command> <code>
ERROR parserParseFunctionCode(SYMBOL_TABLE_PTR st){
	fprintf(stderr,"Parsing function code\n");
	
	enum_RetVal retval = getToken();
	
	if(retval == TTYPE_R_BRACE)
		return E_OK;
	//else if(retval == TTYPE_ELSE) // po ifu musi byt vzdy else
	//	return E_SYNTAX;
	else{
		fprintf(stderr,"Parsing function body\n");
		ERROR err = parserParseCode(st, retval);
		
		if(err == E_OK)
			return parserParseFunctionCode(st);
		else
			return err;
	}
}
//Parsovani prikazu
ERROR parserParseCode(SYMBOL_TABLE_PTR st, enum_RetVal retval){
	fprintf(stderr,"Parsing code [%d]: %s\n", retval, glob_Token.data.data);
	
	ERROR err = E_OK;	
	INSTRUCTION_PTR i1, i2, i3;
	SYMBOL_PTR symbol;
	
	switch(retval){
		case TTYPE_VARIABLE:
			fprintf(stderr,"Found variable: %s\n", glob_Token.data.data);
			
			if(!stSearchSymbol(st->curr, glob_Token.data))
				stInsertSymbol(st->curr, glob_Token.data);
			
			SYMBOL_PTR tmpSymbol = stSearchSymbol(st->curr, glob_Token.data);
	
			//Stejny nazev parametru a funkce - ale nemozne v PHP
			//if(!stSearchFunction(st, glob_Token.data))
			//	return E_SEMANTIC;
				
			retval = getToken();
			
			//Prazdny prikaz
			if(retval == TTYPE_SEMICOLON)
				return E_OK;
			else if(retval != TTYPE_ASSIGN)
				return E_SYNTAX;
			else{
				//DEBUG FILGY
				fprintf(stderr,"Found assign and last symbol was: %s\n", tmpSymbol->id.data);
				//err = parserControlAssign(st, stGetLastSymbol(st->curr));
				err = parserControlAssign(st, tmpSymbol);
				
				if(err != E_OK)
					return err;
			}
			
			break;
			
		//FILGY-TODO - samotne volani funkce
		case TTYPE_FUNCTION:
			fprintf(stderr,"Found function: %s\n", glob_Token.data.data);
			fprintf(stderr, "FILGY-TODO: samotne volani funkce\n");
			break;
		
		case TTYPE_KEYWORD:
			fprintf(stderr,"Found keyword: %s\n", glob_Token.data.data);
			
			//if(<expression>){ <code> }else{ <code> }
			if(strCompare(glob_Token.data, "if")){
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;

				
				retval = getToken();
				
				//Parsovani podminky
				err = parserExpression(st, retval, &symbol);
				
				if(err != E_OK)
					return err;
				
				// Jump na else
				listInsertEnd(&st->curr->instructions, (i1 = makeInstruction(INSTRUCTION_IF_JUMP, NULL, symbol, NULL)));
				
				if(getToken() != TTYPE_L_BRACE)
					return E_SYNTAX;

				// Parsovani tela podminky
				err = parserParseFunctionCode(st);
				
				if(err != E_OK)
					return err;
				
				//Jump na konec else
				listInsertEnd(&st->curr->instructions, (i2 = makeInstruction(INSTRUCTION_JUMP, NULL, NULL, NULL)));
				
				// Navesti pro else
				listInsertEnd(&st->curr->instructions, (i3 = makeInstruction(INSTRUCTION_LABEL, NULL, NULL, NULL)));
				
				i1->destionation = st->curr->instructions.end;
				
				if(getToken() != TTYPE_KEYWORD || !strCompare(glob_Token.data, "else"))
					return E_SYNTAX;
				
				fprintf(stderr,"Found else\n");
				
				if(getToken() != TTYPE_L_BRACE)
					return E_SYNTAX;
				
				err = parserParseFunctionCode(st);
				
				if(err != E_OK)
					return err;

				// Navesti pro konec else
				listInsertEnd(&st->curr->instructions, (i3 = makeInstruction(INSTRUCTION_LABEL, NULL, NULL, NULL)));
				
				i2->destionation = st->curr->instructions.end;
				
				fprintf(stderr, "If/else completed\n");
			}
// 			//while(<expression>){ <code> }
			else if(strCompare(glob_Token.data, "while")){
				
				i2 = makeInstruction(INSTRUCTION_JUMP, NULL, NULL, NULL);
				i3 = makeInstruction(INSTRUCTION_LABEL, NULL, NULL, NULL);
				
				listInsertEnd(&st->curr->instructions, i3);
				i2->destionation = st->curr->instructions.end;
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				retval = getToken();
				
				//Parsovani podminky
				err = parserExpression(st, retval, &symbol);
				
				if(err != E_OK)
					return err;
				
				i1 = makeInstruction(INSTRUCTION_IF_JUMP, NULL, symbol, NULL);
				listInsertEnd(&st->curr->instructions, i1);
				
				if(getToken() != TTYPE_L_BRACE)
					return E_SYNTAX;
				
				//Parsovani tela cyklu
				err = parserParseFunctionCode(st);
				
				if(err != E_OK)
					return err;
				
				listInsertEnd(&st->curr->instructions, i2);
				
				i3 = makeInstruction(INSTRUCTION_LABEL, NULL, NULL, NULL);
				listInsertEnd(&st->curr->instructions, i3);
				
				i1->destionation = st->curr->instructions.end;
				
				fprintf(stderr, "While completed\n");
			}
			//return expression;
			else if(strCompare(glob_Token.data, "return")){
				retval = getToken();
				
				//EDIT
				err = parserExpression(st, retval, &symbol);
				
				if(err != E_OK)
					return err;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_PUSH, symbol, NULL, NULL));
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_RETURN, NULL, NULL, NULL));
				
				//if(getToken() != TTYPE_SEMICOLON)
				//	return E_SYNTAX;
			}
			else
				return E_SYNTAX;
			break;
		default:
			fprintf(stderr,"[debug] SYNTAX ERROR: [%s] %s\n", debugRetval(retval), glob_Token.data.data);
			return E_SYNTAX;
	}
	
	return err;
}

// <assign> - expression
// <assign> - id(<params>)
// <assign> - boolval(<term>)
// <assign> - doubleval(<term>)
// <assign> - intval(<term>)
// <assign> - strval(<term>)
// <assign> - get_string()
// <assign> - put_string(<term>)
// <assign> - strlen(<term>)
// <assign> - get_substring(<term>,<num>,<num>)
// <assign> - find_string(string, string)
// <assign> - sort_string(string)
ERROR parserControlAssign(SYMBOL_TABLE_PTR st, SYMBOL_PTR symbol){
	fprintf(stderr,"Control assign\n");
	ERROR err = E_OK;
	
	enum_RetVal retval = getToken();
	FUNCTION_PTR f = NULL;
	SYMBOL_PTR tmp = NULL;
	
	switch(retval){
		case TTYPE_RESERVED:
			if(strCompare(glob_Token.data, "boolval")){
				fprintf(stderr," - assign lib boolval\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_BOOLVAL, symbol, NULL, NULL));
			}
			else if(strCompare(glob_Token.data, "doubleval")){
				fprintf(stderr," - assign lib boolval\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_DOUBLEVAL, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "intval")){
				fprintf(stderr," - assign lib intval\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_INTVAL, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "strval")){
				fprintf(stderr," - assign lib strval\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_STRVAL, symbol, NULL, NULL));				
			}			
			else if(strCompare(glob_Token.data, "get_string")){
				fprintf(stderr," - assign lib get_string\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_GET_STRING, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "put_string")){
				fprintf(stderr," - assign lib put_string\n");
				
				f = stSearchFunction(st, glob_Token.data);

				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				int* tmpCount = gcMalloc(sizeof(int));
				
				*tmpCount = f->argumentsCalled;

				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_PUT_STRING, symbol, tmpCount, NULL));				
				f->argumentsCalled = 0;
			}
			else if(strCompare(glob_Token.data, "strlen")){
				fprintf(stderr," - assign lib strlen\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_STRLEN, symbol,NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "get_substring")){
				fprintf(stderr," - assign lib get_substring\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_GET_SUBSTRING, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "find_string")){
				fprintf(stderr," - assign lib find_string\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_FIND_STRING, symbol, NULL, NULL));				
			}
			else if(strCompare(glob_Token.data, "sort_string")){
				fprintf(stderr," - assign lib sort_string\n");
				
				f = stSearchFunction(st, glob_Token.data);
				
				if(getToken() != TTYPE_L_BRACKET)
					return E_SYNTAX;
				
				err = parserParseCallParam(st, f);
				
				if(err != E_OK)
					return err;
				
				if(getToken() != TTYPE_SEMICOLON) //weak
					return E_SYNTAX;
				
				listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_SORT_STRING, symbol, NULL, NULL));				
			}
			else
				return E_SYNTAX;
			break;
		// variable + 5
		case TTYPE_VARIABLE:
			fprintf(stderr," - assign variable\n");
			
			err = parserExpression(st, retval, &tmp);

			if(err != E_OK)
				return err;
			
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_MOV, symbol, tmp, NULL));
			//printf("DEBUG: %d\n", tmp->type);
			
			//HOTFIX BY FILGY
			//if(!tmp->filgy)
			//	return E_SEMANTIC_UNDECLARED;
			
			fprintf(stderr," - assign variable completed\n");
			break;
		// funkce(x)
		case TTYPE_FUNCTION:
			fprintf(stderr," - assign function\n");
			
			if(!(f = stSearchFunction(st, glob_Token.data)))
				return E_SEMANTIC;
			
			if(getToken() != TTYPE_L_BRACKET)
				return E_SYNTAX;
			
			err = parserParseCallParam(st, f);
			
			if(err != E_OK)
				return err;
				
			if(getToken() != TTYPE_SEMICOLON)
				return E_SYNTAX;			
			
			if(f->argumentsCount != f->argumentsCalled){
				fprintf(stderr, " - error call function %s - arguments required %d, passed %d\n", f->id.data, f->argumentsCount, f->argumentsCalled);
				return E_SEMANTIC_MISS_PARAM;
			}
			
			f->argumentsCalled = 0;
			
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_CALL, f, NULL, NULL));
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_POP, symbol, NULL, NULL));

			fprintf(stderr," - assign function completed\n");
			break;
		//string
		case TTYPE_STRING:
			fprintf(stderr," - assign string\n");
			
			err = parserExpression(st, retval, &tmp);

			if(err != E_OK)
				return err;
			
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_MOV, symbol, tmp, NULL));			
			//assert(tmp->id.data && tmp->items);
			
			fprintf(stderr," - assign function completed\n");
			break;
		default:
			fprintf(stderr," - assign default: %s\n", glob_Token.data.data);

			err = parserExpression(st, retval, &tmp);

			if(err != E_OK)
				return err;
			
			listInsertEnd(&st->curr->instructions, makeInstruction(INSTRUCTION_MOV, symbol, tmp, NULL));
// 			//assert(tmp->id.data && tmp->items);
			
			fprintf(stderr," - assign default completed\n");			
			
			break;
	}	
	
	return err;	
}

ERROR parserParseCallParam(SYMBOL_TABLE_PTR st, FUNCTION_PTR f){	
	enum_RetVal retval = getToken();
	SYMBOL_PTR symbol = NULL;
	
	//Bez parametru
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	
	listEnd(&st->curr->instructions);
	
	if(retval == TTYPE_VARIABLE){
		if(!stSearchSymbol(st->curr, glob_Token.data))
			return E_SEMANTIC_UNDECLARED;
		
		symbol = stSearchSymbol(st->curr, glob_Token.data);
		
		stInsertStaticValue(st->curr, glob_Token.data, retval, &symbol);
		//fprintf(stderr, "DELL call function with symbol: %d - %s\n", symbol, symbol->id);
		
	}
	else if(
		retval == TTYPE_NUMBER ||
		retval == TTYPE_DEC_NUMBER ||
		retval == TTYPE_TRUE ||
		retval == TTYPE_FALSE || 
		retval == TTYPE_NULL ||
		retval == TTYPE_STRING
	)
		stInsertStaticValue(st->curr, glob_Token.data, retval, &symbol);		
	else
		return E_SYNTAX;

	listInsertPost(&st->curr->instructions, makeInstruction(INSTRUCTION_PUSH, symbol, NULL, NULL));
	
	if(f) f->argumentsCalled++;
	
	return parserParseCallParams(st, f);
}

ERROR parserParseCallParams(SYMBOL_TABLE_PTR st, FUNCTION_PTR f){
	enum_RetVal retval = getToken();
	SYMBOL_PTR symbol = NULL;
	
	listEnd(&st->curr->instructions);
	
	//Bez dalsich parametru
	if(retval == TTYPE_R_BRACKET)
		return E_OK;
	
	//Parametry oddelene carkou
	if(retval != TTYPE_COMMA)
		return E_SYNTAX;

	retval = getToken();
		
	if(retval == TTYPE_VARIABLE){
		if(!stSearchSymbol(st->curr, glob_Token.data))
			return E_SEMANTIC_UNDECLARED;
		
		symbol = stSearchSymbol(st->curr, glob_Token.data);
		
		stInsertStaticValue(st->curr, glob_Token.data, retval, &symbol);
		//fprintf(stderr, "DELL call function with symbol: %d - %s\n", symbol, symbol->id);
	}
	else if(
		retval == TTYPE_NUMBER ||
		retval == TTYPE_DEC_NUMBER ||		
		retval == TTYPE_TRUE ||
		retval == TTYPE_FALSE || 
		retval == TTYPE_NULL ||
		retval == TTYPE_STRING
	)
		stInsertStaticValue(st->curr, glob_Token.data, retval, &symbol);	
	else
		return E_SYNTAX;		
		
	listInsertPost(&st->curr->instructions, makeInstruction(INSTRUCTION_PUSH, symbol, NULL, NULL));
		
	if(f) f->argumentsCalled++;
		
	return parserParseCallParams(st, f);
}