#include "interpret.h"



/*
#define data_free(data) \
{\
	if((data)->type == DATA_STRING) \
	{\
	  mem_free((data)->data.string->input); \
	  mem_free((data)->data.string); \
	}\
	(data)->type = DATA_NULL; \
}
*/
#define data_copy(data_src, data_dst) \
{\
	if((data_dst).type == DATA_STRING) \
	{\
	  mem_free((data_dst).data.string.input); \
	}\
	if((data_src).type == DATA_NUM)\
	{\
	  ((data_dst).type) = DATA_NUM; \
	  ((data_dst).data.num) = ((data_src).data.num); \
	}\
	else if((data_src).type == DATA_NULL)\
	{\
	  ((data_dst).type) = ((data_src).type); \
	}\
	else \
	{\
	  (data_dst).data.string = *((s_string*)mem_alloc(sizeof(struct s_string),id_interpret));\
	  (data_dst).data.string.input = mem_alloc((data_src).data.string.size,id_interpret);\
	  memcpy((data_dst).data.string.input, (data_src).data.string.input, (data_src).data.string.length);\
	  (data_dst).data.string.length = (data_src).data.string.length;\
	  (data_dst).type = DATA_STRING;\
	}\
}



// +++++++++++++++++ STRUCTURES +++++++++++++++++

typedef struct struct_act_func
{
	int32_t pc;
	int32_t cf;
	t_var* registr;
	int32_t parent;
	int32_t ret_index;
}struct_act_func;

typedef struct struct_stack
{
	int32_t s_top;
	int32_t s_free;
	struct_act_func* data;
}struct_stack;

// -------------- STRUCTURES --------------


struct_stack* stack_Init()
{
	struct_stack* stack;
	stack = mem_alloc(sizeof(struct struct_stack)*MAX_STACK_SIZE,id_interpret);
	if(stack == NULL)
	  printf("kurva");
	stack->s_free = MAX_STACK_SIZE;
	stack->s_top = -1;
	return stack;
}

void stack_push(struct_stack* stack,struct_act_func uni)
{
	if(stack->s_free == 0)
	  printf("bum kurva bum");
	stack->s_top++;
	stack->s_free--;
	stack->data[stack->s_top] = uni;
}

struct_act_func stack_pop(struct_stack* stack)
{
	struct_act_func uni;
	uni = stack->data[stack->s_top];
	stack->s_top--;
	stack->s_free++;
	return uni;
}

void stack_free(struct_stack* stack)
{
	while(stack->s_top >= 0)
	{
	  stack_pop(stack);
	}
	mem_free(stack);
}





int interpretation(Pfunction_list* f_list,konst_list* konst)
{
	if(f_list->func_count == 0)
	  return 0;

	t_var v_pom; // variable for nil value
	t_var v_pom2;
	t_var v_pom3;
	char buffer[256];
	int32_t i_pom;
	int32_t i_pom2;
	int32_t i_pom3;

	struct_stack* stack = stack_Init();

	struct_act_func act_func;
	//act_func = mem_alloc(sizeof(struct struct_act_func),id_interpret);

	act_func.pc = 0;
	//act_func.cf = f_list->func_count-1;
	act_func.cf = 0;
	act_func.registr = mem_alloc(sizeof(struct t_var) * f_list->function[act_func.cf].reg_count,id_interpret);
	for(int i=0;i<f_list->function[act_func.cf].reg_count;i++)
	  act_func.registr[i].type = DATA_NULL;
	act_func.parent = -1;
	act_func.ret_index = 0;


	while(1) //infinite loop
	{
	  struct_instruction instruction;
	  if(f_list->function[act_func.cf].instruction == NULL)
	    return 0;
	  instruction = f_list->function[act_func.cf].instruction[act_func.pc];
	  switch(instruction.instruction)
	  {
	    case I_MOVE:
	      data_copy(act_func.registr[instruction.address_2], act_func.registr[instruction.address_1]);
	    break;
	    case I_LOADK:
	      data_copy(konst->konst[instruction.address_2], act_func.registr[instruction.address_1]);
	    break;
	    case I_LOADNIL:
	      v_pom.type = DATA_NULL;
	      data_copy(v_pom, act_func.registr[instruction.address_1]);
	      data_copy(v_pom, act_func.registr[instruction.address_2]);
	    break;
	    case I_ADD:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];
		
		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    v_pom2.type = DATA_NUM;
		    v_pom2.data.num = v_pom2.data.num + v_pom3.data.num;
		    data_copy(v_pom2, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_STRING))
		{
		    v_pom2.data.string = String_concatenate (v_pom2.data.string, v_pom3.data.string);
		    data_copy(v_pom2, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_NUM))
		{
		    v_pom.type = DATA_STRING;
		    sprintf(buffer, "%g", v_pom3.data.num);
		    v_pom.data.string = String_insert(buffer);

		    v_pom2.data.string = String_concatenate (v_pom2.data.string, v_pom.data.string);
		    data_copy(v_pom2, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_STRING))
		{
		    v_pom.type = DATA_STRING;
		    sprintf(buffer, "%g", v_pom2.data.num);
		    v_pom.data.string = String_insert(buffer);

		    v_pom2.data.string = String_concatenate(v_pom.data.string,v_pom3.data.string);
		    data_copy(v_pom2, act_func.registr[instruction.address_1]);
		}
		else
		{
		   // exit(-600);
		}
	    break;
	    case I_SUB:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];

		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    v_pom2.data.num = v_pom2.data.num - v_pom3.data.num;
		    data_copy(v_pom2, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_MUL:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];


		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    v_pom2.data.num = v_pom2.data.num * v_pom3.data.num;
		    data_copy(v_pom2, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_NUM) || (v_pom2.type == DATA_NUM && v_pom3.type == DATA_STRING))
		{
		  if(v_pom2.type == DATA_STRING)
		  {
		    v_pom = v_pom2; // prehodime si v_pom tak abychom meli v v_pom2
		    v_pom2 = v_pom3; // vzdy cislo.
		    v_pom3 = v_pom;
		  }

		  v_pom.type = DATA_STRING;


		  if(v_pom2.data.num == 0)
		    v_pom.data.string = String_insert("");
		  else
		  {
		    v_pom.data.string = String_insert(v_pom3.data.string.input);

		    for(int i=1;i<v_pom2.data.num;i++)
		    {
		      v_pom.data.string = String_concatenate(v_pom.data.string, v_pom3.data.string);
		    }
		  }
		  data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_DIV:
	      if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];


		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    if(v_pom3.data.num == 0)
		      exit(-666);
		    v_pom2.data.num = v_pom2.data.num / v_pom3.data.num;
		    data_copy(v_pom2, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_CONCAT:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];


		if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_STRING))
		{
		  v_pom.type = DATA_STRING;

		  v_pom.data.string = String_concatenate(v_pom2.data.string, v_pom3.data.string);

		  data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_LE:
	      if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];

		v_pom.type = DATA_BOOL;

		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    if(v_pom2.data.num < v_pom3.data.num)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_STRING))
		{
		  if(v_pom2.data.string.length < v_pom3.data.string.length)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}

	    break;
	    case I_GR:
	      if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];

		v_pom.type = DATA_BOOL;

		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    if(v_pom2.data.num > v_pom3.data.num)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_STRING))
		{
		  if(v_pom2.data.string.length > v_pom3.data.string.length)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_EL:
	      if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];

		v_pom.type = DATA_BOOL;

		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    if(v_pom2.data.num <= v_pom3.data.num)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_STRING))
		{
		  if(v_pom2.data.string.length <= v_pom3.data.string.length)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_EG:
	      if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];

		v_pom.type = DATA_BOOL;

		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    if(v_pom2.data.num >= v_pom3.data.num)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_STRING))
		{
		  if(v_pom2.data.string.length >= v_pom3.data.string.length)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_EQ:
	      if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];

		v_pom.type = DATA_BOOL;

		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    if(v_pom2.data.num == v_pom3.data.num)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_STRING))
		{
		  if(v_pom2.data.string.length == v_pom3.data.string.length)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_NEQ:
	      if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];

		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 =konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];

		v_pom.type = DATA_BOOL;

		if((v_pom2.type == DATA_NUM && v_pom3.type == DATA_NUM))
		{
		    if(v_pom2.data.num != v_pom3.data.num)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else if((v_pom2.type == DATA_STRING && v_pom3.type == DATA_STRING))
		{
		  if(v_pom2.data.string.length != v_pom3.data.string.length)
		      v_pom.data.num = 1;
		    else
		      v_pom.data.num = -1;
		    data_copy(v_pom, act_func.registr[instruction.address_1]);
		}
		else
		{
		    exit(-600);
		}
	    break;
	    case I_JMP:
	      act_func.pc= instruction.address_2-1;
	    break;
	    case I_JT:
		if(instruction.address_1 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_1-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_1];

		if(v_pom2.type == DATA_BOOL)
		  if(v_pom2.data.num > 0)
		    act_func.pc= instruction.address_2-1;
	    break;
	    case I_JF:
	      if(instruction.address_1 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_1-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_1];

		if(v_pom2.type == DATA_BOOL)
		  if(v_pom2.data.num < 0)
		    act_func.pc= instruction.address_2-1;
	    break;
	    case I_CALL:
		i_pom = instruction.address_1;
		i_pom2 = instruction.address_2;
		i_pom3 = instruction.address_3;

		stack_push(stack, act_func);

		act_func.pc = 0;
		act_func.cf = i_pom3;
		act_func.registr = mem_alloc(sizeof(struct t_var) * f_list->function[act_func.cf].reg_count,id_interpret);
		for(int i=0;i<f_list->function[act_func.cf].reg_count;i++)
		  act_func.registr[i].type = DATA_NULL;
		act_func.parent = stack->data[stack->s_top].cf;
		act_func.ret_index = i_pom;

		for(int i=0; i < i_pom2; i++)
		{
		  data_copy(stack->data[stack->s_top].registr[i_pom+1], act_func.registr[i]);
		}
	    break;
	    case I_RETURN:
		if(instruction.address_1 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_1-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_1];

		data_copy(v_pom2,stack->data[stack->s_top].registr[act_func.ret_index]);

		mem_free(act_func.registr);

		act_func = stack_pop(stack);
	    break;

	    case I_INPUT:
		v_pom.type = DATA_STRING;
		v_pom.data.string = Input();
		data_copy(v_pom,act_func.registr[instruction.address_1]);
	    break;
	    
	    case I_PRINT:
		
		for(int i=0;i<instruction.address_3;i++)
		{
		  v_pom2 = act_func.registr[instruction.address_2+i];
		  if(v_pom2.type == DATA_NUM)
		    printf("%g",v_pom2.data.num);
		  else if(v_pom2.type == DATA_NUM)
		    printf("%s",v_pom2.data.string.input);
		  else if(v_pom2.type == DATA_NUM)
		    printf("Nil");
		  else if(v_pom2.type == DATA_BOOL && v_pom2.data.num < 0)
		    printf("false");
		  else if(v_pom2.type == DATA_NUM && v_pom2.data.num > 0)
		    printf("true");
		}
		
		v_pom.type = DATA_NULL;
		data_copy(v_pom,act_func.registr[instruction.address_1]);
	    break;

	    case I_LEN:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];
		
		v_pom.type = DATA_NUM;
		if(v_pom2.type == DATA_STRING)
		{
		  v_pom.data.num = v_pom2.data.string.length;
		}
		else
		  v_pom.data.num = 0.0;
		
		data_copy(v_pom,act_func.registr[instruction.address_1]);
	    break;
	    
	    case I_TYPEOF:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];
		
		v_pom.type = DATA_NUM;
		
		  if(v_pom2.type == DATA_NUM)
		    v_pom.data.num = 3.0;
		  else if(v_pom2.type == DATA_STRING)
		    v_pom.data.num = 8.0;
		  else if(v_pom2.type == DATA_NULL)
		    v_pom.data.num = 0.0;
		  else if(v_pom2.type == DATA_BOOL)
		    v_pom.data.num = 1.0;
		  else if(v_pom2.type == DATA_FUNC)
		    v_pom.data.num = 6.0;
		
		data_copy(v_pom,act_func.registr[instruction.address_1]);
	    break;
	    
	    case I_FIND:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];
		if(instruction.address_3 >= MAX_STACK_SIZE)
		  v_pom3 = konst->konst[instruction.address_3-MAX_STACK_SIZE];
		else
		  v_pom3 = act_func.registr[instruction.address_3];
		
		if(v_pom2.type != DATA_STRING && v_pom2.type != DATA_STRING)
		  exit(11);
		
		v_pom.type = DATA_NUM;
		v_pom.data.num = (double)Find_string(v_pom2.data.string, v_pom3.data.string);
		
		
		data_copy(v_pom,act_func.registr[instruction.address_1]);
	    break;
	    
	    case I_SORT:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];
		
		if(v_pom2.type != DATA_STRING)
		  exit(11);
		
		v_pom.type = DATA_STRING;
		v_pom.data.string = Heapsort(v_pom2.data.string);
			
		data_copy(v_pom,act_func.registr[instruction.address_1]);
	    break;
	    
	    case I_NUMERIC:
		if(instruction.address_2 >= MAX_STACK_SIZE)
		  v_pom2 = konst->konst[instruction.address_2-MAX_STACK_SIZE];
		else
		  v_pom2 = act_func.registr[instruction.address_2];
		
		if(v_pom2.type == DATA_BOOL || v_pom2.type == DATA_NULL)
		  exit(12);
				
		v_pom.type = DATA_NUM;
		if(v_pom2.type == DATA_NUM)
		  v_pom.data.num = v_pom2.data.num;
		else
		  v_pom.data.num = MyNumeric(v_pom2.data.string);
			
		data_copy(v_pom,act_func.registr[instruction.address_1]);
	    break;
	    
	    case I_STOP:
		mem_free(act_func.registr);
		stack_free(stack);
		return 0;
	    break;
	  }
	  act_func.pc++;
	}

	stack_free(stack);
	return 0;
}
