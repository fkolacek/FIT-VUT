/**
 * Predmet:  IFJ / IAL
 * Projekt:  Implementace interpretu jazyka PHP13
 * Varianta: a/1/I
 * Soubor:   gc.c
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

#include "gc.h"

GC_ITEM_PTR root;
GC_ITEM_PTR curr;

void gcInit(){

   signal(SIGINT, gcAbort);
    signal(SIGABRT, gcAbort);

    root = NULL;
}

PTR gcMalloc(int size){

        if(root == NULL)
        {
            root = malloc(sizeof(struct GC_ITEM));
            root->type = GC_ITEM_MEMORY;

        //    printf("root malloc struct: %d\n",root);

            if(!root)
                gcAbort();

            root->block = malloc(size);
          //  printf("malloc block: %d\n",root->block);

            if(!root->block)
                gcAbort();

            root->next = NULL;
            curr = root;


            return root->block;
        }
        else
        {
            GC_ITEM_PTR node;
            node = malloc(sizeof(struct GC_ITEM));

            node->type = GC_ITEM_MEMORY;
            //printf("malloc struct: %d\n",node);

            if(!node)
                gcAbort();

            node->block = malloc(size);

           // printf("malloc block: %d\n",node->block);

            if(!node->block)
                gcAbort();

            node->next = NULL;

            curr->next = node;
            //printf("next address: %d\n",curr->next);
            curr = node;

            return node->block;
        }
}

PTR gcRealloc(PTR block, int size){

    GC_ITEM_PTR temp;
    if(root->block == block)
    {
        root->block = realloc(block,size);
        return root->block;
    }
    else
    {
        temp = root;
        while(temp != NULL)
        {
            if(temp->block == block)
            {
                temp->block = realloc(block,size);
                return temp->block;
            }
            temp = temp->next;
        }
    }
    return NULL;
}

PTR gcCalloc(int count, int size){

        if(root == NULL)
        {
            root = malloc(sizeof(struct GC_ITEM));
            //printf("calloc structure: %d\n",root);
            root->type = GC_ITEM_MEMORY;
            if(!root)
                gcAbort();


            root->block = calloc(count,size);
            //printf("calloc block: %d\n",root->block);
            if(!root->block)
                gcAbort();

            root->next = NULL;
            curr = root;


            return root->block;
        }
        else
        {
            GC_ITEM_PTR node;
            node = malloc(sizeof(struct GC_ITEM));
            node->type = GC_ITEM_MEMORY;

            if(!node)
                gcAbort();


            node->block = calloc(count,size);

            if(!node->block)
                gcAbort();


            node->next = NULL;

            curr->next = node;
            curr = node;

            return node->block;
        }
}

PTR gcFopen(const char* fileName, const char* mode){

        if(root == NULL)
        {
            root = malloc(sizeof(struct GC_ITEM));

            root->type = GC_ITEM_FILE;
           // printf("root malooc! %d\n",root);

            if(!root)
            {
                gcAbort();
            }

            root->block = fopen(fileName, mode);

//            printf("root block malooc! %d\n",root->block);

            if(!root->block)
            {
              //  printf("no block allocated!\n");
                gcAbort();
            }
            root->next = NULL;
            curr = root;

            return root->block;
        }
        else
        {
            GC_ITEM_PTR node;
            node = malloc(sizeof(struct GC_ITEM));
         //   printf("file alloc: %d\n",node);

            node->type = GC_ITEM_FILE;

            if(!node)
            {
              //  printf("abort!\n");
                gcAbort();
            }

            node->block = fopen(fileName, mode);
         //   printf("file block alloc: %d\n",node->block);
            if(!node->block)
            {
              //  printf("abort!\n");
                gcAbort();
            }

            node->next = NULL;

            curr->next = node;
            //printf("next address: %d\n",curr->next);
            curr = node;

            return node->block;
        }
}

void gcFree(PTR block){
       // printf("root block: %d\n",root->block);
        if(!block)
                return;

        GC_ITEM_PTR temp;
        GC_ITEM_PTR temp2;

        if(block == root->block)
        {

            temp = root;
            root = temp->next;

			if(temp->block != NULL)
				free(temp->block);
            free(temp);

        }
        else
        {
            temp = root;
            while(temp->next != NULL)
            {
                    if(temp->next->block == block)
                    {
                        temp2 = temp->next;
                        if(temp->next->next == NULL)
                        {
                        //    printf("pointing to last: %d\n",temp);
                            temp->next = NULL;
                            curr = temp;

							if(temp2->block != NULL)
								free(temp2->block);
                            free(temp2);
                            return;
                        }
                        else
                        {
                            //curr = temp;
                            temp->next = temp2->next;

							if(temp2->block != NULL)
								free(temp2->block);
                            free(temp2);
                            return;
                        }

                    }
                    else
                    {
                        temp = temp->next;
                    }
            }
        }
}

void gcFclose(PTR block){

        if(!block)
                return;

        GC_ITEM_PTR temp;
        GC_ITEM_PTR temp2;

        if(block == root->block)
        {

            temp = root;
            root = temp->next;

            fclose(temp->block);
            free(temp);
        }
        else
        {
            temp = root;
            while(temp->next != NULL)
            {
                    if(temp->next->block == block)
                    {
                        temp2 = temp->next;
                        if(temp->next->next == NULL)
                        {
                        //    printf("pointing to last: %d\n",temp);
                            temp->next = NULL;
                            curr = temp;

                            fclose(temp2->block);
                            free(temp2);
                            return;
                        }
                        else
                        {
                            temp->next = temp2->next;

                            fclose(temp2->block);
                            free(temp2);
                            return;
                        }
                    }
                    else
                    {
                        temp = temp->next;
                    }
            }
        }
}

void gcDispose(){           // global free

    GC_ITEM_PTR temp;
    while(root != NULL)
    {
        temp = root;
        root = temp->next;
        if(temp->type == GC_ITEM_MEMORY)
		{
			if(temp->block != NULL)
				free(temp->block);
		}
		else
			if(temp->block != NULL)
				fclose(temp->block);
        free(temp);
    }
}

void gcAbort(){
    gcDispose();
    exit(E_COMPILATOR);
}


void printList()
{
    while(root!=NULL)
        root = root->next;
}
