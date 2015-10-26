#include "parser.h"

enum Arg
{
    Next,
    ArgOne

}PArgState = ArgOne;


int32_t PProcess(table_symbol* PSTable, Pfunction_list* PFList, konst_list* PKonst)
{
  if(CTable[0]==NULL){};
    Pfunction_struct PFMain;
    PFMain.reg_count = 0;
    PFMain.allocated = 0;
    PFMain.position = 0;
    PFMain.instruction = NULL;
    PFMain.sp_table = symbol_pos_init();
    Pfunction_list_Insert(PFList,PFMain);
    
    struct_token* tok;
    int32_t ret_value = 0;
    while(1){
        tok = Get_Next_Token();
        switch(tok->type)
        {
            case T_EndLine:
                break;

            case T_Id:
                if(!TSymbol_Insert(PSTable,tok->data.string,PVar))
                    return id_parser;
                ret_value = PId(PSTable,PFList,0,PKonst);
                if(ret_value)
                    return id_parser;
                break;

//            case T_Function:
//                ret_value = PFunction(PSTable,PFList,1);
//                if(ret_value)
//                    return id_parser;
//                break;
//
//            case T_While:
//                ret_value = PWhile(PSTable);
//                if(ret_value)
//                    return id_parser;
//                break;
//
//            case T_If:
//                ret_value = PIf(PSTable);
//                if(ret_value)
//                    return id_parser;
//                break;
//
//            case T_Return:
//                ret_value = PReturn(PSTable);
//                if(ret_value)
//                    return id_parser;
//                break;
//
//            case T_UndefinedState:
//                return id_lexi;
//
//            case T_Print:
//                ret_value = PPrint(PSTable,PFList,1,PKonst);
//                if(ret_value)
//                {
//                    return id_parser;
//                }
//                break;
//
//            case T_Input:
//                ret_value = PInput(PSTable,PFList,1);
//                if(ret_value)
//                {
//                    return id_parser;
//                }
//                break;
//
//            case T_Len:
//             //   ret_value = PLen(PSTable);
//              //  if(ret_value)
//             //       return id_parser;
//                break;
//
//            case T_Typeof:
//    //            ret_value = PTypeOf(PSTable);
//    //            if(ret_value)
//    //                return id_parser;
//                break;
//
//            case T_Numeric:
//    //            ret_value = PNumeric(PSTable);
//    //            if(ret_value)
//    //                return id_parser;
//                break;
//
//            case T_Sort:
//    //            ret_value = PSort(PSTable);
//    //            if(ret_value)
//    //                return id_parser;
//                break;
//
//            case T_Find:
//    //            ret_value = PFind(PSTable);
//    //            if(ret_value)
//    //                return id_parser;
//                break;
//
//            case T_SimpleBracketLeft:
//    //            tok = Get_Next_Token();
//    //            if(tok->type != T_Id)
//    //            {
//    //                return id_parser;
//    //            }
//    //            tok = Get_Next_Token();
//    //            if(tok->type != T_SimpleBracketRight)
//    //            {
//    //                return id_parser;
//    //            }
//    //            ret_value = PId(PSTable);
//    //            if(ret_value)
//    //                return id_parser;
//                break;

            case T_EOF:
		instruction_insert(PFList,0,I_STOP,0,0,0);
              return 0;

            default:
                return id_parser;
        }
        instruction_insert(PFList,0,I_STOP,0,0,0);
    }
    instruction_insert(PFList,0,I_STOP,0,0,0);
}

//int32_t PFunction(table_symbol* PSTable,Pfunction_list* PFList, int32_t PParentNum)
//{
//    struct_token* tok;
//    tok = Get_Next_Token();
//    if(tok->type != T_Id)
//        return id_parser;
//     if(!TSymbol_Insert(PSTable,tok->data.string,PFunc))
//        return id_parser;
//
//    tok = Get_Next_Token();
//    if(tok->type != T_SimpleBracketLeft)
//        return id_parser;
//
//    while(1)
//    {
//        tok = Get_Next_Token();
//        switch(PArgState)
//        {
//            case ArgOne:
//                if(tok->type != T_Id)
//                {
//                    return id_parser;
//                }
//                PArgState = Next;
//                break;
//
//            case Next:
//                if(tok->type == T_Comma)
//                    PArgState = ArgOne;
//                if(tok->type == T_SimpleBracketRight)
//                {
//                    tok = Get_Next_Token();
//                    if(tok->type != T_EndLine)
//                        return id_parser;
//                    return 0;
//                }
//                break;
//        }
//    }
//        return 0;
//}

int32_t PId(table_symbol* PSTable,Pfunction_list* PFList, int32_t PParentNum, konst_list* PKonstList)
{
    struct_token* tok;
    tok = Get_Next_Token();
    if(tok->type != T_Assign)
        return id_parser;

    Expr array;
    array = ExprInit();
    
    int32_t ret_value = ProcessExpr(PSTable,PKonstList,array);
//     PrintExpr(array,PKonstList);
    
    if(ret_value == 0)
    {
      QVarEle element;
      QVarEle element2;
      QVarEle element3;
      PStack p_stack;
      p_stack = PStackInit();
      
      for(int i=0;i<array->count;i++)
      {
	element = array->elements[i];
	int32_t longobongo;
	switch(element.type)
	{
	  case QOp:
	    element2 = PStackTopPop(p_stack);
	    element3 = PStackTopPop(p_stack);
	    switch(element.var.opt)
	    {
	      case EPlus:
		 instruction_insert(PFList,PParentNum,I_ADD,PFList->function[PParentNum].sp_table[0].more_hats,element2.var.TKonsIndex,element3.var.TKonsIndex);
		
		PFList->function[PParentNum].reg_count++;
		longobongo = PFList->function[PParentNum].sp_table[0].more_hats;
		element.type = QNum;
		element.var.TKonsIndex = longobongo;
		PStackPush(p_stack,element); 
		
		PFList->function[PParentNum].sp_table[0].more_hats++;
	      break;
	      default:
		break;
	    }
	   
	    
	    
	  break;
	  default:
	    PStackPush(p_stack,element); 
	  break;
	}
      }
    }
    
    return 0;
    // process expr

}


// int32_t PPrint(table_symbol* PSTable,Pfunction_list* PFList, int32_t PParentNum,konst_list* PKonst)
// {
//    struct_token* tok;
//    tok = Get_Next_Token();
//    if(tok->type != T_SimpleBracketLeft)
//        return id_parser;
//    int32_t longobongo = PFList->function[0].sp_table[0].more_hats;
//    int32_t i=0;
//    int32_t brutaleanale;
//    t_var drtikol;
//    while(1)
//    {	   
//        tok = Get_Next_Token();
//        switch(PArgState)
//        {
//            case ArgOne:
//                if(tok->type == T_Id)
//                {
//                    if(TSymbol_Search(PSTable,tok->data.string) == NULL){
//                        return id_parser;
//                    }
//                    
//                    drtikol.type = DATA_STRING;
// 		   drtikol.data.string = String_insert(tok->data.string);
// 		   
//                    brutaleanale = konst_list_Insert(PKonst,drtikol);
//                    
//                    instruction_insert(PFList,PParentNum,I_LOADK,longobongo+i,brutaleanale+MAX_STACK_SIZE,0);
// 		   PFList->function[PParentNum].sp_table[0].more_hats++;
// 		   PFList->function[PParentNum].reg_count++;
//                     i++;
// 		   PArgState = Next;
//                }
//                else if(tok->type == T_String)
//                {
//                    PArgState = Next;
//                }
//                else
//                {
//                    return id_parser;
//                }
//                break;
// 
//            case Next:
//                if(tok->type == T_Comma)
//                    PArgState = ArgOne;
//                if(tok->type == T_SimpleBracketRight)
//                {
//                    tok = Get_Next_Token();
//                    if(tok->type != T_EndLine)
//                        return id_parser;
//                    return 0;
//                }
//                break;
//        }
//    }
// }
//
//int32_t PWhile(table_symbol* PSTable)
//{
//    printf("process expr!\n");
//    int32_t ret_value = ProcessExpr(PSTable);
//    if(!ret_value)
//    return 0;
//    else
//    return 0;
//}
//
//int32_t PIf(table_symbol* PSTable)
//{
//    printf("process expr!\n");
//    int32_t ret_value = ProcessExpr(PSTable);
//    if(!ret_value)
//    return 0;
//    else
//    return 0;
//}
//
//int32_t PReturn(table_symbol *PSTable)
//{
//    printf("process expr!\n");
//    int32_t ret_value = ProcessExpr(PSTable);
//    if(!ret_value)
//    return 0;
//    else
//    return 0;
//}
//
//int32_t PInput(table_symbol *PSTable, Pfunction_list* PFList,int32_t PParentNum)
//{
//    instruction_insert(PFList,0,I_INPUT,0,0,0);
//    //instruction_insert(PFList,0,I_STOP,0,0,0);
//    return 0;
//}

PStack PStackInit()
{
    PStack S;
    S = (PStack)mem_alloc(sizeof(struct PStack),id_parser);
    S->element = (QVarEle*)mem_alloc(sizeof(QVarEle)*PINITSIZE,id_parser);
    S->count = 0;
    S->size = PINITSIZE;
    return S;
}

void PStackPush(PStack S,QVarEle temp)
{
    if(S->count == S->size)
    {
        S->size*=2;
        S->element = (QVarEle*)mem_realloc(S->element,((S->size)*sizeof(QVarEle)));
    }
    S->element[S->count++] = temp;
}

QVarEle PStackTopPop(PStack S)
{
    S->count--;
    return S->element[S->count];
}