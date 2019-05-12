/*********************************************************************************************************
* 模块名称: ProcKeyOne.c
* 摘    要: 独立按键处理模块，进行独立按键处理模块初始化，以及独立按键处理函数实现
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
#include <stm32f10x_conf.h>
#include "ProcKeyOne.h"
#include "UART1.h"
#include "IWDG.h" 

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
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
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitProcKeyOne
* 函数功能: 初始化ProcKeyOne
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void InitProcKeyOne(void)
{
  
}

/*********************************************************************************************************
* 函数名称: ProcKeyDownKey1
* 函数功能: 处理按键按下的事件，即按键按下的响应函数 
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void  ProcKeyDownKey1(void)
{ 

  FeedIWDG();                    //喂独立看门狗 
  printf("KEY1 PUSH DOWN\r\n");  //打印按键状态
  
}

/*********************************************************************************************************
* 函数名称: ProcKeyUpKey1
* 函数功能: 处理按键弹起的事件，即按键弹起的响应函数 
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void  ProcKeyUpKey1(void)
{
  printf("KEY1 RELEASE\r\n");   //打印按键状态
}

/*********************************************************************************************************
* 函数名称: ProcKeyDownKey2
* 函数功能: 处理按键按下的事件，即按键按下的响应函数 
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void  ProcKeyDownKey2(void)
{
  printf("KEY2 PUSH DOWN\r\n");   //打印按键状态
}

/*********************************************************************************************************
* 函数名称: ProcKeyUpKey2
* 函数功能: 处理按键弹起的事件，即按键弹起的响应函数 
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void  ProcKeyUpKey2(void)
{
  printf("KEY2 RELEASE\r\n");     //打印按键状态
}

/*********************************************************************************************************
* 函数名称: ProcKeyDownKey3
* 函数功能: 处理按键按下的事件，即按键按下的响应函数 
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void  ProcKeyDownKey3(void)
{
  printf("KEY3 PUSH DOWN\r\n");   //打印按键状态
}

/*********************************************************************************************************
* 函数名称: ProcKeyUpKey3
* 函数功能: 处理按键弹起的事件，即按键弹起的响应函数 
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void  ProcKeyUpKey3(void)
{  
  printf("KEY3 RELEASE\r\n");     //打印按键状态
}
