#include "memory.h"

//++++++++++++++++++++++++ DEFINITIONS ++++++++++++++++++++++++|
#define MEM_HASHTABLESIZE 1024 //has to be 2^n
#define MEM_MASK MEM_HASHTABLESIZE-1
#define MEM_HASHTABLESIZE_BYTES MEM_HASHTABLESIZE*sizeof(memrecord*)
#define MEM_RECORDSIZE sizeof(memrecord)
//#define MEM_DEBUG

#define prerror( str ) fprintf(stderr, str )
//------------------- DEFINITIONS ------------------|

struct __memrecord {
	int32_t size;
	int32_t id;	//Optional (id of module requesting memory space)
	struct __memrecord * next;
};

typedef struct __memrecord memrecord;

memrecord ** g_memtab=NULL; //Global memory table

void mem_destroy(){
	if(g_memtab == NULL){
	  prerror("MEM:Called mem_destroy before mem_init!!!\n");
	  exit(ERR_MEM_INTERNALERROR);
	}
	#ifdef MEM_DEBUG
	  prerror("MEM:mem_destroy()\n");
	#endif //MEM_DEBUG
	for(size_t i = 0;i<MEM_HASHTABLESIZE;i++)
	{   //vertical walkthrough
	  for(memrecord* list=g_memtab[i];list!=NULL;)
	  {   //Horizontal walkthrough
	    memrecord* aux=list;
	    list=list->next;	//Incrementation happens here
	    #ifdef MEM_DEBUG
	      fprintf(stderr,"MEM:Destroy%d: Freeing address '%d' with id '%d' size '%d'.\n",i,(int)aux,aux->id,aux->size);
	      //fflush(stderr);
	    #endif //MEM_DEBUG
	    free(aux);
	  }
	}
	free(g_memtab);
	g_memtab=NULL;
}

/*
 * @name 
 * 
 * 
 * 
 *
 * */
void mem_init()
{
	if(g_memtab != NULL){
	  #ifdef MEM_DEBUG
	    prerror("MEM:Called mem_init twice!!!\n");
	  #endif //MEM_DEBUG
	  return;
	}
	#ifdef MEM_DEBUG
	  prerror("MEM:mem_init()\n");
	#endif //MEM_DEBUG
	g_memtab=(memrecord**)malloc(MEM_HASHTABLESIZE_BYTES);
	if(g_memtab==NULL){
	  #ifdef MEM_DEBUG
	    prerror("MEM:Allocation failed!\n");
	  #endif //MEM_DEBUG
	  exit(ERR_NOT_ENOUGHT_MEMORY);
	}
	bzero((char*)g_memtab,MEM_HASHTABLESIZE_BYTES);
	atexit(mem_destroy);
}

//hash macro "Some kind of magic!"
#define hash( ptr ) ((((size_t) ptr )&((MEM_MASK)<<4))>>4)

/*
size_t hash(void * ptr){
	return (((size_t)ptr)&MEM_MASK);
}
*/

void* mem_alloc(size_t size, int32_t id)
{
	#ifdef MEM_DEBUG
	prerror("MEM:mem_alloc()\n");
	#endif //MEM_DEBUG
	if(g_memtab == NULL){
	  prerror("MEM:Called mem_alloc before mem_init!!!\n");
	  exit(ERR_MEM_INTERNALERROR);
	}
	
	void* ptr=malloc(size+MEM_RECORDSIZE);
	if(ptr==NULL){
	  #ifdef MEM_DEBUG
	    prerror("MEM:Allocation failed!\n");
	  #endif //MEM_DEBUG
	  exit(ERR_NOT_ENOUGHT_MEMORY);
	}
	
	memrecord* record=(memrecord*)ptr;
	size_t h=hash(ptr);
	record->next=g_memtab[h];
	record->id=id;
	record->size=size;
	g_memtab[h]=record;
	return (void*)(record+1);
}

void* mem_realloc(void* ptr, size_t size){
	#ifdef MEM_DEBUG
	  prerror("MEM:mem_realloc()\n");
	#endif //MEM_DEBUG
	if(ptr==NULL){
	  #ifdef MEM_DEBUG
	    prerror("MEM:mem_realloc():Got NULL argument\n");
	  #endif //MEM_DEBUG
	  exit(ERR_MEM_INTERNALERROR);
	}
	int32_t found=0;
	void* origptr=(void*)(((char*)ptr)-MEM_RECORDSIZE);
	size_t h = hash(origptr);
	memrecord** list=&(g_memtab[h]);
	memrecord* next=NULL;
	for(;(*list);list=&((*list)->next)){
	  if((*list)==origptr){
	    found=1;
	    next=((memrecord*)origptr)->next;
	    break;
	  }
	}
	if(!found){
	  #ifdef MEM_DEBUG
	    prerror("MEM:mem_realloc():ptr not allocated or free'd.\n");
	  #endif //MEM_DEBUG
	  exit(ERR_MEM_INTERNALERROR);
	}
	int32_t id=((memrecord*)origptr)->id;

	void * newptr=realloc(origptr, size+MEM_RECORDSIZE);
	if(newptr != origptr){
	  #ifdef MEM_DEBUG
	  prerror("MEM:mem_realloc():realloc got new pointer!\n");
	  #endif //MEM_DEBUG
	  if(newptr==NULL){
	    #ifdef MEM_DEBUG
	      prerror("MEM:mem_realloc():realloc failed!\n");
	    #endif //MEM_DEBUG
	    exit(ERR_NOT_ENOUGHT_MEMORY);
	  }
	  *list=next;
	  memrecord* record=(memrecord*)newptr;
	  size_t h=hash(newptr);
	  record->next=g_memtab[h];
	  record->id=id;
	  record->size=size;
	  g_memtab[h]=record;
	  return (void*)(record+1);
	}
	else{
	  #ifdef MEM_DEBUG
	    prerror("MEM:mem_realloc():realloc got old pointer!\n");
	  #endif //MEM_DEBUG
	  ((memrecord*)origptr)->size=size;
	  return (void*)(((char*)origptr)+MEM_RECORDSIZE);
	}
}

int32_t mem_free(void* ptr){
	#ifdef MEM_DEBUG
	  prerror("MEM:mem_free()\n");
	#endif //MEM_DEBUG
	void* origptr=(void*)(((char*)ptr)-MEM_RECORDSIZE);
	//find
	size_t h = hash(origptr);
	memrecord* aux = g_memtab[h];
	memrecord* auxprev = g_memtab[h];
	if(aux == NULL) 
	  return -1;
	if(aux==origptr){
	  g_memtab[h] = aux->next;
	  free(aux);
	  aux = NULL;
	  return 0;
        }
	else{
	  aux = aux->next;
	}
	while(aux != NULL)
	{
	  if(aux==origptr){
	    auxprev->next = aux->next;
	    #ifdef MEM_DEBUG
	      fprintf(stderr,"MEM:mem_free: Freeing address '%d' with id '%d' size '%d'.\n",(int)origptr,((memrecord*)origptr)->id,((memrecord*)origptr)->size);
	    #endif //MEM_DEBUG
	    free(aux);
	    return 0;
	  }
	  auxprev = aux;
	  aux = aux->next;
	}
// 	memrecord** list=&(g_memtab[h]);
// 	memrecord* bckup=*list;
// 	for(;(*list);(*list)=(*list)->next){
// 	  if((*list)==origptr){
// 	    (*list)=((memrecord*)origptr)->next;
// 	    #ifdef MEM_DEBUG
// 	      fprintf(stderr,"MEM:Destroy: Freeing address '%d' with id '%d' size '%d'.\n",(int)origptr,((memrecord*)origptr)->id,((memrecord*)origptr)->size);
// 	    #endif //MEM_DEBUG
// 	    free(origptr);
// 	    g_memtab[h]=bckup;
// 	    return 0;
// 	  }
// 	}
	#ifdef MEM_DEBUG
	  prerror("MEM:Attempted to free an address already freed or never aloocated!\n");
	#endif //MEM_DEBUG
// 	g_memtab[h]=bckup;
	return -1;
}
