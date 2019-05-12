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
#include "EXTI.h"
#include <stm32f10x_conf.h> 
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
 
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/

static void ConfigEXTIGPIO(void);   //配置 EXTI 按键的 GPIO  
static void ConfigEXTI(void);       //配置 EXTI 

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: ConfigEXTIGPIO 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
static void ConfigEXTIGPIO(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;  //定义结构体 GPIO_InitStructure,用来配置 EXTI 按键的 GPIO 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能 KEY1 和 KEY2 的时钟 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //KEY3
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;           //设置 KEY1 的引脚 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;     //设置 I/O 口速率为 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置 KEY0 的 GPIO 端口的模式 
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化 KEY1 的 GPIO 端口 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;           //设置 KEY2 的引脚 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;     //设置 I/O 口速率为 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置 KEY0 的 GPIO 端口的模式 
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化 KEY2 的 GPIO 端口

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;           //设置 KEY3 的引脚 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;     //设置 I/O 口速率为 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //设置 KEY0 的 GPIO 端口的模式 
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //根据参数初始化 KEY3 的 GPIO 端口 
}
/*********************************************************************************************************
* 函数名称: ConfigEXTI
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
static void ConfigEXTI(void) 
{
  EXTI_InitTypeDef EXTI_InitStructure;//定义结构 EXTI_InitStructure,用来配置 EXTI 按键的中断 
  NVIC_InitTypeDef NVIC_InitStructure;//定义结构 NVIC_InitStructure,用来配置 EXTI 按键的 NVIC 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //开启 AFIO 时钟 
  
  //配置 KEY1 的中断服务 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);   //中断线配置 
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;                    //中断线 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //中断 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
  EXTI_Init(&EXTI_InitStructure);                               //初始化中断线参数
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;              //使能按键外部中断通道 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //抢占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //子优先级 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能外部中断通道 
  NVIC_Init(&NVIC_InitStructure);                               //初始化 NVIC 
  
  //配置 KEY2 的中断服务 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);   //中断线配置 
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;                    //中断线 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //中断 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  EXTI_Init(&EXTI_InitStructure);        //初始化中断线参数
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;              //使能按键外部中断通道 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //抢占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //子优先级 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能外部中断通道 
  NVIC_Init(&NVIC_InitStructure);                               //初始化 NVIC
  
  //配置 KEY3 的中断服务 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);   //中断线配置 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                    //中断线 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //中断 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //上升沿触发 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  EXTI_Init(&EXTI_InitStructure);     //初始化中断线参数 
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;              //使能按键外部中断通道 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //抢占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //子优先级 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //使能外部中断通道 
  NVIC_Init(&NVIC_InitStructure);                               //初始化 NVIC 
  
  
}


/*********************************************************************************************************
* 函数名称: EXTI0_IRQHandler 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) //检查中断标志位
  {
    //为所欲为
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4))); 
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5))); 
    
    //
    EXTI_ClearITPendingBit(EXTI_Line0);     //清除 Line0 上的中断标志位
  }
}
/*********************************************************************************************************
* 函数名称: EXTI1_IRQHandler 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET) //检查中断标志位
  {
    //为所欲为
    
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4))); 

    
    //
    EXTI_ClearITPendingBit(EXTI_Line2);     //清除 Line0 上的中断标志位
  }
}
/*********************************************************************************************************
* 函数名称: EXTI2_IRQHandler 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET) //检查中断标志位
  {
    //为所欲为
    
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5))); 
    
    //
    EXTI_ClearITPendingBit(EXTI_Line2);     //清除 Line0 上的中断标志位
  }
}


/*********************************************************************************************************
*                                              API函数实现
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
void  InitEXTI(void) 
{
  ConfigEXTIGPIO(); //配置EXTI按键的GPIO 
  ConfigEXTI();     //配置EXTI的中断 
}

