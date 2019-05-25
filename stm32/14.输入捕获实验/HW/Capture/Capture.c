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
#include "Capture.h"
#include "stm32f10x_conf.h" 
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
 
 //s_iCaptureSts中的bit7：为捕获完成的标志，bit6：捕获低电平标志，bit5-0：捕获低电平后定时器溢出的次数 
 static  u8  s_iCaptureSts = 0;     //捕获状态 
 static  u16 s_iCaptureVal;         //捕获值 
 
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/

static  void ConfigTIMxForCapture(u16 arr, u16 psc);    //配置定时器TIMx的通道输入捕获 

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: ConfigTIMxForCapture 
* 函数功能: 配置定时器 x 的通道输入捕获 
* 输入参数: arr：自动重装值；psc：时钟预分频数 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意: 使用定时器 TIM5 的通道 1 （PA0） 来做输入捕获， 捕获 PA0 上低电平的脉宽 （用 KEY3 按键输入高电
*********************************************************************************************************/
static  void ConfigTIMxForCapture(u16 arr, u16 psc) 
{
  GPIO_InitTypeDef        GPIO_InitStructure;    //定义结构体 GPIO_InitStructure，用来配置捕获通道的 GPIO 
  TIM_TimeBaseInitTypeDef TIMx_TimeBaseStructure;//定义结构体 TIMx_TimeBaseStructure，用来配置定时器 TIMx 的参 数 
  TIM_ICInitTypeDef       TIMx_ICInitStructure;  //定义结构 TIMx_ICInitStructure,用来初始化捕获通道 
  NVIC_InitTypeDef        NVIC_InitStructure;    //定义结构体 NVIC_InitStructure，用来配置中断 NVIC 的参数 
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);                //使能定时器 TIMx 的时钟 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);               //使能捕获的 GPIO 端口时钟 
  
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                //设置捕获通道对应的引脚 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             //上拉输入模式  
  GPIO_Init(GPIOA, &GPIO_InitStructure);                    //根据参数初始化捕获的 GPIO 端口 
  GPIO_SetBits(GPIOA, GPIO_Pin_0);  //将捕获对应的引脚置为高电平 
  
  //定时器 TIMx 初始化 
  TIMx_TimeBaseStructure.TIM_Period        = arr;                     //设定计数器自动重装值 
  TIMx_TimeBaseStructure.TIM_Prescaler     = psc;                     //设置 TIMx 时钟频率除数的预分频值 
  TIMx_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;            //设置时钟分割 
  TIMx_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;      //设置定时器 TIMx 为向上计数模式 
  TIM_TimeBaseInit(TIM5, &TIMx_TimeBaseStructure); //根据指定的参数初始化 TIMx 的时间基数单位
  
  //配置 TIMx 的捕获通道参数 
  //CC1S = 01，CC1 通道被配置为输入，输入通道 IC1 映射到定时器引脚 TI1 上 
  TIMx_ICInitStructure.TIM_Channel        = TIM_Channel_1;  
  TIMx_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Falling;   //设置初始化为下降沿捕获 
  TIMx_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; //设置为直接映射到 TI1 上 
  TIMx_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;           //配置每一个边沿都捕获，捕捉不分频 
  TIMx_ICInitStructure.TIM_ICFilter       = 0x08;                     //配置输入滤波器 
  TIM_ICInit(TIM5, &TIMx_ICInitStructure);         //初始化 TIMx 输入捕获参数 
  
  //中断分组初始化 
  NVIC_InitStructure.NVIC_IRQChannel                   = TIM5_IRQn;   //设置定时器 TIMx 的中断通道 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;           //设置抢占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;           //设置从优先级 
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      //使能中断 NVIC 的中断通道 
  NVIC_Init(&NVIC_InitStructure);                                     //根据参数初始化中断 NVIC 寄存器

  //允许更新中断，允许 CC1IE 捕获中断（捕获到有效信号可以开启中断） 
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);  


  TIM_Cmd(TIM5, ENABLE);  //使能定时器 TIMx
   
}

/*********************************************************************************************************
* 函数名称: TIM5_IRQHandler 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void TIM5_IRQHandler(void) 
{
  if((s_iCaptureSts & 0x80) == 0) //最高位为 0，表示捕获还未完成 
  {
    //高电平，定时器TIMx 发生了溢出事件 
    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
      if(s_iCaptureSts & 0x40)    //发生溢出，并且前一次已经捕获到低电平 
      {
        //TIM_APR 16 位预装载值，即 CNT > 65536-1（2^16 - 1）时溢出。 
        //若不处理，(s_iCaptureSts & 0X3F)++等于 0x40 ，溢出数等于清 0 
        if((s_iCaptureSts & 0x3F) == 0x3F)  //达到多次溢出，低电平太长
        {
          s_iCaptureSts |= 0x80;  //强制标记成功捕获了一次 
          s_iCaptureVal = 0xFFFF; //捕获值为 0xFFFF 
        }
        else
        {
          s_iCaptureSts++;        //标记计数器溢出一次
        }
      }
    }
    
    if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET) //发生捕获事件
    {
      if(s_iCaptureSts & 0x40)  //bit6 为 1，即上次捕获到下降沿，那么这次捕获到上升沿
      {
        s_iCaptureSts |= 0x80;  //完成捕获，标记成功捕获到一次上升沿 
        s_iCaptureVal = TIM_GetCapture1(TIM5);  //s_iCaptureVa 记录捕获比较寄存器的值
        //CC1P=1 设置为下降沿捕获，为下次捕获做准备
        TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling); 
      }
      else //bit6 为 0，表示上次没捕获到下降沿，这是第一次捕获下降沿
      {
        s_iCaptureSts = 0;  //清空溢出次数 
        s_iCaptureVal = 0;  //捕获值为 0
        
        TIM_SetCounter(TIM5, 0);  //设置寄存器的值为 0 
        
        s_iCaptureSts |= 0x40;    //bit6 置为 1，标记捕获到了下降沿
        
        TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);  //CC1P=0 设置为上升沿捕获
      }
    }
  }
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update);    //清除中断标志位 

}
/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitCapture 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 
* 注    意:
*********************************************************************************************************/
void  InitCapture(void) 
{
  //计数器达到最大装载值0xFFFF，会产生溢出；以72MHz/（72-1+1）=1MHz的频率计数 
  ConfigTIMxForCapture(0xFFFF, 72 - 1);   
}

/*********************************************************************************************************
* 函数名称: GetCaptureVal 
* 函数功能: 获取捕获时间，返回值为1表示捕获成功，此时*pCapVal才有意义 
* 输入参数: 
* 输出参数: pCalVal，捕获到的值的地址 
* 返 回 值: ok：1-获取成功 
* 创建日期: 
* 注    意:
*********************************************************************************************************/
u8   GetCaptureVal(i32* pCapVal)
{
  u8  ok = 0; 
  
  if(s_iCaptureSts & 0x80)              //最高位为1，表示成功捕获到了上升沿（获取到按键弹起标志）
  {
    ok = 1;                             //捕获成功 
    (*pCapVal)  = s_iCaptureSts & 0x3F; //取出低6位计数器的值赋给(*pCapVal)，得到溢出次数 
    (*pCapVal) *= 65536;//计数器计数次数为2^16=65536，乘以溢出次数，得到溢出时间总和（以1/1MHz=1us为单位） 
    (*pCapVal) += s_iCaptureVal;        //加上最后一次比较捕获寄存器的值，得到总的低电平时间 
    
    s_iCaptureSts = 0;                  //设置为0，开启下一次捕获 
  }
  
  return(ok);
}