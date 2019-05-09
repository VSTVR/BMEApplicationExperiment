/*********************************************************************************************************
* 模块名称: ADC.c
* 摘    要: ADC模块
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
#include "ADC.h"
#include "stm32f10x_conf.h"
#include "U16Queue.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
static u16 s_arrADC1Data;   //ADC选用数组

static StructU16CirQue  s_structADCCirQue;            //ADC的CirQue
static u16              s_arrADCBuf[ADC1_BUF_SIZE];   //ADC的Buf

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static void ConfigADC1(void);     //配置ADC1
static void ConfigDMA1Ch1(void);  //配置DMA通道1
static void ConfigTimer3(u16 arr, u16 psc); //配置Timer3

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: ConfigADC1
* 函数功能: 配置ADC
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: ADC12的通道和引脚定义
*           ADC123_IN0-PA0   ADC123_IN1-PA1   ADC123_IN2-PA2   ADC123_IN3-PA3
*           ADC12_IN4-PA4    ADC12_IN5-PA5    ADC12_IN6-PA6    ADC12_IN7-PA7    
*           ADC12_IN8-PB0    ADC12_IN9-PB1    
*           ADC123_IN10-PC0  ADC123_IN11-PC1  ADC123_IN12-PC2  ADC123_IN13-PC3  
*           ADC12_IN14-PC4   ADC12_IN15-PC5
*----------------------------------------------------------------------------------------------------------
*           ADC3的通道和引脚定义
*           ADC123_IN0-PA0   ADC123_IN1-PA1   ADC123_IN2-PA2   ADC123_IN3-PA3
*           ADC3_IN4-PF6     ADC3_IN5-PF7     ADC3_IN6-PF8     ADC3_IN7-PF9
*           ADC3_IN8-PF10    ADC12_IN9-PB1
*           ADC123_IN10-PC0  ADC123_IN11-PC1  ADC123_IN12-PC2  ADC123_IN13-PC3  
*           ADC12_IN14-PC4   ADC12_IN15-PC5
**********************************************************************************************************/
static void ConfigADC1(void)
{                          
  GPIO_InitTypeDef  GPIO_InitStructure;            //定义结构体GPIO_InitStructure，用来配置ADC的GPIO
  ADC_InitTypeDef   ADC_InitStructure;             //定义结构ADC_InitTypeDef，用来配置ADC参数

  RCC_ADCCLKConfig(RCC_PCLK2_Div6);                //ADC时钟分频，ADCCLK = PCLK2/6=12MHz
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1  , ENABLE);  //使能ADC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //使能GPIOA时钟
 
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);    //ADC123_IN1-PA1

  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;  //独立模式（DUALMOD[3:0]）
  ADC_InitStructure.ADC_ScanConvMode       = ENABLE;                //模数转换工作在扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;               //模数转换工作在单次转换模式
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T3_TRGO;    //使用TIM3来进行触发
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;   //ADC数据右对齐
  ADC_InitStructure.ADC_NbrOfChannel       = 1;            //顺序进行规则转换的ADC通道的数目
  ADC_Init(ADC1, &ADC_InitStructure);

  ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  1, ADC_SampleTime_239Cycles5);

  ADC_DMACmd(ADC1, ENABLE);                       //使能ADC的DMA传输
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);          //只需要选择ADC1触发即可
  ADC_Cmd(ADC1, ENABLE);                          //使能ADC1
  ADC_ResetCalibration(ADC1);                     //重置ADCx的复位校准寄存器RSTCAL（CR2）
  while(ADC_GetResetCalibrationStatus(ADC1));     //获取ADCx重置校准寄存器的状态,设置状态则等待
  ADC_StartCalibration(ADC1);                     //启动ADCx校准，即设置寄存器CAL（CR2）
  while(ADC_GetCalibrationStatus(ADC1));          //获取指定ADC1的校准程序,设置状态则等待
}

/*********************************************************************************************************
* 函数名称: ConfigDMA1Ch1
* 函数功能: 配置DMA通道1
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
**********************************************************************************************************/
static void ConfigDMA1Ch1(void)
{
  DMA_InitTypeDef DMA_InitStructure;                      //定义结构DMA_InitTypeDef，用来配置DMA通道1参数
  
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);      //开启DMA1的时钟
  
  DMA_DeInit(DMA1_Channel1);                              //将DMA1_CH1寄存器设置为默认值
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);           //SOURCE：ADC1
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&s_arrADC1Data;        //DESTINATION：SRAM	
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;           //外设作为SOURCE
  DMA_InitStructure.DMA_BufferSize         = 1;                               //设置缓冲的大小
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;       //外设地址不需要递增
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;            //SRAM地址递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设为HalfWord传输
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;     //SRAM为HalfWord传输
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;               //循环模式
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;             //优先级为中等
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;                 //存储器到存储器传输除能
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  DMA_Cmd(DMA1_Channel1, ENABLE); //使能通道1
}

/*********************************************************************************************************
* 函数名称: ConfigTimer3
* 函数功能: 配置定时器TIM3的参数
* 输入参数: arr：自动重装值；psc：时钟预分频数
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意:  
**********************************************************************************************************/
static void ConfigTimer3(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //TIM_TimeBaseStructure，用来配置定时器TIM3的参数
  NVIC_InitTypeDef NVIC_InitStructure;            //NVIC_InitStructure，用来配置中断NVIC的参数
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);          //TIM3时钟使能
  
  TIM_TimeBaseStructure.TIM_Period        = arr;                //arr
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;                //psc
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //设置时钟分割
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; //TIMx向上计数模式
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
  
  TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);          //选择TRGO触发源为TIMx更新时间
  
  TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);                     //使能指定的TIMx中断,允许更新中断
  
  NVIC_InitStructure.NVIC_IRQChannel      = TIM3_IRQn;          //TIMx中断
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //抢占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 1;     //子优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd   = ENABLE;             //IRQ通道被使能
  NVIC_Init(&NVIC_InitStructure);                               //初始化NVIC寄存器
  
  TIM_Cmd(TIM3, ENABLE);
}
 
/*********************************************************************************************************
* 函数名称: TIM3_IRQHandler
* 函数功能: TIM3中断服务函数
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
**********************************************************************************************************/
void TIM3_IRQHandler(void)
{  
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIMx更新中断发生与否
  {
    TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);     //清除TIMx更新中断标志 
  }  

  WriteADCBuf(s_arrADC1Data);
}

/*********************************************************************************************************
* 函数名称: InitADC
* 函数功能: 初始化ADC
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 
**********************************************************************************************************/
void InitADC(void)
{
  ConfigTimer3(1000,719);   //100KHz，计数到1000为10ms
  ConfigADC1();             //配置ADC1
  ConfigDMA1Ch1();          //配置用在ADC1的DMA1

  InitU16Queue(&s_structADCCirQue, s_arrADCBuf, ADC1_BUF_SIZE); //初始化ADC缓冲区  
}

/*********************************************************************************************************
* 函数名称: WriteADCBuf
* 函数功能: 写入ADCBuf
* 输入参数: d：写入元素
* 输出参数: void
* 返 回 值: 成功标志；1：成功；0不成功
* 创建日期: 2018年01月01日
* 注    意: 
**********************************************************************************************************/
u8 WriteADCBuf(u16 d)
{
  u8 ok = 0;   //初始化标志

  ok = EnU16Queue(&s_structADCCirQue, &d, 1); //入队
  
  return ok;  //返回标志
}

/*********************************************************************************************************
* 函数名称: ReadADCBuf
* 函数功能: 读出ADCBuf
* 输入参数: p：缓存地址
* 输出参数: void
* 返 回 值: 成功标志；1：成功；0不成功
* 创建日期: 2018年01月01日
* 注    意: 
**********************************************************************************************************/
u8 ReadADCBuf(u16* p)
{
  u8 ok = 0;  //初始化标志

  ok = DeU16Queue(&s_structADCCirQue, p, 1); //出队
  
  return ok;  //返回标志
}
