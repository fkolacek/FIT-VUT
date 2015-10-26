#include "tkonst.h"


konst_list* konst_list_Init()
{
	konst_list* list;
	list = mem_alloc(sizeof(struct konst_list),id_list);
	if(list == NULL)
	  printf("Not enough of memory");
    list->konst = mem_alloc(sizeof(t_var)*TINITSIZE,id_list);
	//list->konst = NULL;
	list->count = 0;
	list->allocated = TINITSIZE;
	return list;
}

void konst_list_Free(konst_list* list)
{
	if(list->konst == NULL)
	  return;
	mem_free(list->konst);
	mem_free(list);
}

int32_t konst_list_Insert(konst_list* list,t_var var)
{
//	if(sizeof(var) >= (unsigned)list->allocated)
//	{
//	  if(list->konst == NULL)
//	    list->konst = mem_alloc(sizeof(var)*2, id_list);
//	  else
//	  {
//        list->konst = mem_realloc(list->konst, sizeof(var)*2 + list->allocated);
//	  }
//	  if(list->konst == NULL)
//	    printf("Not enough memory for reallocation");
//	  list->allocated += sizeof(var);
//	}
    if(list->count == list->allocated)
    {
        list->allocated*=2;
        list->konst = realloc(list->konst,sizeof(t_var)*list->allocated);
    }

	list->konst[list->count] = var;
	list->count++;

	return list->count-1;
}
