#include "inst_array.h"

#define ALLOCATION_COUNT 10

Pfunction_list* Pfunction_list_Init()
{
	Pfunction_list* list;
	list = mem_alloc(sizeof(struct Pfunction_list),id_list);
	if(list == NULL)
	  printf("Not enough of memory");
	list->function = NULL;
	list->func_count = 0;
	list->allocated = 0;
	return list;
}

void Pfunction_list_Free(Pfunction_list* list)
{
	if(list->function == NULL)
	  return;
	mem_free(list->function);
	mem_free(list);
}

void Pfunction_list_Insert(Pfunction_list* list,Pfunction_struct inst)
{

	if(list->func_count >= list->allocated)
	{
	  if(list->function == NULL)
	    list->function = mem_alloc(sizeof(Pfunction_struct) * (list->allocated + APPEND), id_list);
	  else
	    list->function = mem_realloc(list->function, sizeof(Pfunction_struct) * (list->allocated + APPEND) );
	  if(list->function == NULL)
	    printf("Not enough memory for reallocation");
	  list->allocated += APPEND;
	}

	list->function[list->func_count] = inst;
	list->func_count++;
}

void instruction_insert(Pfunction_list* list,int32_t func,int32_t inst,int32_t A,int32_t B,int32_t C)
{
	if(list->function[func].position == list->function[func].allocated)
	{
	  if(list->function[func].instruction == NULL)
	    list->function[func].instruction = mem_alloc(sizeof(struct_instruction) * ALLOCATION_COUNT, id_list);
	  else
	    list->function[func].instruction = mem_realloc(list->function[func].instruction, sizeof(struct_instruction) * (list->function[func].allocated + ALLOCATION_COUNT) );
	  if(list->function[func].instruction == NULL)
	    printf("Not enough memory for reallocation");
	  list->function[func].allocated += ALLOCATION_COUNT;
	}
	//so sad :( - Don't worry be happy
	list->function[list->func_count].allocated += ALLOCATION_COUNT;

	list->function[func].instruction[list->function[func].position].instruction = inst;
	list->function[func].instruction[list->function[func].position].address_1 = A;
	list->function[func].instruction[list->function[func].position].address_2 = B;
	list->function[func].instruction[list->function[func].position].address_3 = C;

	list->function[func].position++;
}


symbol_pos_table* symbol_pos_init()
{
	symbol_pos_table* table;
	table = mem_alloc(sizeof(struct symbol_pos_table),id_list);
	if(table == NULL)
	  printf("Not enough of memory");
	table->symbol = NULL;
	table->free_pos = 0;
	table->allocated = 0;
	table->more_hats = 0;
	return table;
}

void symbol_pos_insert(symbol_pos_table* table,struct_symbol_pos symb)
{
	if(table->free_pos >= table->allocated)
	{
	  if(table->symbol == NULL)
	    table->symbol = mem_alloc(sizeof(struct_symbol_pos) * (table->allocated + APPEND), id_list);
	  else
	    table->symbol = mem_realloc(table->symbol, sizeof(struct_symbol_pos) * (table->allocated + APPEND) );
	  if(table->symbol == NULL)
	    printf("Not enough memory for reallocation");
	  table->allocated += APPEND;
	}

	table->symbol[table->free_pos] = symb;
	table->free_pos++;
}

void symbol_pos_free(symbol_pos_table* table)
{
	if(table->symbol == NULL)
	  return;
	mem_free(table->symbol);
	mem_free(table);
}

int32_t symbol_pos_find(symbol_pos_table* table,char* id)
{
	for(int i=0;i<table->free_pos;i++)
	{
	  if(!strcmp(table->symbol[i].id,id))
	    return i;
	}
	return -1;
}
