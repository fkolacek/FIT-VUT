/*
 * XHERMA25 - Lukas Hermann		25	%
 * XSTODU06 - Dan Stodulka		25	%
 * XTRANH00 - Tran Manh Hoang		25	%
 * XMARCE02 - Andrea Marcelyova	25	%
 */
#include "global.h"
#include "ial.h"
#include "lexi.h"
#include "interpret.h"
#include "inst_array.h"
#include "tkonst.h"
#include "parser.h"

int32_t main(int argc, char** argv)
{
	FILE *file;
	mem_init(); // do not fucking remove ziz! Initialization of memmory management


	if (argc == 1)
	{
	  fprintf(stderr, "Please specify input file.\n");
	  return -1;
	}
	if (!(file = fopen(argv[1], "r")))
	{
	  fprintf(stderr, "File cannot be opened.\n");
	  return -1;
	}

	Lexi_Init(file); // nitialization of lexical analyzator, and sending file in argument.


//	f_list->func_count = 1;
//	f_list->function = mem_alloc(sizeof(struct Pfunction_struct),id_main);
//	f_list->function[0].reg_count = 2;
//	f_list->function[0].instruction = mem_alloc(sizeof(struct struct_instruction) * 2,id_main);
//	f_list->function[0].instruction[0].instruction = I_MUL;
//	f_list->function[0].instruction[0].address_1 = 0;
//	f_list->function[0].instruction[0].address_2 = 0+MAX_STACK_SIZE;
//	f_list->function[0].instruction[0].address_3 = 1+MAX_STACK_SIZE;
//
//	f_list->function[0].instruction[1].instruction = I_STOP;



	// ------ konst list test --------
//	t_var* pom;
//	pom = mem_alloc(sizeof(t_var),id_main);
//	pom->type = DATA_NUM;
//	pom->data.num = 2.000;
//
//	t_var* pom2;
//	pom2 = mem_alloc(sizeof(t_var),id_main);
//	pom2->type = DATA_STRING;
//	pom2->data.string = String_insert("ahoj blbe");
//
//	konst_list_Insert(konst,*pom);
//	konst_list_Insert(konst,*pom2);
//
//	printf("BLABLA: %f\n",konst->konst[0].data.num);
//	printf("BLABLA: %s\n",konst->konst[1].data.string.input);
//
//	mem_free(pom);
//	mem_free(pom2);
//
//	printf("BLABLA: %f\n",konst->konst[0].data.num);
//	printf("BLABLA: %s\n",konst->konst[1].data.string.input);
//	// ------ konst list test --------
//

//
//	printf("RETURN: %d\n",prd);



// 	s_string jedna,dve,tri;
//
// 	jedna = String_insert("ahoj");
//
// 	dve = String_insert("dva");
//
// 	tri = String_insert("tri");
//
// 	tri = String_concatenate(jedna, dve);
// 	printf("\nHOOVNO: %s\n\n",tri);
// 	printf("\nHOOVNO: %s\n\n",jedna);
// 	dve = String_concatenate(tri,jedna);
//
//
// 	printf("\nHOOVNO: %s\n\n",dve);


//	 -------------- MEMTEST ------------------
// 	void* pole[1200];
//
// 	char lol[]="sdfsdafsadfsadfasd";
// 	int32_t lal=strlen(lol)+1;
// 	char *m = (char *)mem_alloc(lal,1);
//
// 	memcpy(m,lol,lal);
// 	printf("%s\n",lol);
// 	printf("%s\n",m);
// 	for(int32_t i=2;i<1200;i++){
// 		pole[i]=mem_alloc(lal,i);
// 	}
//
// 	for(int32_t i=2;i<100;i++){
// 		mem_free(pole[i]);
// 	}
// 	for(int32_t i=200;i<500;i++){
// 		mem_free(pole[i]);
// 	}
// 	for(int32_t i=999;i>800;i--){
// 		mem_free(pole[i]);
// 	}
//
// 	char* l=(char*)mem_realloc(m,lal*2);
// 	l=(char*)mem_realloc(l,lal*2*4);
// 	printf("%s\n",l);
// 	memcpy(l+lal,l,lal);
// 	l[lal-1]='#';
// 	printf("%s\n",l);
//
// 	printf("=======================================\n");
// 	s_string str = String_insert("I've got a bad feeling about this!");
// 	printf("%s\n",str.input);
// 	Heapsort(str);
// 	printf("%s\n",str.input);
//
//
// 	for(int32_t i=1200;i<1423;i++){
// 		mem_free(mem_alloc(lal,i));
// 	}
	// -------------- MEMTEST ------------------



	// -------------- LEXITEST ------------------
// 	struct_token* token;
// 	do
// 	{
// 	  token = Get_Next_Token();
// 	  if(token->type == T_EOF)
// 	  {
// 	    Free_Token(token);
// 	    break;
// 	  }
// 	  if(token->type == T_UndefinedState)
// 	    printf("token: ERROR\n");
// 	  else if(token->type != T_Number)
// 	    printf("line: %d \ttoken type: %d \ttoken: %s\n",token->line, token->type,token->data.string);
// 	  else
// 	    printf("line: %d \ttoken type: %d \ttoken: %f\n",token->line, token->type, token->data.num);
// 	  Free_Token(token);
// 	}
// 	while(1);
	// -------------- LEXITEST ------------------

	// -------------- JOHNYTEST ------------------
// 	char* klic;
// 	klic = "ahoj";
// 	t_universal dataa2;
// 	dataa2.data.string = String_insert("Cauko");
// 	dataa2.type = DATA_STRING;
//
// 	char* klic2;
// 	klic2 = "Nasrat";
// 	TSymbol_Insert(symbol_table,klic2,dataa2);
// 	t_universal *po = TSymbol_Read(symbol_table,klic2);
//       // moje = TSymbol_Read(symbol_table,klic);
//
// 	printf("valTyp %d value : %s\n",po->type, po->data.string.input);
//
// 	t_universal data_pom;
// 	data_pom.data.num = 5555;
// 	data_pom.type = DATA_NUM;
// 	t_universal data_pom2;
// 	data_pom2.data.string = String_insert("Cauko");
// 	data_pom2.type = DATA_NUM;
// 	char* klic3;
// 	klic3 = "vole";
//
// 	TSymbol_Insert(symbol_table,klic3,data_pom);
// 	t_universal* moje = TSymbol_Read(symbol_table,klic3);
// 	printf("valTyp %d value : %lf\n",moje->type, moje->data.num);
//
// 	TSymbol_Insert(symbol_table,klic3,data_pom2);
// 	t_universal *lol = TSymbol_Read(symbol_table,klic3);
// 	printf("valTyp %d value : %s\n",lol->type, lol->data.string.input);
//
// 	printf("hash %d hash2 %d\n",TSymbol_Hash(klic3),TSymbol_Hash(klic));
//
//
// 	printf("Typ: %d value : %lf\n",(*symbol_table[TSymbol_Hash(klic)])->data.varTyp,(*symbol_table[TSymbol_Hash(klic)])->data.value.d);
// 	tData* dataa;
// 	dataa = TSymbol_Read(symbol_table,klic);
// 	printf("Typ: %d value : %lf\n",dataa->varTyp,dataa->value.d);
// 	printf("(%s,%d)\n",(*symbol_table).key,(*symbol_table)->data.value);
//
	// -------------- EXPR ------------------
//

    table_symbol* symbol_table;
	symbol_table = TSymbol_Init();

	Pfunction_list* f_list;
	f_list = Pfunction_list_Init();

	konst_list* konst;
	konst = konst_list_Init();
	if(CTable[0]==NULL){};
	int32_t ret_value = PProcess(symbol_table,f_list,konst);
	ret_value = interpretation(f_list,konst);


	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	// Jsem ryba, jsem ryba, jsem ryba... 
	TSymbol_Free(symbol_table);
	Lexi_Destroy(); // Free all that lexical analyzator alloced.
	fclose(file); // Close the file.
	return ret_value;
}
