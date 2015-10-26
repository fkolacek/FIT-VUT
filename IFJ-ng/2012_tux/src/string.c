#include "string.h"

int32_t String_length(char* s_target)
{
	return s_target==NULL?-1:(s_target[0]=='\0'?1:((int32_t)strlen(s_target)+1));
}

s_string Input()
{
    s_string s_input;
    int32_t c;
    char* i_temp_ptr;
    int32_t i_buffer = 64;
    int32_t i = 0;
    i_temp_ptr = mem_alloc(sizeof(char)*i_buffer,id_string);
    printf("input fce! ---------------- \n");
    while((c = getchar()) != '\n')
    {
        if( i == i_buffer - 2)
        {
            i_buffer*=2;
            i_temp_ptr = mem_realloc(i_temp_ptr,sizeof(char)*i_buffer);
        }
        i_temp_ptr[i] = (char)c;
        i++;
		i_temp_ptr[i] = '\0';
    }

    s_input.input = i_temp_ptr;

    s_input.length = String_length(s_input.input);
    s_input.size = i_buffer;

    return s_input;
}

double MyNumeric(s_string s_target)
{
    double temp;
    char* end_ptr;
    temp = strtod(s_target.input,&end_ptr);

    if (temp == HUGE_VAL || temp == -HUGE_VAL)
    {
        return 12;
    }

    return temp;
}

s_string String_insert(char* word)
{
    s_string s_temp;
    s_temp.input = mem_alloc(sizeof(char)*String_length(word),id_string);
    memcpy(s_temp.input,word,sizeof(char)*String_length(word));
    s_temp.length = String_length(s_temp.input);
    s_temp.size = s_temp.length;
    return s_temp;
}

s_string String_concatenate(s_string s1, s_string s2)
{
    s_string s_temp;
    s_temp.input = mem_alloc(sizeof(char)*(s1.length+s2.length),id_string);
    s_temp.length = s1.length+s2.length-1;
    s_temp.size = s_temp.length;

    memcpy(s_temp.input,s1.input,s1.length);
	memcpy(s_temp.input+s1.length-1,s2.input,s2.length);
    return s_temp;
}

int32_t String_init(s_string target)
{
    target.input = mem_alloc(sizeof(char)*STRING_INC,id_string);
    if(target.input == NULL)
      return 1;
    target.input[0] = '\0';
    target.length = 1;
    target.size = STRING_INC;
    return 0;
}

int32_t String_equal(s_string s1, s_string s2)
{
    return !strcmp(s1.input, s2.input);
}
