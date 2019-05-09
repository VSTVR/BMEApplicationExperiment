/*********************************************************************************************************
* 模块名称: DAC.c
* 摘    要: DAC模块
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
#include "DAC.h"
#include <stm32f10x_conf.h>
#include "Wave.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define DAC_DHR12R1_ADDR    ((u32)0x40007408)   //DAC1的地址（12位右对齐）

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
static StructDACWave s_strDAC1WaveBuf;  //DAC1波形缓冲区

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void ConfigTimer4(u16 arr, u16 psc);            //配置TIM4
static  void ConfigDAC1(void);                          //配置DAC1
static  void ConfigDMA2Ch3ForDAC1(StructDACWave wave);  //配置DMA2通道3传输DAC1
                                          
/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: ConfigTimer4
* 函数功能: 配置定时器Timer4
* 输入参数: arr,psc
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: DAC每两个点之间的时间差为(arr+1)*(psc+1)/72(us)
*********************************************************************************************************/
static  void ConfigTimer4(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   //配置定时器TIM4的结构体
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   //使能TIM4时钟
  
  TIM_DeInit(TIM4);                                 //重置为缺省值
  TIM_TimeBaseStructure.TIM_Period        = arr;    //设置在下一个更新事件装入活动的自动重装载寄存器周期的值   
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;    //设置用来作为TIM4时钟频率除数的预分频值    
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //设置时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //设置向上计数模式 
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                //根据指定的参数初始化TIM4的时间基数单位

  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);          //选择TRGO触发源为计时器更新事件

  TIM_Cmd(TIM4, ENABLE);                            //使能
}  

/*********************************************************************************************************
* 函数名称: ConfigDAC1
* 函数功能: 配置DAC1，DAC1通过PA4输出
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 为避免寄生干扰和额外功耗，GPIO模式建议设置为模拟输入
*********************************************************************************************************/
static  void ConfigDAC1(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;          //DAC1的GPIO配置结构体
  DAC_InitTypeDef   DAC_InitStructure;           //DAC参数结构体

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);    //使能DAC时钟
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;            //设置DAC1的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //设置I/O口速率
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;         //设置引脚的模式，模拟输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                 //根据参数初始DAC1的GPIO端口
  
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;            //TSELx[2:0]/TENx-DAC通道x触发选择
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //关闭波形发生器
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; //不屏蔽LSFR位0/三角波幅值等于1
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;   //BOFFx-使能DACx输出缓存
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);    //初始化DAC通道1

  DAC_DMACmd(DAC_Channel_1, ENABLE);              //DMAENx-使能DAC通道1的DMA模式
 
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);        //12位右对齐数据格式设置DAC值
  
  DAC_Cmd(DAC_Channel_1, ENABLE);                 //ENx-通道使能
}

/*********************************************************************************************************
* 函数名称: ConfigDMA2Ch3ForDAC1
* 函数功能: 为DAC1配置DMA2CH3
* 输入参数: wave，包括波形地址和点数
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
static  void ConfigDMA2Ch3ForDAC1(StructDACWave wave)
{  
  DMA_InitTypeDef   DMA_InitStructure;       //DMA结构体
  NVIC_InitTypeDef  NVIC_InitStructure;      //NVIC中断结构体

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);     //使能DMA2时钟
  
  DMA_DeInit(DMA2_Channel3);                 //DMA2通道13
  DMA_InitStructure.DMA_PeripheralBaseAddr      = DAC_DHR12R1_ADDR;     //DAC1的地址

  DMA_InitStructure.DMA_MemoryBaseAddr          = wave.waveBufAddr;     //波形Buf地址
  DMA_InitStructure.DMA_BufferSize              = wave.waveBufSize;     //波形Buf大小
                                              
  DMA_InitStructure.DMA_DIR                     = DMA_DIR_PeripheralDST;//数据传输方向内存到外设
  DMA_InitStructure.DMA_PeripheralInc           = DMA_PeripheralInc_Disable;  //外设地址不变
  DMA_InitStructure.DMA_MemoryInc               = DMA_MemoryInc_Enable;       //内存地址寄存器递增

  DMA_InitStructure.DMA_PeripheralDataSize      = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize          = DMA_MemoryDataSize_HalfWord;

  DMA_InitStructure.DMA_Mode                    = DMA_Mode_Circular;    //循环模式
  DMA_InitStructure.DMA_Priority                = DMA_Priority_High;    //优先级别
  DMA_InitStructure.DMA_M2M                     = DMA_M2M_Disable;      //拒绝变量相互访问
  DMA_Init(DMA2_Channel3, &DMA_InitStructure);                          //根据参数初始化DMA2通道13

  NVIC_InitStructure.NVIC_IRQChannel            = DMA2_Channel3_IRQn;   //中断通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //设置先占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //设置从优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;               //使能中断
  NVIC_Init(&NVIC_InitStructure);

  DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);  //配置DMA发送完成后产生中断

  DMA_Cmd(DMA2_Channel3, ENABLE);                  //DMA使能
}

/*********************************************************************************************************
* 函数名称: DMA2_Channel3_IRQHandler
* 函数功能: DMA2_Channel3的中断服务函数
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA2_IT_TC3))                //中断标志
  {
    NVIC_ClearPendingIRQ(DMA2_Channel3_IRQn);     //清除标志
    DMA_ClearITPendingBit(DMA2_IT_GL3);           //清除全部中断标志        
 
    ConfigDMA2Ch3ForDAC1(s_strDAC1WaveBuf);  //DAC1波形缓冲区0
  }
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitDAC
* 函数功能: DAC初始化函数 
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void InitDAC(void)
{              
  s_strDAC1WaveBuf.waveBufAddr  = (u32)GetSineWave100PointAddr(); //波形地址
  s_strDAC1WaveBuf.waveBufSize  = 100;                            //波形点数  

  ConfigDAC1();               //配置DAC1
  ConfigTimer4(59, 11);       //(arr+1)*(psc+1)/72 (us) = 60*12/72=10us   
  ConfigDMA2Ch3ForDAC1(s_strDAC1WaveBuf); //为DAC1配置DMA2通道3
}

/*********************************************************************************************************
* 函数名称: SetDACWave
* 函数功能: 设置DAC波形属性 
* 输入参数: wave，包括波形地址和点数
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
*********************************************************************************************************/
void SetDACWave(StructDACWave wave)
{
  s_strDAC1WaveBuf = wave;  //存入一个波形
}
