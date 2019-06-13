/*********************************************************************************************************
* 模块名称：
* 摘    要：
* 当前版本：1.0.0
* 作    者：SZLY(COPYRIGHT 2018 - 2020 SZLY. All rights reserved.)
* 完成日期：2020年01月01日  
* 内    容：
* 注    意：                                                                  
**********************************************************************************************************
* 取代版本：
* 作    者：
* 完成日期：
* 修改内容：
* 修改文件：
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "DAC.h"
#include "Wave.h" 
#include "stm32f10x_conf.h" 

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define DAC_DHR12R1_ADDR    ((u32)0x40007408)   //DAC1 的地址（12 位右对齐）
/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
 static StructDACWave s_strDAC1WaveBuf;  //存储 DAC1 波形属性，包括波形地址和点数
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void ConfigTimer4(u16 arr, u16 psc);            //配置 TIM4 
static  void ConfigDAC1(void);                          //配置 DAC1 
static  void ConfigDMA2Ch3ForDAC1(StructDACWave wave);  //配置 DMA2 通道 3 
/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：ConfigTimer4 
* 函数功能：
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：DAC 每两个点之间的时间差为(arr+1)*(psc+1)/72(us)，100 个点，周期即为 1ms
*********************************************************************************************************/
static  void ConfigTimer4(u16 arr, u16 psc) 
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;   //TIM_TimeBaseStructure 用于存放定时器的参数 
  //使能 RCC 相关时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   //使能定时器的时钟
  
  //配置 TIM4 
  TIM_DeInit(TIM4);                                 //重置为缺省值 
  TIM_TimeBaseStructure.TIM_Period        = arr;    //设置自动重装载值    
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;    //设置预分频器值     
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //设置时钟分割：tDTS = tCK_INT 
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;  //设置向上计数模式  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                //根据参数初始化定时器 
  
  TIM_SelectOutputTrigger(TIM4, TIM_TRGOSource_Update);          //选择更新事件为触发输入 
  
  TIM_Cmd(TIM4, ENABLE);  //使能定时器
}

/*********************************************************************************************************
* 函数名称：ConfigDAC1
* 函数功能：配置 DAC1，DAC1 通过 PA4 输出 
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：为避免寄生干扰和额外功耗，GPIO 模式建议设置为模拟输入
*********************************************************************************************************/
static  void ConfigDAC1(void) 
{
  GPIO_InitTypeDef  GPIO_InitStructure; //GPIO_InitStructure 用于存放 GPIO 的参数 
  DAC_InitTypeDef   DAC_InitStructure;  //DAC_InitStructure 用于存放 DAC 的参数 
  
  //使能 RCC 相关时钟 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使能 GPIOA 的时钟 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);    //使能 DAC 的时钟
  
  //配置 DAC1 的 GPIO 
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4;            //设置引脚 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      //设置 I/O 输出速度 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;         //设置输入类型 
  GPIO_Init(GPIOA, &GPIO_InitStructure);                 //根据参数初始化 GPIO
  
  //配置 DAC1 
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_T4_TRGO;            //设置 DAC 触发 
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None; //关闭波形发生器 
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; //不屏蔽 LSFR 位 0/三角波幅值等于 1 
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;   //使能 DAC 输出缓存 
  DAC_Init(DAC_Channel_1, &DAC_InitStructure);    //初始化 DAC 通道 1
  
  DAC_DMACmd(DAC_Channel_1, ENABLE);              //使能 DAC 通道 1 的 DMA 模式
  
  DAC_SetChannel1Data(DAC_Align_12b_R, 0);        //设置为 12 位右对齐数据格式
  
  DAC_Cmd(DAC_Channel_1, ENABLE);                 //使能 DAC 通道 1
}
/*********************************************************************************************************
* 函数名称：ConfigDMA2Ch3ForDAC1
* 函数功能：配置 DMA2 通道 3 
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
static  void ConfigDMA2Ch3ForDAC1(StructDACWave wave)
{
  DMA_InitTypeDef   DMA_InitStructure;  //DMA_InitStructure 用于存放 DMA 的参数 
  NVIC_InitTypeDef  NVIC_InitStructure; //NVIC_InitStructure 用于存放 NVIC 的参数
  
  //使能 RCC 相关时钟 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);  //使能DMA2 的时钟 
  
  //配置 DMA2_Channel3 
  DMA_DeInit(DMA2_Channel3);  //将 DMA1_CH1 寄存器设置为默认值 
  DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDR;     //设置外设地址 
  DMA_InitStructure.DMA_MemoryBaseAddr     = wave.waveBufAddr;     //设置存储器地址 
  DMA_InitStructure.DMA_BufferSize         = wave.waveBufSize;     //设置要传输的数据项数目                              
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;//设置为存储器到外设模式 
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;  //设置外设为非递增模式 
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;       //设置存储器为递增模式 
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //设置外设数据长度为半字 
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;     //设置存储器数据长度为半字 
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;    //设置为循环模式 
  DMA_InitStructure.DMA_Priority           = DMA_Priority_High;    //设置为高优先级 
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;      //禁止存储器到存储器访问 
  DMA_Init(DMA2_Channel3, &DMA_InitStructure); //根据参数初始化 DMA2_Channel3 
  
  //配置 NVIC 
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Channel3_IRQn;   //中断通道号 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //设置抢占优先级 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //设置子优先级 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能中断

  NVIC_Init(&NVIC_InitStructure);                            //根据参数初始化 NVIC 
  
  DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, ENABLE);  //使能 DMA2_Channel3 的传输完成中断
  
  DMA_Cmd(DMA2_Channel3, ENABLE);                  //使能 DMA2_Channel3
}

/*********************************************************************************************************
* 函数名称：DMA2_Channel3_IRQHandler
* 函数功能：配置 DMA2 通道 3 
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void DMA2_Channel3_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA2_IT_TC3))  //判断 DMA2_Channel3 传输完成中断是否发生
  {
    NVIC_ClearPendingIRQ(DMA2_Channel3_IRQn);  //清除 DMA2_Channel3 中断挂起 
    DMA_ClearITPendingBit(DMA2_IT_GL3);        //清除 DMA2_Channel3 传输完成中断标志
    
    ConfigDMA2Ch3ForDAC1(s_strDAC1WaveBuf);    //配置 DMA2 通道 3
  }
}
/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称：InitDAC
* 函数功能：
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void InitDAC(void)
{
  //s_strDAC1WaveBuf.waveBufAddr  = (u32)GetSineWave100PointAddr(); //波形地址 
  s_strDAC1WaveBuf.waveBufSize  = 100;                            //波形点数
  
  ConfigDAC1(); //配置 DAC1 
  ConfigTimer4(59, 11); //(arr+1)*(psc+1)/72(us)=60*12/72=10us    
  ConfigDMA2Ch3ForDAC1(s_strDAC1WaveBuf); //配置 DMA2 通道 3 
}

/*********************************************************************************************************
* 函数名称：SetDACWave
* 函数功能：
* 输入参数：
* 输出参数：
* 返 回 值：
* 创建日期：2018年01月01日
* 注    意：
*********************************************************************************************************/
void SetDACWave(StructDACWave wave) 
{
  s_strDAC1WaveBuf = wave;  //根据wave设置DAC波形属性 
}

//-------------------------------------------------------------------------------------------------------

//task
void  InitDAC_task(u8 mode)
{
  switch(mode)
  {
    case 1:
      s_strDAC1WaveBuf.waveBufAddr  = (u32)GetSineWave100PointAddr(); //波形地址（￥这里只发送正弦波）
      break;
    case 2:
      s_strDAC1WaveBuf.waveBufAddr=(u32)GetTriWave100PointAddr();
      break;
    case 3:
      s_strDAC1WaveBuf.waveBufAddr=(u32)GetRectWave100PointAddr();
      break;
    default:
      break;
  }
}

