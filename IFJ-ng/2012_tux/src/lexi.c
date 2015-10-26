#include "lexi.h"

#define NONCHAR 256
#define EOL 10

#define is_hexa( a ) (( a >= '0' && a <= '9') || (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'))
#define get_hexa( a ) (( a >= 9)? a - '0' : ( (a <= 'F') ? ( ( a - 'A' ) + 10 ):( ( a - 'a' ) + 10 )))

//++++++++++++++++++++++++ GLOBAL ++++++++++++++++++++++++|
int act_line = 0;
char* lexi_buffer;
FILE* lexi_file;
//------------------- GLOBAL ------------------|

enum lexi_state
{
    s_start,
    s_number,
    s_numberDot,
    s_numberE,
    s_string_state,
    s_identificator,
    s_assign,
    s_logicLesser,
    s_logicLesserEqual,
    s_logicGreater,
    s_logicGreaterEqual,
    s_logicEqual,
    s_mark,
    s_escape,
    s_mul,
    s_div,
    s_endState,
};

void Lexi_Init(FILE * file)
{
	lexi_file = file;
	lexi_buffer = (char *)mem_alloc(sizeof(char)*1000,id_lexi);
}

void Lexi_Destroy()
{
	mem_free(lexi_buffer);
}


void Free_Token(struct_token* token)
{
// 	if(token->data.string)
// 	  mem_free(token->data.string);
	if(token->type != T_Number && token->type != T_EOF && token->type != T_UndefinedState)
	  mem_free(token->data.string);
	mem_free(token);
}

/**
 * @info Return char from a file, if prev_char is set, return prev_char.
 * @param int32_t - Pointer to prev_char.
 * @return int32_t - char
 */
inline int32_t My_fgetc(int32_t *prev_char)
{
	int32_t pom_char;
	if(*prev_char == NONCHAR)
	{
	  pom_char = fgetc(lexi_file);
	  if (pom_char == '\n')
	    act_line++;
	  else if (pom_char == EOL)
	    act_line++;
	}
	else
	{
	  pom_char = *prev_char;
	  *prev_char = NONCHAR;
	}
	return pom_char;
}

/**
 * @info Compare string to get know if it is identifikator.
 * @param char* - string to compare.
 * @param struct_token* - pointer to token
 * @return int32_t - MY_FUNC if function, MY_KEY_WORD if key word and MY_RESERVER_WORD if reserved word
 */
int32_t Compare_s_string_state(char*, struct_token* token);

struct_token* Get_Next_Token()
{
	static int32_t pznak = NONCHAR;
	int32_t state = s_start; // state is s_start by default
	int32_t i = 0, line, znak;
	line=act_line;
	struct_token* token; // Create and allocate new token.
	token = mem_alloc(sizeof(struct_token),id_lexi);
	token->line = line + 1;

	do
	{
	  if(state == s_endState) // break state
	    break;

	  if(i == MAXTOKENLENGHT)
	  {
	    token->type = T_UndefinedState;
	    return token;
	  }

	  znak = My_fgetc(&pznak);
// 	  printf("ZNAK: %c\n", znak);

	  switch (state) // automat
	  {
	    //--------------- START ---------------
	    case s_start:
	      if(znak == EOF)      // ----- EOF -----
	      {
		token->type = T_EOF;
		return token;
	      }
	      else if(znak == '\n' || znak == EOL)// ----- \n -----
	      {
		token->type = T_EndLine;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(isspace(znak))// ----- MEZERA -----
		continue;
	      else if(znak == '!') // ----- ! -----
	      {
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		i++;
		znak = My_fgetc(&pznak);
		if(znak == '=')
		{
		  lexi_buffer[i] = znak;
		  lexi_buffer[i+1] = '\0';
		  token->type = T_LogicNotEqual;
		  state = s_endState;
		  break;
		}
		else
		{
		  token->type = T_UndefinedState;
		  return token;
		}
	      }
	      else if(znak == '/') // ----- / -----
	      {
		token->type = T_Div;
		state = s_div;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
	      }
	      else if(znak == '*') // ----- * -----
	      {
		token->type = T_Mul;
		state = s_mul;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
	      }
	      else if(znak == '=') // ----- = -----
	      {
		state = s_assign;
		token->type = T_Assign;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
	      }
	      else if(znak == ',') // ----- , -----
	      {
		token->type = T_Comma;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == ':') // ----- : -----
	      {
		token->type = T_Colom;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == '<') // ----- < -----
	      {
		state = s_logicLesser;
		token->type = T_LogicLesser;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
	      }
	      else if(znak == '>') // ----- > -----
	      {
		state = s_logicGreater;
		token->type = T_LogicGreater;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
	      }
	      else if(znak == '(') // ----- ( -----
	      {
		token->type = T_SimpleBracketLeft;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == ')') // ----- ) -----
	      {
		token->type = T_SimpleBracketRight;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == '[') // ----- [ -----
	      {
		token->type = T_SquareBracketLeft;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == ']') // ----- ] -----
	      {
		token->type = T_SquareBracketRight;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == '+') // ----- + -----
	      {
		token->type = T_PlusSign;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == '-') // ----- - -----
	      {
		token->type = T_MinusSign;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == '"') // ----- string s_start -----
	      {
		token->type = T_String;
		state = s_string_state;
		i--;
		break;
	      }
	      else if((znak == '_') || (znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z')) // ----- identifikator -----
	      {
		token->type = T_Id;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_identificator;
		break;
	      }
	      else if((znak >= '0' && znak <= '9')) // ----- number -----
	      {
		token->type = T_UndefinedState;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_number;
		break;
	      }
	    break;

	    //--------------- ASSIGN ---------------
	    case s_assign:
	      if(znak == '=') // ----- = -----
	      {
		token->type = T_LogicEqual;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
	      }
	      else
		state = s_endState;
	    break;

	    //--------------- STAR ---------------
	    case s_mul:
	      if(znak == '*') // ----- ** -----
	      {
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
	      }
	      else
		state = s_endState;
	    break;

	    //--------------- SLASH ---------------
	    case s_div:
	      if(znak == '*') // ----- /* -----
	      {
		char prev_znak;
		do{
		  prev_znak = znak;
		  znak = My_fgetc(&pznak);
		}while((znak != '/' || prev_znak != '*') && znak != EOF);

		state = s_start;
		i=-1;
		lexi_buffer[i+1] = '\0';

		break;
	      }
	      else if(znak == '/') // ----- // -----
	      {
		do{
		  znak = My_fgetc(&pznak);
		}while(znak != '\n' && znak != EOF);

		state = s_start;
		i=-1;
		lexi_buffer[i+1] = '\0';
		pznak = znak;
		break;
	      }
	      else
		state = s_endState;
	    break;

	    //--------------- LESSER ---------------
	    case s_logicLesser:
	      if(znak == '=') // ----- = -----
	      {
		token->type = T_LogicLesserEqual;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
	      }
	      else
		state = s_endState;
	    break;

	    //--------------- GREATER ---------------
	    case s_logicGreater:
	      if(znak == '=') // ----- = -----
	      {
		token->type = T_LogicGreaterEqual;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		state = s_endState;
	      }
	      else
		state = s_endState;
	    break;

	    //--------------- IDENTIFIKATOR ---------------
	    case s_identificator:
	      if((znak == '_') || (znak >= 'a' && znak <= 'z') || (znak >= 'A' && znak <= 'Z') || (znak >= '0' && znak <= '9')) // ----- identifikator -----
	      {
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else
	      {
		pznak = znak;
		state = s_endState;
	      }
	    break;

	    //--------------- NUMBERS ---------------
	    case s_number:
	      if((znak >= '0' && znak <= '9')) // ----- number -----
	      {
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else if(znak == '.')
	      {
		token->type = T_Number;
		state = s_numberDot;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else if(znak == 'e' || znak == 'E')
	      {
		token->type = T_Number;
		state = s_numberE;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else
	      {
		token->type = T_UndefinedState;
		return token;
	      }
	    break;

	    //--------------- NUMBERS DOT ---------------
	    case s_numberDot:
	      if((znak >= '0' && znak <= '9')) // ----- number -----
	      {
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else if(znak == '.')
	      {
		token->type = T_UndefinedState;
		break;
	      }
	      else if(znak == 'e' || znak == 'E')
	      {
		state = s_numberE;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else
	      {
		pznak = znak;
		state = s_endState;
	      }
	    break;

	    //--------------- NUMBERS E ---------------
	    case s_numberE:
	      if((znak >= '0' && znak <= '9')) // ----- number -----
	      {
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else if(znak == '.')
	      {
		token->type = T_UndefinedState;
		break;
	      }
	      else if(znak == '+' || znak == '-')
	      {
		if(lexi_buffer[i-1] == '+' || lexi_buffer[i-1] == '-')
		{
		  token->type = T_UndefinedState;
		  break;
		}
		state = s_numberE;
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else
	      {
		pznak = znak;
		state = s_endState;
	      }
	    break;

	    //--------------- STRING ---------------
	    case s_string_state:
	      if(znak == '\n') // ----- \n -----
	      {
		token->type = T_UndefinedState;
		return token;
	      }
	      else if(znak == '"') // ----- " -----
	      {
		if(i == 0)
		  lexi_buffer[i] = '\0';
		state = s_endState;
		break;
	      }
	      else if(znak == '\\') // ----- \ -----
	      {
		state = s_escape;
		i--;
		break;
	      }
	      else
	      {
		lexi_buffer[i] = znak;
		lexi_buffer[i+1] = '\0';
	      }
	    break;

	    //--------------- ESCAPE ---------------
	    case s_escape:
	      if(znak == 'n') // ----- \n -----
	      {
		state = s_string_state;
		lexi_buffer[i] = '\n';
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else if(znak == 't') // ----- \t -----
	      {
		state = s_string_state;
		lexi_buffer[i] = '\t';
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else if(znak == '"') // ----- \" -----
	      {
		state = s_string_state;
		lexi_buffer[i] = '"';
		lexi_buffer[i+1] = '\0';
		break;
	      }
	      else if(znak == 'x') // ----- \x00 -----
	      {
		state = s_string_state;

		int32_t pom1 = My_fgetc(&pznak);
		int32_t pom2 = My_fgetc(&pznak);

		if(!is_hexa(pom1) || !is_hexa(pom2))// bad escape sequence in string
		{
		  token->type = T_UndefinedState;
		  return token;
		}

		pom1 = get_hexa(pom1);
		pom2 = get_hexa(pom2);
		pom1 *= 16;

		pom1+= pom2;

		lexi_buffer[i] = pom1;
		lexi_buffer[i+1] = '\0';
		break;
	      }
	    break;
	  }

	  i++; // increase position counter
	}while(TRUE); // loop. Breake with automat!
	// ----- END WHILE -----

	if(token->type == T_UndefinedState)
	  return token;

	if(token->type != T_Number)
	{
	  // ----- make token string -----
	  token->data.string = mem_alloc(sizeof(char)*(i+2),id_lexi);
	  memcpy(token->data.string, lexi_buffer,i+2);
	  token->data.string[i+1] = '\0';
	  token->line = line + 1;
	  // ----- make token string -----
	}
	else
	{
	  // ----- make number -----
	  token->data.num = strtod(lexi_buffer,NULL);
	  // ----- make number -----
	}


	// ----- change type if needed -----
	if(token->type == T_Id)
	{
	  if (Compare_s_string_state(lexi_buffer,token) == MY_KEY_WORD)
	    {}
	  else if(Compare_s_string_state(lexi_buffer,token) == MY_RESERVER_WORD)
	    token->type = T_Reserved_Word;
	  else if(Compare_s_string_state(lexi_buffer,token) == MY_FUNC)
	    {}
	}
	else if(state == s_number) // we cant have number in format 666 , it have to be 666.0
	  token->type = T_UndefinedState;
	return token;
}

int32_t Compare_s_string_state(char* str, struct_token* token){
	if(strcmp(str,"else") == 0){token->type = T_Else; return MY_KEY_WORD;}
	else if(strcmp(str,"end") == 0){token->type = T_End; return MY_KEY_WORD;}
	else if(strcmp(str,"false") == 0){token->type = T_False; return MY_KEY_WORD;}
	else if(strcmp(str,"function") == 0){token->type = T_Function; return MY_KEY_WORD;}
	else if(strcmp(str,"if") == 0){token->type = T_If; return MY_KEY_WORD;}
	else if(strcmp(str,"nil") == 0){token->type = T_Nil; return MY_KEY_WORD;}
	else if(strcmp(str,"return") == 0){token->type = T_Return; return MY_KEY_WORD;}
	else if(strcmp(str,"true") == 0){token->type = T_True; return MY_KEY_WORD;}
	else if(strcmp(str,"while") == 0){token->type = T_While; return MY_KEY_WORD;}
	else if(strcmp(str,"as") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"def") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"directive") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"export") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"from") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"import") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"launch") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"load") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"macro") == 0) return MY_RESERVER_WORD;
	else if(strcmp(str,"print") == 0){token->type = T_Print; return MY_FUNC;}
	else if(strcmp(str,"input") == 0){token->type = T_Input; return MY_FUNC;}
	else if(strcmp(str,"typeOf") == 0){token->type = T_Typeof; return MY_FUNC;}
	else if(strcmp(str,"len") == 0){token->type = T_Len; return MY_FUNC;}
	else if(strcmp(str,"find") == 0){token->type = T_Find; return MY_FUNC;}
	else if(strcmp(str,"sort") == 0){token->type = T_Sort; return MY_FUNC;}
	else if(strcmp(str,"numeric") == 0){token->type = T_Numeric; return MY_FUNC;}
	return 0;
}
