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
#include "EXTI.h"
#include <stm32f10x_conf.h> 
/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/

/*********************************************************************************************************
*                                              �ڲ�����
*********************************************************************************************************/
 
/*********************************************************************************************************
*                                              �ڲ���������
*********************************************************************************************************/

static void ConfigEXTIGPIO(void);   //���� EXTI ������ GPIO  
static void ConfigEXTI(void);       //���� EXTI 

/*********************************************************************************************************
*                                              �ڲ�����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: ConfigEXTIGPIO 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
static void ConfigEXTIGPIO(void) 
{
  GPIO_InitTypeDef GPIO_InitStructure;  //����ṹ�� GPIO_InitStructure,�������� EXTI ������ GPIO 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //ʹ�� KEY1 �� KEY2 ��ʱ�� 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //KEY3
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;           //���� KEY1 ������ 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;     //���� I/O ������Ϊ 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //���� KEY0 �� GPIO �˿ڵ�ģʽ 
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //���ݲ�����ʼ�� KEY1 �� GPIO �˿� 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;           //���� KEY2 ������ 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;     //���� I/O ������Ϊ 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //���� KEY0 �� GPIO �˿ڵ�ģʽ 
  GPIO_Init(GPIOC, &GPIO_InitStructure);                //���ݲ�����ʼ�� KEY2 �� GPIO �˿�

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;           //���� KEY3 ������ 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;     //���� I/O ������Ϊ 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;        //���� KEY0 �� GPIO �˿ڵ�ģʽ 
  GPIO_Init(GPIOA, &GPIO_InitStructure);                //���ݲ�����ʼ�� KEY3 �� GPIO �˿� 
}
/*********************************************************************************************************
* ��������: ConfigEXTI
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
static void ConfigEXTI(void) 
{
  EXTI_InitTypeDef EXTI_InitStructure;//����ṹ EXTI_InitStructure,�������� EXTI �������ж� 
  NVIC_InitTypeDef NVIC_InitStructure;//����ṹ NVIC_InitStructure,�������� EXTI ������ NVIC 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //���� AFIO ʱ�� 
  
  //���� KEY1 ���жϷ��� 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);   //�ж������� 
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;                    //�ж��� 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�ж� 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  
  EXTI_Init(&EXTI_InitStructure);                               //��ʼ���ж��߲���
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;              //ʹ�ܰ����ⲿ�ж�ͨ�� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //��ռ���ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //�����ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ�� 
  NVIC_Init(&NVIC_InitStructure);                               //��ʼ�� NVIC 
  
  //���� KEY2 ���жϷ��� 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);   //�ж������� 
  EXTI_InitStructure.EXTI_Line = EXTI_Line2;                    //�ж��� 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�ж� 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  EXTI_Init(&EXTI_InitStructure);        //��ʼ���ж��߲���
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;              //ʹ�ܰ����ⲿ�ж�ͨ�� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //��ռ���ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //�����ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ�� 
  NVIC_Init(&NVIC_InitStructure);                               //��ʼ�� NVIC
  
  //���� KEY3 ���жϷ��� 
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);   //�ж������� 
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;                    //�ж��� 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;           //�ж� 
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;        //�����ش��� 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;  EXTI_Init(&EXTI_InitStructure);     //��ʼ���ж��߲��� 
  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;              //ʹ�ܰ����ⲿ�ж�ͨ�� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     //��ռ���ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;            //�����ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //ʹ���ⲿ�ж�ͨ�� 
  NVIC_Init(&NVIC_InitStructure);                               //��ʼ�� NVIC 
  
  
}


/*********************************************************************************************************
* ��������: EXTI0_IRQHandler 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line0) != RESET) //����жϱ�־λ
  {
    //Ϊ����Ϊ
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4))); 
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5))); 
    
    //
    EXTI_ClearITPendingBit(EXTI_Line0);     //��� Line0 �ϵ��жϱ�־λ
  }
}
/*********************************************************************************************************
* ��������: EXTI1_IRQHandler 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET) //����жϱ�־λ
  {
    //Ϊ����Ϊ
    
    GPIO_WriteBit(GPIOC, GPIO_Pin_4, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_4))); 

    
    //
    EXTI_ClearITPendingBit(EXTI_Line2);     //��� Line0 �ϵ��жϱ�־λ
  }
}
/*********************************************************************************************************
* ��������: EXTI2_IRQHandler 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
void EXTI2_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line2) != RESET) //����жϱ�־λ
  {
    //Ϊ����Ϊ
    
    GPIO_WriteBit(GPIOC, GPIO_Pin_5, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_5))); 
    
    //
    EXTI_ClearITPendingBit(EXTI_Line2);     //��� Line0 �ϵ��жϱ�־λ
  }
}


/*********************************************************************************************************
*                                              API����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
void  InitEXTI(void) 
{
  ConfigEXTIGPIO(); //����EXTI������GPIO 
  ConfigEXTI();     //����EXTI���ж� 
}

