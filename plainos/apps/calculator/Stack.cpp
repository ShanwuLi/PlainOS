/**##############################   文件说明  #############################################
用途：提供字符型和双精度浮点型两种栈
作者：ElecM
时间：2019.10.10
属性：开源
***####################################################################################****/
#include "Stack.h"
STACK::STACK(void)
{
    unsigned int i=0;
    for(i=0;i<200;i++)
        StackArray[i] = 0;
    StackPoint = -1;
}

void STACK::Push(const char Data)
{
    if(StackPoint == 10)
	return;
    else
        StackArray[++StackPoint] = Data;
}

char STACK::Pop(void)
{
    char RD = 0;
    if(StackPoint < 0)
	return 0;
    else
    {
       RD = StackArray[StackPoint];
       StackPoint -- ;
    }
    return RD;
}

char STACK::GetTopData(void)
{
   return  StackArray[StackPoint];
}


int STACK::StackEmpty(void)
{
    if(StackPoint == -1)
     return 1;
    else
     return 0;
}


DOUBLE_STACK::DOUBLE_STACK(void)
{
  int i=0;
  for(i=0;i<200;i++)
   StackArray[i]=0.0;
  StackPoint = -1;
}


void DOUBLE_STACK::Push(double Data)
{
   if(StackPoint==200)
   return;
   else {
     StackArray[++StackPoint]= Data;
   }
}


double DOUBLE_STACK::Pop(void)
{
    double RD = 0.0;
    if(StackPoint<0)
	return 0.0;
    else
      RD = StackArray[StackPoint--];

    return RD;
}

int DOUBLE_STACK::StackEmpty(void)
{
    if(StackPoint == -1)
        return 1;
    else
        return 0;
}
