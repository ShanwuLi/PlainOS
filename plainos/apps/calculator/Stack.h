#ifndef _STACK_H_
#define _STACK_H_

/**##############################   文件说明  #############################################
用途：提供字符型和双精度浮点型两种栈
作者：ElecM
时间：2019.10.10
属性：开源
***####################################################################################****/

/***----------------------------------------------------------------------------------------
类名：字符型STACK
方法：1.入栈 Push(const char Data);
      2.出栈 char Pop(void);
      3.获取栈顶元素  char GetTopData(void);
      4.栈空判断  int StackEmpty(void); 栈空返回1 ； 栈不空返回0
------------------------------------------------------------------------------------------**/
class STACK
{
   private:
     char StackArray[200];
     int StackPoint;

   public:
     STACK(void);
     void Push(const char Data);
     char Pop(void);
     char GetTopData(void);
     int StackEmpty(void);
};

/***----------------------------------------------------------------------------------------
类名：双精度浮点型STACK
方法：1.入栈 Push(double Data);
      2.出栈 double Pop(void);
      4.栈空判断  int StackEmpty(void); 栈空返回1 ； 栈不空返回0
------------------------------------------------------------------------------------------**/
class DOUBLE_STACK
{
   private:
     double StackArray[200];
     int StackPoint;

   public:
     DOUBLE_STACK(void);
     void Push(double Data);
     double Pop(void);
     int StackEmpty(void);
};



#endif // _STACK_H_
