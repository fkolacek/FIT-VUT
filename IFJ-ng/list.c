/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   list.c
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

#include "list.h"

void listInit(LIST_PTR list){
    list->begin = NULL;
    list->end = NULL;
    list->curr = NULL;
}

void listInsertBegin(LIST_PTR list, PTR value){
	LIST_NODE_PTR tmp = gcMalloc(sizeof(LIST_NODE));
	
	tmp->value = value;
	tmp->next = list->begin;
	
	if(!list->begin)
		list->end = tmp;
	
	list->begin = tmp;
}
    
void listInsertEnd(LIST_PTR list, PTR value){
    LIST_NODE_PTR tmp = gcMalloc(sizeof(LIST_NODE));
	
	tmp->value = value;
	tmp->next = NULL;
	
	if(!list->begin){
		list->begin = tmp;
		list->end = tmp;
	}
	else
		list->end->next = tmp;
		
	list->end = tmp;   
}

void listInsertPost(LIST_PTR list, PTR value){
    LIST_NODE_PTR tmp = gcMalloc(sizeof(LIST_NODE));
	
	tmp->value = value;
	
	if(list->curr){
		tmp->next = list->curr->next;
		list->curr->next = tmp;
		
		if(!tmp->next)
			list->end = tmp;		
	}
	else if(!list->begin){
		tmp->next = NULL;
		
		list->begin = tmp;
		list->end = tmp;		
	}
	else{
		tmp->next = list->begin;
		list->begin = tmp;		
	}		
}

void listBegin(LIST_PTR list){
    list->curr = list->begin;
}
    
void listSucc(LIST_PTR list){
    if(list->curr)
		list->curr = list->curr->next;
}

void listEnd(LIST_PTR list){
    list->curr = list->end;
}

void listFree(LIST_PTR list){
    LIST_NODE_PTR tmp;
	
	while((tmp = list->begin)){
		list->begin = tmp->next;
		
		gcFree(tmp);
	}
	
	listInit(list);
}

LIST_NODE_PTR listAt(LIST_PTR list, int num)
{
	LIST_NODE_PTR new;
	new = list->begin;
	int i;
	for(i=0;i<num;i++)
	{
		if(new->next != NULL)
		{
			new = new->next;
			return new;
		}
		else
		{
			new = NULL;
			return new;
		}
	}
	return new;
}

