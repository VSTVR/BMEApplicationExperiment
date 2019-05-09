/*********************************************************************************************************
* 模块名称: LED.c
* 摘    要: LED模块，包含LED模块初始化以及LED闪烁函数
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
#include "LED.h"
#include "delay.h"
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
static  void  ConfigLEDGPIO(void);         //配置LED的GPIO

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: ConfigLEDGPIO
* 函数功能: 配置LED的GPIO 
* 输入参数: void 
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
static  void  ConfigLEDGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  //定义结构体GPIO_InitStructure,用来配置LED的GPIO
                                                                     
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能LED1的时钟
                                                                                                                 
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;           //设置LED1的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //设置I/O速率为50MHz
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     //推挽输出模式
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化LED1的GPIO端口

  GPIO_WriteBit(GPIOC, GPIO_Pin_4, Bit_SET);            //LED1默认值为点亮

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;           //设置LED2的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //设置I/O口速率为50MHz
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;     //推挽输出模式
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //根据参数初始化LED2的GPIO端口

  GPIO_WriteBit(GPIOC, GPIO_Pin_5, Bit_RESET);          //LED2默认值为熄灭
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitLED
* 函数功能: 初始化LED模块
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void InitLED(void)
{
  ConfigLEDGPIO();  //配置LED的GPIO
}

/*********************************************************************************************************
* 函数名称: LEDFlicker
* 函数功能: LED闪烁函数，主要是为了测试
* 输入参数: cnt
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: LEDFlicker在Proc2msTask中调用，因此，cnt为250时表示500毫秒更改一次LED状态
*********************************************************************************************************/
void LEDFlicker(u16 cnt)
{
  static u16 s_iCnt;     //定义静态变量s_iCnt作为循环计数器
  
  s_iCnt++;              //计数器数值加1
  
  if(s_iCnt >= cnt)      //计数器数值大于LED闪烁定时参数cnt
  { 
    s_iCnt = 0;          //计数器数值重置为0

    //LED1状态取反，实现LED0闪烁
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4)));
    
    //LED2状态取反，实现LED1闪烁
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5)));
  }
}

void mode1()
{
  GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(0));
  GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(0));
}

void mode2()
{
  
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(0));
        GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(0));
        delay_ms(1000);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(0));
        GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1));
        delay_ms(1000);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1));
        GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(0));
        delay_ms(1000);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1));
        GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1));
        delay_ms(1000);
        GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(0));
        GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(0));
      
}

void mode3()
{
  
          GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(0));
          GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(0));
          delay_ms(1000);
          GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1));
          GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1));
          delay_ms(1000);
          GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1));
          GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(0));
          delay_ms(1000);
          GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(0));
          GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1));
          delay_ms(1000);
          GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(0));
          GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(0));
       
}

