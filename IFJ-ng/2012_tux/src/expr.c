#include "expr.h"
#define SINITSIZE 256
#define QINITSIZE 256







Stack EStackInit();                 // Init stack
void EStackPush(Stack,ELogic);      // Push a symbol onto the stack
ELogic EStackTopPop(Stack);            // Pops and store a symbol from the stack
ELogic EStackTop(Stack);                // Returns top symbol from the stack w/o pop
ELogic TokenToStackSymbol(struct_token*);   // Convert enum type to stack symbol
void EStackPop(Stack);                      // Pops a symbol from the stack

void ExprInsert(Expr,QVarEle);
Expr ExprInit();

ELogic CtablePrecedence(int32_t,int32_t);
int32_t StackSymbolToIndex(ELogic);



int32_t ProcessExpr(table_symbol* PSTable, konst_list* EKonstList,Expr array)
{
    if(CTable[0]==NULL){};
    struct_token* tok;
    QVarEle new_element;
    t_var* tvar_temp;
	tvar_temp = mem_alloc(sizeof(t_var),id_main);
	Stack S = EStackInit();
	ELogic Etemp;

    int32_t temp_operator = FALSE;
    int32_t temp_id = TRUE;



    int32_t tkons_index = 0;
    do
    {
        tok = Get_Next_Token();
        if(tok->type == T_Id)
        {
            if(!temp_id)
            {
                return id_parser;
            }
            temp_operator = TRUE;
            temp_id = FALSE;
            if(TSymbol_Search(PSTable,tok->data.string) == NULL)
            {
                return id_parser;
            }
            else
            {
                new_element.var.TSymbolKey = mem_alloc(sizeof(char)*strlen(tok->data.string)+1,id_parser);
                ExprInsert(array,new_element);
            }
        }
        else if(tok->type == T_Number)
        {
            if(!temp_id)
            {
                return id_parser;
            }
            temp_operator = TRUE;
            temp_id = FALSE;
            tvar_temp->type = DATA_NUM;
            tvar_temp->data.num = tok->data.num;
            tkons_index = konst_list_Insert(EKonstList,*tvar_temp);
            new_element.type = QNum;
            new_element.var.TKonsIndex = tkons_index + MAX_STACK_SIZE;
            ExprInsert(array,new_element);
        }
        else if(tok->type == T_String)
        {
            if(!temp_id)
            {
                return id_parser;
            }
            temp_operator = TRUE;
            temp_id = FALSE;
            tvar_temp->type = DATA_STRING;
        	tvar_temp->data.string = String_insert(tok->data.string);
            tkons_index = konst_list_Insert(EKonstList,*tvar_temp);
            new_element.var.TKonsIndex = tkons_index + MAX_STACK_SIZE;
            new_element.type = QString;
            ExprInsert(array,new_element);
        }
        else if(tok->type == T_Concatenate || tok->type == T_Mul || tok->type == T_Div || tok->type == T_PlusSign || tok->type == T_MinusSign)
        {
            if(!temp_operator)
            {
                return id_parser;
            }
            temp_id = TRUE;
            temp_operator = FALSE;
            if(S->count == 0)
            {
                EStackPush(S,TokenToStackSymbol(tok));
            }
            else if((CtablePrecedence(StackSymbolToIndex(TokenToStackSymbol(tok)),StackSymbolToIndex(EStackTop(S))))== EGt)
            {
                EStackPush(S,TokenToStackSymbol(tok));
            }
            else{
                while(S->count != 0 || CtablePrecedence(StackSymbolToIndex(TokenToStackSymbol(tok)),StackSymbolToIndex(EStackTop(S))!= ELe) )
                {
                    Etemp = EStackTopPop(S);
                    new_element.var.opt = Etemp;
                    new_element.type = QOp;
                    ExprInsert(array,new_element);
                }
                EStackPush(S,TokenToStackSymbol(tok));
            }
        }
    }while(tok->type!= T_EndLine);

    if(!temp_operator)
    {
        return id_parser;
    }

        while(S->count != 0)
        {
            Etemp = EStackTopPop(S);
            new_element.var.opt = Etemp;
            new_element.type = QOp;
            ExprInsert(array,new_element);
        }

    //PrintExpr(array,EKonstList);

    return 0;
}

Stack EStackInit()
{
    Stack S;
    S = (Stack)mem_alloc(sizeof(struct Stack),id_parser);
    S->element = (ELogic*)mem_alloc(sizeof(ELogic)*SINITSIZE,id_parser);
    S->count = 0;
    S->size = SINITSIZE;
    return S;
}

void EStackPush(Stack S,ELogic temp)
{
    if(S->count == S->size)
    {
        S->size*=2;
        S->element = (ELogic*)mem_realloc(S->element,((S->size)*sizeof(ELogic)));
    }
    S->element[S->count++] = temp;
}

ELogic EStackTopPop(Stack S)
{
    S->count--;
    return S->element[S->count];
}

ELogic EStackTop(Stack S)
{
    return S->element[S->count-1];
}

ELogic TokenToStackSymbol(struct_token* tok)
{
    switch(tok->type)
    {
        case T_Id:
            return EId;

        case T_Concatenate:
            return EConc;

        case T_PlusSign:
            return EPlus;

        case T_MinusSign:
            return EMinus;

        case T_Mul:
            return EMul;

        case T_Div:
            return EDivide;

        case T_LogicLesser:
            return ELesser;

        case T_LogicLesserEqual:
            return ELesserEq;

        case T_LogicGreater:
            return EGreater;

        case T_LogicGreaterEqual:
            return EGreaterEq;

        case T_LogicEqual:
            return EEqual;

        case T_LogicNotEqual:
            return ENequal;

        case T_SimpleBracketLeft:
            return ESimpleLeft;

        case T_SimpleBracketRight:
            return ESimpleRight;

        case T_String:
            return ETermString;

        case T_Number:
            return ETermNumb;

        default:
            return ERr;
    }
}

Expr ExprInit()
{
    Expr temp;
    temp = mem_alloc(sizeof(struct ExprArray),id_parser);
    temp->count = 0;
    temp->size = QINITSIZE;
    temp->elements = mem_alloc(sizeof(QVar)*QINITSIZE,id_parser);
    return temp;
}

void ExprInsert(Expr temp, QVarEle element)
{
    if(temp->count == temp->size)
    {
        temp->size*=2;
        temp->elements = (QVarEle*)mem_realloc(temp->elements,sizeof(QVar)*temp->size);
    }
    temp->elements[temp->count] = element;
    temp->count++;
}

ELogic CtablePrecedence(int32_t i, int32_t j)
{
    if(i < 0 || j < 0)
    {
        return -1;
    }
//     printf("ctable! %d\n",CTable[i][j]);
    return CTable[i][j];
}

int32_t StackSymbolToIndex(ELogic op)
{
    switch(op)
    {
        case EMul:
        case EDivide:
            return 8;

        case EConc:
            return 10;

        case EPlus:
        case EMinus:
            return 6;

        default:
            return -1;
    }
}

// void PrintExpr(Expr array,konst_list* tkonst)
// {
// //     t_var temp;
//     //printf("array count %d\n",array->count);
//     for(int i = 0; i < array->count; i++)
//     {
//         switch(array->elements[i].type)
//         {
//             case QNum:
// //                 temp =  tkonst->konst[array->elements[i].var.TKonsIndex - MAX_STACK_SIZE];
//                 //printf("%lf",temp.data.num);
//                 break;
//             case QString:
// //                 temp =  tkonst->konst[array->elements[i].var.TKonsIndex - MAX_STACK_SIZE];
//                // printf("%s",temp.data.string.input);
//                 break;
//             case QOp:
//                 //printf("\t%d\t",array->elements[i].var.opt);
//                 break;
//         }
//     }
// //     printf("\n");
// }

