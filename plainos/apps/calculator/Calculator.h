/**##############################   文件说明  #############################################
用途：将中缀表达式转化为后缀表达式，并计算其值
范围：目前支持 + - * / 的所有浮点型四则运算，以及开平方运算，注意开平方用“！”表示
作者：ElecM
时间：2019.10.10
属性：开源
***####################################################################################****/

#ifndef  _CALCULATOR_H_
#define  _CALCULATOR_H_

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






#endif // _CALCULATOR_H_
