/**##############################   文件说明  #############################################
用途：将中缀表达式转化为后缀表达式，并计算其值
范围：目前支持 + - * / 的所有浮点型四则运算，以及开平方运算，注意开平方用“！”表示
作者：ElecM
时间：2019.10.10
属性：开源
***####################################################################################****/

#include "Calculator.h"
#include <math.h>
#include <kernel/mempool.h>

#include "Stack.h"
#include <stdlib.h>


/***----------------------------------------------------------------------------------------
类名：INFIX_INTO_SUFFIX
方法：1.中缀转后缀方法：        Convert(中缀表达式输入流, 后缀表达式输出流);
      2.中缀表达式合法性检查：  bool  CheckLegal(中缀表达式输入流);
        合法返回true ，非法返回false
------------------------------------------------------------------------------------------**/
class  INFIX_INTO_SUFFIX
{
   private:
     int ISP(const char symbol);
     int OSP(const char symbol);
   public:
     INFIX_INTO_SUFFIX(void);
     void  Convert(const  char *InStream, char *OutStream);
     bool  CheckLegal(const  char *InStream);
};

/***----------------------------------------------------------------------------------------
类名：CALCULATE_SUFFIX
方法：double calculate(后缀表达式输入流);
      返回计算结果
-----------------------------------------------------------------------------------------**/
class  CALCULATE_SUFFIX
{
   public:
     double calculate(const char *Str);
};

INFIX_INTO_SUFFIX::INFIX_INTO_SUFFIX(void)
{
   ;
}

int INFIX_INTO_SUFFIX::ISP(const char symbol)
{
   switch(symbol)
   {
     case '!' : return 7;
     case '*' : return 5;
     case '/' : return 5;
     case '+' : return 3;
     case '-' : return 3;
     case '(' : return 1;
     case ')' : return 8;
     case '@' : return 0;
     default  : return  -1;
   }
}

int INFIX_INTO_SUFFIX::OSP(const char symbol)
{
   switch(symbol)
   {
     case '!' : return 6;
     case '*' : return 4;
     case '/' : return 4;
     case '+' : return 2;
     case '-' : return 2;
     case '(' : return 8;
     case ')' : return 1;
     case '@' : return 0;
     default  : return  -1;
   }
}

bool INFIX_INTO_SUFFIX::CheckLegal(const  char *InStream)
{
   int BracketNum = 0;
   bool OperatorLegal = true;
   while(*InStream !='\0')
   {
      if(*InStream == '(')
          BracketNum++;
      else if(*InStream == ')')
          BracketNum--;
      else if((((*InStream )=='+')||((*InStream )=='-')||((*InStream)=='*')||((*InStream)=='/'))  \
              &&((*(InStream+1)< '0')||((*(InStream+1))>'9'))&&(*(InStream+1)!='!')&&(*(InStream+1)!='('))
              OperatorLegal = false;
      else  if((*InStream !='+')&&(*InStream !='-')&&(*InStream !='*')&&(*InStream !='/')&&(*InStream !='.') \
               &&(((*InStream)<'0')||((*InStream )>'9'))&&((*InStream)!='!'))
              OperatorLegal = false;
      else if((*InStream == '!')&&(*(InStream+1) == '!'))
           OperatorLegal = false;

     InStream++;
   }

   if((BracketNum!=0)||(OperatorLegal == false))
     return  false;
   else
     return  true;

}




void INFIX_INTO_SUFFIX::Convert(const  char *InStream, char *OutStream)
{
   int NumbleFirst=0;
   STACK Stack;
   Stack.Push('@');
   char m=0;
   while( !(Stack.StackEmpty()) )
   {
     if(((*InStream) == '(')&&((*(InStream+1) == '-')))
       {
          Stack.Push(*InStream);
          InStream++;
          if(NumbleFirst == 0)
           {
             *(OutStream ++) = '$';
             NumbleFirst ++;
           }
          *(OutStream++) = *(InStream++);
       }
    /*
     else if((*InStream) == '!')
      {
         if(NumbleFirst == 0)
           {
             *(OutStream ++) = '$';
             NumbleFirst ++;
           }
          *(OutStream++) = *(InStream++);
      }
      */
     else if( (((*InStream)<= '9')&&((*InStream)>='0'))||((*InStream) == '.'))
        {
          if(NumbleFirst == 0)
           {
             *(OutStream ++) = '$';
             NumbleFirst ++;
           }

          *(OutStream++) = *(InStream++);
        }
     else
    {
      NumbleFirst = 0;
      if(OSP(*InStream) > ISP(Stack.GetTopData()))
        {
          Stack.Push(*InStream);
          InStream++;
        }
     else if(OSP(*InStream) < ISP(Stack.GetTopData()))
        {
          *OutStream = Stack.Pop();
          OutStream++;
        }
     else
      {
          m= Stack.Pop();
         if(m== '(')
            InStream++;
      }
    }
   }
   *(OutStream-1) = '\0';
   *(OutStream)= '\0';
}





double CALCULATE_SUFFIX::calculate(const char *Str)
{

    double result1 = 0.0,result2 = 0.0;
    char WorkSpace[50]={'\0'}; //
    char *p = WorkSpace;
    
	DOUBLE_STACK *Stack = (DOUBLE_STACK *)pl_mempool_malloc(g_pl_default_mempool, sizeof(DOUBLE_STACK));
    while(*Str!='\0')
    {
       p = WorkSpace;
       if((*Str) == '$')
       {
          Str++;
        /*  if(*Str == '!')
          {
            Str++;
            while((((*Str)>='0')&&((*Str)<='9'))||((*Str)=='.'))
            {
              *(p) = *Str;
              p++;
              Str++;
            }
          (*p)='\0';
          result1 =  atof(WorkSpace);
          Stack.Push(sqrt(result1));
          }
          */
         // else
          {
            while((((*Str)>='0')&&((*Str)<='9'))||((*Str)=='.')||(((*Str)=='-')&&(*(Str+1)>='0')&&(*(Str+1)<='9')))
            {
              *(p) = *Str;
              p++;
              Str++;
            }
          (*p)='\0';
          result1 =  atof(WorkSpace);
          Stack->Push(result1);
          }
       }
       else if(*Str == '!')
       {
           result1 = Stack->Pop();
           result1 = sqrt(result1);
           Stack->Push(result1);
           Str++;
       }
       else
       {
           result2 = Stack->Pop();
           result1 = Stack->Pop();
           switch(*Str)
           {
               case '+' : result1 = result1 + result2;break;
               case '-' : result1 = result1 - result2;break;
               case '*' : result1 = result1 * result2;break;
               case '/' : result1 = result1 / result2;break;    // 在此可添加检错机制
               default  : break;
           }
           Stack->Push(result1);
           Str++;
       }
    }

    result1 = Stack->Pop();

	pl_mempool_free(g_pl_default_mempool, Stack);
    return result1;

}

int pl_calculator(int argc, char **argv, float *result)
{
	if (argc != 2)
		return -1;

	char str[50]={0};

    INFIX_INTO_SUFFIX InfixIntoSuffix;
    CALCULATE_SUFFIX  cal;

    if(!InfixIntoSuffix.CheckLegal(argv[1]))
       return -2;

     InfixIntoSuffix.Convert(argv[1], str);
    *result = cal.calculate(str);
    
    return 0;
}
