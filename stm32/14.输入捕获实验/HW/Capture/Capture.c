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
#include "Capture.h"
#include "stm32f10x_conf.h" 
/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/

/*********************************************************************************************************
*                                              �ڲ�����
*********************************************************************************************************/
 
 //s_iCaptureSts�е�bit7��Ϊ������ɵı�־��bit6������͵�ƽ��־��bit5-0������͵�ƽ��ʱ������Ĵ��� 
 static  u8  s_iCaptureSts = 0;     //����״̬ 
 static  u16 s_iCaptureVal;         //����ֵ 
 
/*********************************************************************************************************
*                                              �ڲ���������
*********************************************************************************************************/

static  void ConfigTIMxForCapture(u16 arr, u16 psc);    //���ö�ʱ��TIMx��ͨ�����벶�� 

/*********************************************************************************************************
*                                              �ڲ�����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: ConfigTIMxForCapture 
* ��������: ���ö�ʱ�� x ��ͨ�����벶�� 
* �������: arr���Զ���װֵ��psc��ʱ��Ԥ��Ƶ�� 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��: ʹ�ö�ʱ�� TIM5 ��ͨ�� 1 ��PA0�� �������벶�� ���� PA0 �ϵ͵�ƽ������ ���� KEY3 ��������ߵ�
*********************************************************************************************************/
static  void ConfigTIMxForCapture(u16 arr, u16 psc) 
{
  GPIO_InitTypeDef        GPIO_InitStructure;    //����ṹ�� GPIO_InitStructure���������ò���ͨ���� GPIO 
  TIM_TimeBaseInitTypeDef TIMx_TimeBaseStructure;//����ṹ�� TIMx_TimeBaseStructure���������ö�ʱ�� TIMx �Ĳ� �� 
  TIM_ICInitTypeDef       TIMx_ICInitStructure;  //����ṹ TIMx_ICInitStructure,������ʼ������ͨ�� 
  NVIC_InitTypeDef        NVIC_InitStructure;    //����ṹ�� NVIC_InitStructure�����������ж� NVIC �Ĳ��� 
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);                //ʹ�ܶ�ʱ�� TIMx ��ʱ�� 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);               //ʹ�ܲ���� GPIO �˿�ʱ�� 
  
  
  GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                //���ò���ͨ����Ӧ������ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;             //��������ģʽ  
  GPIO_Init(GPIOA, &GPIO_InitStructure);                    //���ݲ�����ʼ������� GPIO �˿� 
  GPIO_SetBits(GPIOA, GPIO_Pin_0);  //�������Ӧ��������Ϊ�ߵ�ƽ 
  
  //��ʱ�� TIMx ��ʼ�� 
  TIMx_TimeBaseStructure.TIM_Period        = arr;                     //�趨�������Զ���װֵ 
  TIMx_TimeBaseStructure.TIM_Prescaler     = psc;                     //���� TIMx ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
  TIMx_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;            //����ʱ�ӷָ� 
  TIMx_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;      //���ö�ʱ�� TIMx Ϊ���ϼ���ģʽ 
  TIM_TimeBaseInit(TIM5, &TIMx_TimeBaseStructure); //����ָ���Ĳ�����ʼ�� TIMx ��ʱ�������λ
  
  //���� TIMx �Ĳ���ͨ������ 
  //CC1S = 01��CC1 ͨ��������Ϊ���룬����ͨ�� IC1 ӳ�䵽��ʱ������ TI1 �� 
  TIMx_ICInitStructure.TIM_Channel        = TIM_Channel_1;  
  TIMx_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Falling;   //���ó�ʼ��Ϊ�½��ز��� 
  TIMx_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; //����Ϊֱ��ӳ�䵽 TI1 �� 
  TIMx_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;           //����ÿһ�����ض����񣬲�׽����Ƶ 
  TIMx_ICInitStructure.TIM_ICFilter       = 0x08;                     //���������˲��� 
  TIM_ICInit(TIM5, &TIMx_ICInitStructure);         //��ʼ�� TIMx ���벶����� 
  
  //�жϷ����ʼ�� 
  NVIC_InitStructure.NVIC_IRQChannel                   = TIM5_IRQn;   //���ö�ʱ�� TIMx ���ж�ͨ�� 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;           //������ռ���ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;           //���ô����ȼ� 
  NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      //ʹ���ж� NVIC ���ж�ͨ�� 
  NVIC_Init(&NVIC_InitStructure);                                     //���ݲ�����ʼ���ж� NVIC �Ĵ���

  //��������жϣ����� CC1IE �����жϣ�������Ч�źſ��Կ����жϣ� 
  TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);  


  TIM_Cmd(TIM5, ENABLE);  //ʹ�ܶ�ʱ�� TIMx
   
}

/*********************************************************************************************************
* ��������: TIM5_IRQHandler 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
void TIM5_IRQHandler(void) 
{
  if((s_iCaptureSts & 0x80) == 0) //���λΪ 0����ʾ����δ��� 
  {
    //�ߵ�ƽ����ʱ��TIMx ����������¼� 
    if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
      if(s_iCaptureSts & 0x40)    //�������������ǰһ���Ѿ����񵽵͵�ƽ 
      {
        //TIM_APR 16 λԤװ��ֵ���� CNT > 65536-1��2^16 - 1��ʱ����� 
        //��������(s_iCaptureSts & 0X3F)++���� 0x40 ������������� 0 
        if((s_iCaptureSts & 0x3F) == 0x3F)  //�ﵽ���������͵�ƽ̫��
        {
          s_iCaptureSts |= 0x80;  //ǿ�Ʊ�ǳɹ�������һ�� 
          s_iCaptureVal = 0xFFFF; //����ֵΪ 0xFFFF 
        }
        else
        {
          s_iCaptureSts++;        //��Ǽ��������һ��
        }
      }
    }
    
    if (TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET) //���������¼�
    {
      if(s_iCaptureSts & 0x40)  //bit6 Ϊ 1�����ϴβ����½��أ���ô��β���������
      {
        s_iCaptureSts |= 0x80;  //��ɲ��񣬱�ǳɹ�����һ�������� 
        s_iCaptureVal = TIM_GetCapture1(TIM5);  //s_iCaptureVa ��¼����ȽϼĴ�����ֵ
        //CC1P=1 ����Ϊ�½��ز���Ϊ�´β�����׼��
        TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Falling); 
      }
      else //bit6 Ϊ 0����ʾ�ϴ�û�����½��أ����ǵ�һ�β����½���
      {
        s_iCaptureSts = 0;  //���������� 
        s_iCaptureVal = 0;  //����ֵΪ 0
        
        TIM_SetCounter(TIM5, 0);  //���üĴ�����ֵΪ 0 
        
        s_iCaptureSts |= 0x40;    //bit6 ��Ϊ 1����ǲ������½���
        
        TIM_OC1PolarityConfig(TIM5, TIM_ICPolarity_Rising);  //CC1P=0 ����Ϊ�����ز���
      }
    }
  }
  
  TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_Update);    //����жϱ�־λ 

}
/*********************************************************************************************************
*                                              API����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: InitCapture 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 
* ע    ��:
*********************************************************************************************************/
void  InitCapture(void) 
{
  //�������ﵽ���װ��ֵ0xFFFF��������������72MHz/��72-1+1��=1MHz��Ƶ�ʼ��� 
  ConfigTIMxForCapture(0xFFFF, 72 - 1);   
}

/*********************************************************************************************************
* ��������: GetCaptureVal 
* ��������: ��ȡ����ʱ�䣬����ֵΪ1��ʾ����ɹ�����ʱ*pCapVal�������� 
* �������: 
* �������: pCalVal�����񵽵�ֵ�ĵ�ַ 
* �� �� ֵ: ok��1-��ȡ�ɹ� 
* ��������: 
* ע    ��:
*********************************************************************************************************/
u8   GetCaptureVal(i32* pCapVal)
{
  u8  ok = 0; 
  
  if(s_iCaptureSts & 0x80)              //���λΪ1����ʾ�ɹ������������أ���ȡ�����������־��
  {
    ok = 1;                             //����ɹ� 
    (*pCapVal)  = s_iCaptureSts & 0x3F; //ȡ����6λ��������ֵ����(*pCapVal)���õ�������� 
    (*pCapVal) *= 65536;//��������������Ϊ2^16=65536����������������õ����ʱ���ܺͣ���1/1MHz=1usΪ��λ�� 
    (*pCapVal) += s_iCaptureVal;        //�������һ�αȽϲ���Ĵ�����ֵ���õ��ܵĵ͵�ƽʱ�� 
    
    s_iCaptureSts = 0;                  //����Ϊ0��������һ�β��� 
  }
  
  return(ok);
}