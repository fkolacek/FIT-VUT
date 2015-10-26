/*
 * XHERMA25 - Lukas Hermann		11.8	%
 * XSTODU06 - Dan Stodulka		66.6	%
 * XTRANH00 - Tran Manh Hoang		11.6	%
 * XMARCE02 - Andrea Marcelyova	10	%
 */
#ifndef __INST_ARRAY_LOOPINCLUDEPROTECTION
#define __INST_ARRAY_LOOPINCLUDEPROTECTION

#include "global.h"

#define APPEND 20

enum enum_instruction
{// R = registr,  kst -pole konstant, load nil -  RK - registr nebo konstanta
	I_MOVE = 0,    /*    A B     R(A) := R(B)  (konstantu move nepresouva)          */
	I_LOADK = 1,   /*    A Bx    R(A) := Kst(Bx)    indexujeme konstanty od 0      */
	I_LOADNIL = 2, /*    A B     R(A) := ... := R(B) := nil                      */ // local a;
	/** konstanta je posunuta o MAX_STACK_SIZE  oproti registrum */
	I_ADD = 3,     /*    A B C   R(A) := RK(B) + RK(C)                           */
	I_SUB = 4,     /*    A B C   R(A) := RK(B) - RK(C)                           */
	I_MUL = 5,     /*    A B C   R(A) := RK(B) * RK(C)                           */
	I_DIV = 6,     /*    A B C   R(A) := RK(B) / RK(C)                           */
	//I_MOD = 7,     /*    A B C   R(A) := RK(B) % RK(C)                           */
	//I_POW = 8,     /*    A B C   R(A) := RK(B) ^ RK(C)                           */
	//I_UNM = 9,     /*    A B     R(A) := -RK(B)                                   */
	//I_NOT = 10,    /*    A B     R(A) := not RK(B)                                */
	I_CONCAT = 7, /*    A B C   R(A) := RK(B) .. RK(C)                            */
// 	I_AND = 13,    /*    A B C   R(A) := RK(B) and RK(C)                           */
// 	I_OR = 14,     /*    A B C   R(A) := RK(B) or RK(C)                            */
	I_LE = 8,     /*    A B C   R(A) := RK(B) <  RK(C)                             */
	I_GR = 9,     /*    A B C   R(A) := RK(B) >  RK(C)                             */
	I_EL = 10,     /*    A B C   R(A) := RK(B) <= RK(C)                             */
	I_EG = 11,     /*    A B C   R(A) := RK(B) >= RK(C)                             */
	I_EQ = 12,     /*    A B C   R(A) := RK(B) == RK(C)                             */
	I_NEQ = 13,    /*    A B C   R(A) := RK(B) ~= RK(C)                             */
	I_JMP = 14,    /*      B     pc=B                                    */
	I_JT = 15,     /*    A B     if(RK(A) == true)   pc=B                */
	I_JF = 16,     /*    A B     if(RK(A) == false)  pc=B                */
	I_CALL = 17,   /*    A B C   R(A) <= funkce C (parametry R(A) * B)
				      neboli R(A) = call(R(A) = prvni parametr, B = pocet parametru, C = id funkce),
				      obdobne write - neni konstanta nic*/
	I_RETURN = 18, /*    A       return RK(A) */
// 	I_READ = 26,   /*    A B     R(A) = read(RK(B)) */
// 	I_WRITE = 27,   /*    A B     write(R(A) * R(B)) - neni konstanta nic*/

	I_INPUT = 19,	// embedd function input - syntax = input() - returns stdin input
			// R(A) := stdin = input()
	I_PRINT = 20,  	// R(A) := print(R(B) - prvni parametr, C - pocet parametru)
	I_LEN = 21,   	// R(A) := typeof(RK(B))
	I_TYPEOF = 22,	// R(A) := len(RK(B))
	I_FIND = 23,	// R(A) := find(RK(B),RK(C))
        I_SORT = 24,	// R(A) := sort(RK(B))
        I_NUMERIC = 25,	// R(A) := numeric(RK(B))
			
	I_STOP = 26 //- konec
};

typedef struct struct_symbol_pos
{
	char* id;
	int32_t index;
}struct_symbol_pos;

typedef struct symbol_pos_table
{
	struct_symbol_pos* symbol;
	int32_t allocated;
	int32_t free_pos;
	int32_t more_hats;
}symbol_pos_table;

typedef struct struct_instruction
{
	int32_t instruction;
	int32_t address_1;
	int32_t address_2;
	int32_t address_3;
}struct_instruction;

typedef struct Pfunction_struct
{
	int32_t reg_count;
	int32_t position;
	int32_t allocated;
	struct_instruction* instruction;
 	symbol_pos_table* sp_table;
}Pfunction_struct;

typedef struct Pfunction_list
{
	int32_t func_count;
        int32_t allocated;
	Pfunction_struct* function;
}Pfunction_list;



symbol_pos_table* symbol_pos_init();
void symbol_pos_insert(symbol_pos_table*,struct_symbol_pos);
void symbol_pos_free(symbol_pos_table*);
int32_t symbol_pos_find(symbol_pos_table*,char*);


Pfunction_list* Pfunction_list_Init();
void Pfunction_list_Free(Pfunction_list*);
void Pfunction_list_Insert(Pfunction_list*,Pfunction_struct);
void instruction_insert(Pfunction_list*,int32_t,int32_t,int32_t,int32_t,int32_t);

#endif
