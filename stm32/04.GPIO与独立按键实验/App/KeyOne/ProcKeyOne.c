/*********************************************************************************************************
* 模块名称: 
* 摘    要: 
* 当前版本: 1.0.0
* 作    者: SZLY(COPYRIGHT 2018 SZLY. All rights reserved.)
* 完成日期: 2018年01月01日 
* 内    容:
* 注    意: none                                                                  
**********************************************************************************************************
* 取代版本: 
* 作    者:
* 完成日期: 
* 修改内容:
* 修改文件: 
*********************************************************************************************************/
 

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "ProcKeyOne.h"
#include "UART1.h" 
#include "LED.h"
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
 
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitProcKeyOne
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void InitProcKeyOne(void) 
{ 
  ;
} 

/*********************************************************************************************************
* 函数名称: ProcKeyDownKey1 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  ProcKeyDownKey1(void) 
{   
  printf("KEY1 PUSH DOWN\r\n");  //打印按键状态 
  mode1();
} 

/*********************************************************************************************************
* 函数名称: ProcKeyUpKey1 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  ProcKeyUpKey1(void) 
{ 
    //mode1();
    //printf("KEY1 RELEASE\r\n");   //打印按键状态 
}

/*********************************************************************************************************
* 函数名称: ProcKeyDownKey2 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  ProcKeyDownKey2(void) 
{   
  printf("KEY2 PUSH DOWN\r\n");  //打印按键状态
  
  mode2();    
} 

/*********************************************************************************************************
* 函数名称: ProcKeyUpKey2 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  ProcKeyUpKey2(void) 
{ 
    //mode2();   //打印按键状态 
}

/*********************************************************************************************************
* 函数名称: ProcKeyDownKey3 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  ProcKeyDownKey3(void) 
{   
  printf("KEY3 PUSH DOWN\r\n");  //打印按键状态 
  
  mode3();
  
} 

/*********************************************************************************************************
* 函数名称: ProcKeyUpKey3 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  ProcKeyUpKey3(void) 
{ 
    //mode3(); 
}










