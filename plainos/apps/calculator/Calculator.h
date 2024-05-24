/**##############################   文件说明  #############################################
用途：将中缀表达式转化为后缀表达式，并计算其值
范围：目前支持 + - * / 的所有浮点型四则运算，以及开平方运算，注意开平方用“！”表示
作者：ElecM
时间：2019.10.10
属性：开源
***####################################################################################****/

#ifndef  _CALCULATOR_H_
#define  _CALCULATOR_H_

#ifdef __cplusplus
extern "C" {
#endif
int pl_calculator(int argc, char **argv, float *result);

#ifdef __cplusplus
}
#endif



#endif // _CALCULATOR_H_
