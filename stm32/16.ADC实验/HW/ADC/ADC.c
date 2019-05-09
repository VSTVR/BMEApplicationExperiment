/*********************************************************************************************************
* ģ������: 
* ժ    Ҫ: 
* ��ǰ�汾: 1.0.0
* ��    ��: SZLY(COPYRIGHT 2018 SZLY. All rights reserved.)
* �������: 2018��01��01�� 
* ��    ��:
* ע    ��: none                                                                  
**********************************************************************************************************
* ȡ���汾: 
* ��    ��:
* �������: 
* �޸�����:
* �޸��ļ�: 
*********************************************************************************************************/

/*********************************************************************************************************
*                                              ����ͷ�ļ�
*********************************************************************************************************/
#include "stm32f10x_conf.h" 
#include "ADC.h"
#include "U16Queue.h" 
/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/

/*********************************************************************************************************
*                                              �ڲ�����
*********************************************************************************************************/
 static u16 s_arrADC1Data;   //ADC ѡ������ 
 static StructU16CirQue  s_structADCCirQue;            //ADC �� CirQue 
 static u16              s_arrADCBuf[ADC1_BUF_SIZE];   //ADC �� Buf 
/*********************************************************************************************************
*                                              �ڲ���������
*********************************************************************************************************/

static void ConfigADC1(void);     //���� ADC1 
static void ConfigDMA1Ch1(void);  //���� DMA ͨ�� 1 
static void ConfigTimer3(u16 arr, u16 psc); //���� Timer3

/*********************************************************************************************************
*                                              �ڲ�����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: ConfigADC1
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
static void ConfigADC1(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;            //����ṹ��GPIO_InitStructure����������ADC��GPIO 
  ADC_InitTypeDef   ADC_InitStructure;             //����ṹADC_InitTypeDef����������ADC���� 

  RCC_ADCCLKConfig(RCC_PCLK2_Div6);                //ADC ʱ�ӷ�Ƶ��ADCCLK = PCLK2/6=12MHz 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1  , ENABLE);  //ʹ�� ADC ʱ�� 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ�� GPIOA ʱ�� 

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1; 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);    //ADC123_IN1-PA1 
  
  ADC_InitStructure.ADC_Mode               = ADC_Mode_Independent;  //����ģʽ��DUALMOD[3:0]�� 
  ADC_InitStructure.ADC_ScanConvMode       = ENABLE;                //ģ��ת��������ɨ��ģʽ 
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;               //ģ��ת�������ڵ���ת��ģʽ 
  ADC_InitStructure.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_T3_TRGO;    //ʹ�� TIM3 �����д��� 
  ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;   //ADC �����Ҷ��� 
  ADC_InitStructure.ADC_NbrOfChannel       = 1;            //˳����й���ת���� ADC ͨ������Ŀ 
  ADC_Init(ADC1, &ADC_InitStructure); 
  
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1,  1, ADC_SampleTime_239Cycles5);

  ADC_DMACmd(ADC1, ENABLE);                       //ʹ�� ADC �� DMA ���� 
  ADC_ExternalTrigConvCmd(ADC1, ENABLE);          //ֻ��Ҫѡ�� ADC1 �������� 
  ADC_Cmd(ADC1, ENABLE);                          //ʹ�� ADC1 
  ADC_ResetCalibration(ADC1);                     //���� ADCx �ĸ�λУ׼�Ĵ��� RSTCAL��CR2�� 
  while(ADC_GetResetCalibrationStatus(ADC1));     //��ȡ ADCx ����У׼�Ĵ�����״̬,����״̬��ȴ� 
  ADC_StartCalibration(ADC1);                     //���� ADCx У׼�������üĴ��� CAL��CR2�� 
  while(ADC_GetCalibrationStatus(ADC1));          //��ȡָ�� ADC1 ��У׼����,����״̬��ȴ�

}
/*********************************************************************************************************
* ��������: ConfigDMA1Ch1
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
static void ConfigDMA1Ch1()
{
  DMA_InitTypeDef DMA_InitStructure;                      //����ṹ DMA_InitTypeDef���������� DMA ͨ�� 1 ���� 
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);      //���� DMA1 ��ʱ�� 

  DMA_DeInit(DMA1_Channel1);                              //�� DMA1_CH1 �Ĵ�������ΪĬ��ֵ 
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);           //SOURCE��ADC1 
  DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)&s_arrADC1Data;        //DESTINATION��SRAM  
  DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;           //������Ϊ SOURCE 
  DMA_InitStructure.DMA_BufferSize         = 1;                               //���û���Ĵ�С 
  DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;       //�����ַ����Ҫ���� 
  DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;            //SRAM ��ַ���� 
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //����Ϊ HalfWord ���� 
  DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;     //SRAM Ϊ HalfWord ���� 
  DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;               //ѭ��ģʽ 
  DMA_InitStructure.DMA_Priority           = DMA_Priority_Medium;             //���ȼ�Ϊ�е� 
  DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;                 //�洢�����洢��������� 
  DMA_Init(DMA1_Channel1, &DMA_InitStructure); 

  DMA_Cmd(DMA1_Channel1, ENABLE); //ʹ��ͨ�� 1
}
/*********************************************************************************************************
* ��������: ConfigTimer3
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
static  void ConfigTimer3(u16 arr, u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; //����ṹ��TIM_TimeBaseStructure���������ö�ʱ��TIM3�Ĳ���
  NVIC_InitTypeDef NVIC_InitStructure;            //����ṹ��NVIC_InitStructure�����������ж�NVIC�Ĳ���

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);          //ʹ�ܶ�ʱ��TIM3��ʱ��
  
  //��ʱ��TIM3��ʼ��
  TIM_TimeBaseStructure.TIM_Period        = arr;  //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
  TIM_TimeBaseStructure.TIM_Prescaler     = psc;  //����������ΪTIM3ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       //����ʱ�ӷָ�:TDTS = Tck_tim
  TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up; //���ö�ʱ��TIM3Ϊ���ϼ���ģʽ
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);               //����ָ���Ĳ�����ʼ��TIM2��ʱ�������λ
 
  TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update);          //ѡ�� TRGO ����ԴΪ TIMx ����ʱ��
  
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                    //ʹ��ָ���Ķ�ʱ��TIM2�жϣ�������������ж�

  //�ж����ȼ�NVIC����
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;               //���ö�ʱ��TIM2���ж�ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     //������ռ���ȼ�Ϊ0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;            //���ô����ȼ�Ϊ3��
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ж�NVIC���ж�ͨ��
  NVIC_Init(&NVIC_InitStructure);                               //���ݲ�����ʼ���ж�NVIC�ļĴ���

  TIM_Cmd(TIM3, ENABLE);                                        //ʹ�ܶ�ʱ��TIM2  
}

/*********************************************************************************************************
* ��������: TIM3_IRQHandler 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
void TIM3_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIMx �����жϷ������
  {
    TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);     //���TIMx �����жϱ�־
  }

  WriteADCBuf(s_arrADC1Data);

}
/*********************************************************************************************************
*                                              API����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: InitADC
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
void InitADC(void) 
{
  ConfigTimer3(1000,719);   //100KHz�������� 1000 Ϊ10ms (72M/720=100kHz)
  ConfigADC1();             //���� ADC1 
  ConfigDMA1Ch1();          //��������ADC1��DMA1 
  InitU16Queue(&s_structADCCirQue, s_arrADCBuf, ADC1_BUF_SIZE); //��ʼ��ADC������ 

}
/*********************************************************************************************************
* ��������: WriteADCBuf
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
u8 WriteADCBuf(u16 d)
{
  u8 ok = 0;   //��ʼ����־ 
  ok = EnU16Queue(&s_structADCCirQue, &d, 1); //��� 
  return ok;  //���ر�־ 
}
/*********************************************************************************************************
* ��������: ReadADCBuf
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
u8 ReadADCBuf(u16* p) 
{
  u8 ok = 0;  //��ʼ����־ 
  ok = DeU16Queue(&s_structADCCirQue, p, 1); //���� 
  return ok;  //���ر�־ 
}