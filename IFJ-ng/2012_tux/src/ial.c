#include "ial.h"

//++++++++++++++++++++++ SYMBOL TABLE ++++++++++++++++++++++|

int TSymbol_Hash(char* key)
{
	int retval = 1;
	int keylen = strlen(key);
	for (int i=0;i < keylen;i++)
	  retval += key[i];
	return (retval % HTSIZE);
}

table_symbol* TSymbol_Init()
{
	table_symbol* ptr;
	ptr = mem_alloc(sizeof(struct struct_symbol*)*HTSIZE,id_ial);
	memset(*ptr,0,sizeof(struct struct_symbol*)*HTSIZE);
	return ptr;
}

struct_symbol* TSymbol_Search(table_symbol* ptr, char* key )
{
	if(!ptr || !(*ptr))
	  return NULL;
	struct_symbol* temp = (*ptr)[TSymbol_Hash(key)];
	while(temp)
	{
	  if(!strcmp(temp->key, key))
	    return temp;
	  temp = temp->ptrnext;
	}
	return NULL;
}

int32_t TSymbol_Insert(table_symbol* ptr, char* key, Type data )
{
	size_t l;
	if(!ptr)
	  return FALSE;
	struct_symbol* temp;
	temp = TSymbol_Search(ptr,key);

	if(temp){
	    return FALSE;
//
	}
//temp->data.value.d = data.value.d;
	else
	{
	  temp = (*ptr)[TSymbol_Hash(key)];
	  struct_symbol* new = mem_alloc(sizeof(struct struct_symbol),id_ial);
	  if(new == NULL)
	    exit(-666);
	  new->key = mem_alloc(l=strlen(key)+1,id_ial);
	  if(new->key == NULL)
	    exit(-666);
	  memcpy(new->key,key,l);
        new->data = data;
	  new->ptrnext = temp;
	  (*ptr)[TSymbol_Hash(key)] = new;
	  return TRUE;
	}

}

Type* TSymbol_Read(table_symbol* ptr, char* key )
{
	if(!ptr)
	  return NULL;
	struct_symbol* temp;
	temp = TSymbol_Search(ptr,key);
	if(temp)
	  return &(temp->data);
	return NULL;
}

void TSymbol_Delete(table_symbol* ptr, char* key )
{
	if(!ptr || !(*ptr)  || !key)
	  return;

	struct_symbol* tmp = (*ptr)[TSymbol_Hash(key)];
	struct_symbol* tmp_prev = (*ptr)[TSymbol_Hash(key)];
	if(!tmp)
	  return;

	if(!strcmp(tmp->key, key))
	{
	  (*ptr)[TSymbol_Hash(key)] = tmp->ptrnext;
	  mem_free(tmp->key);
	  mem_free(tmp);
	  tmp = NULL;
	}
	else
	  tmp = tmp->ptrnext;

	while(tmp != NULL){
	  if(!strcmp(tmp->key, key))
	  {
	    tmp_prev->ptrnext = tmp->ptrnext;
	    mem_free(tmp->key);
	    mem_free(tmp);
	    return;
	  }
	  tmp_prev = tmp;
	  tmp = tmp->ptrnext;
	}
	return;
}

void TSymbol_Free(table_symbol* ptr )
{
	if(ptr == 0)
	  return;
	for(size_t i = 0; i < HTSIZE; i++)
	  for(struct_symbol* item = (*ptr)[i]; item != NULL; ){
	    struct_symbol* tmp=item;
	    item=item->ptrnext;
	    mem_free(tmp->key);
	    mem_free(tmp);
	  }
	memset(*ptr,0,sizeof(struct struct_symbol*)*HTSIZE);
}

//----------------- SYMBOL TABLE -----------------|


//++++++++++++++++++++++++ HEAP SORT ++++++++++++++++++++++++|
void swap(char array[], int32_t num1, int32_t num2)
{
	int32_t tmp = array[num2];
	array[num2] = array[num1];
	array[num1] = tmp;
}

void make_heap(s_string str,int32_t n)
{
	while(n!=0){
	  int32_t prev_n = n;
	  n = (n + n%2)/2 - 1; //ziziz nozing, moving in heap
	  if(str.input[prev_n] > str.input[n])
	    swap(str.input, prev_n, n);
	}
}

s_string Heapsort(s_string str)
{
	str.length-=1;
	//make a heap
	for (int32_t i = 0; i <str.length-1; i++)
	  make_heap(str,i);

	//now we need to sort the heap
	for (int32_t i = 0; i < str.length; i++) {
	  swap(str.input, 0, str.length-i-1);
	  for (int32_t j = 0; j <str.length-i-1; j++){
            make_heap(str,j);
	  }
	}
	str.length+=1;
	
	return str;
}
//------------------- HEAP SORT ------------------|


//++++++++++++++ KNUTH MORRIS PRATT ALGORITHM ++++++++++++++|
int32_t* Prefix(s_string s_pattern)
{
	int32_t k = -1;
	int32_t *pi;
	//pi = malloc(sizeof(int32_t)*s_pattern.size);
	pi = mem_alloc(sizeof(int32_t)*s_pattern.size,id_string);
	pi[0] = k;
	int32_t i;
	for( i=1;i<s_pattern.length;i++)
	{
	  while((k > -1) && (s_pattern.input[k+1] != s_pattern.input[i]))
	    k = pi[k];
	  if(s_pattern.input[k+1] == s_pattern.input[i])
	    k++;
	  pi[i] = k;
	}
	return pi;
}

int32_t Find_string(s_string s_target, s_string s_pattern)
{
	if(s_pattern.input[0] == '\0')
	  return 0.0;
	int32_t* pi = Prefix(s_pattern);
	int32_t k = -1;
	int32_t i = 0;

	for(i = 0; i < s_target.length; i ++)
	{
	  while(k > -1 && s_pattern.input[k+1] != s_target.input[i])
	    k = pi[k];
	  if(s_pattern.input[k+1] == s_target.input[i])
            k++;
	  if(k == s_pattern.length - 1)
	  {
            mem_free(pi);
            return i-k;
	  }
	}
	mem_free(pi);
	return -1.0;
}
//------------ KNUTH MORRIS PRATT ALGORITHM -----------|
