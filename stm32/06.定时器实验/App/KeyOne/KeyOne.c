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

#include <stm32f10x_conf.h> 
#include "KeyOne.h" 
/*********************************************************************************************************
*                                              �궨��
*********************************************************************************************************/

#define KEY1    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))  //���� KEY1 Ϊ������״̬�� I/O �ڵĸߵ͵�ƽ
#define KEY2    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2))  //���� KEY2 Ϊ������״̬�� I/O �ڵĸߵ͵�ƽ 
#define KEY3    (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))  //���� KEY3 Ϊ������״̬�� I/O �ڵĸߵ͵�ƽ 

/*********************************************************************************************************
*                                              �ڲ�����
*********************************************************************************************************/
 
static  u8  s_iarrKeyDownLevel[KEY_NAME_MAX];      //ʹ��ǰҪ�� InitKeyOne �����н��г�ʼ�� 
 
/*********************************************************************************************************
*                                              �ڲ���������
*********************************************************************************************************/

static  void  ConfigKeyOneGPIO(void); 

/*********************************************************************************************************
*                                              �ڲ�����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: ConfigKeyOneGPIO 
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
static  void  ConfigKeyOneGPIO(void) 
{ 
  GPIO_InitTypeDef GPIO_InitStructure;  //����ṹ�� GPIO_InitStructure���������ð����� GPIO 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //ʹ�� KEY3 ��ʱ�� 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);           //ʹ�� KEY1 �� KEY2 ��ʱ�� 

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;                     //���� KEY1 ������ 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;               //���� I/O ������Ϊ 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;                  //���� KEY1 �� GPIO �˿ڵ�ģʽ 
  GPIO_Init(GPIOC, &GPIO_InitStructure);                          //���ݲ�����ʼ�� KEY1 �� GPIO �˿� 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;                     //���� KEY2 ������ GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;               //���� I/O ������Ϊ 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;                  //���� KEY2 �� GPIO �˿ڵ�ģʽ 
  GPIO_Init(GPIOC, &GPIO_InitStructure);                          //���ݲ�����ʼ�� KEY2 �� GPIO �˿� 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;                     //���� KEY3 ������ 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;               //���� I/O ������Ϊ 10MHz 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;                  //���� KEY3 �� GPIO �˿ڵ�ģʽ 
  GPIO_Init(GPIOA, &GPIO_InitStructure);                          //���ݲ�����ʼ�� KEY3 �� GPIO �˿� 
} 
  
/*********************************************************************************************************
*                                              API����ʵ��
*********************************************************************************************************/
/*********************************************************************************************************
* ��������: InitKeyOne
* ��������: 
* �������: 
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:
*********************************************************************************************************/
void InitKeyOne(void) 
{ 
  ConfigKeyOneGPIO();                                           //���ð����� GPIO  
  s_iarrKeyDownLevel[KEY_NAME_KEY1]   = KEY_DOWN_LEVEL_KEY1;    //���� KEY1 ����ʱΪ�͵�ƽ 
  s_iarrKeyDownLevel[KEY_NAME_KEY2]   = KEY_DOWN_LEVEL_KEY2;    //���� KEY2 ����ʱΪ�͵�ƽ 
  s_iarrKeyDownLevel[KEY_NAME_KEY3]   = KEY_DOWN_LEVEL_KEY3;    //���� KEY3 ����ʱΪ�͵�ƽ 
} 

/*********************************************************************************************************
* ��������: ScanKeyOne
* ��������: ����ɨ�裬2ms �� 10ms ɨ��һ��
* �������: keyName-��ͷ�ļ���ö�ٻ�ȡ,OnKeyOneUp-�����������Ӧ������ָ��,OnKeyOneDown-�������µ���Ӧ������ ָ��
* �������: 
* �� �� ֵ: 
* ��������: 2018��01��01��
* ע    ��:��� s_arrKeyDownLevel[keyName] = 0xFF���� s_arrKeyDownLevel[keyName]ֱ��ȡ���ó����� 256������ 0 *           
��ȷ��������(u8)(~s_arrKeyDownLevel[keyName])�������ó��Ĳ��� 0
*********************************************************************************************************/

void ScanKeyOne(u8 keyName, void(*OnKeyOneUp)(void), void(*OnKeyOneDown)(void)) 
{ 
    static  u8  s_arrKeyVal[KEY_NAME_MAX];    //����һ�� u8 ���͵����� s_arrKeyVal[],��Ű�������ֵ 
    static  u8  s_arrKeyFlag[KEY_NAME_MAX];   //����һ�� u8 ���͵����� s_arrKeyFlag[]����Ű�����־λ 

    s_arrKeyVal[keyName] = s_arrKeyVal[keyName] << 1;   //����Ƿ�����Ч��������������80ms �ڵĹ̶��ȶ��������� Ч 
  
    switch (keyName) 
    {
      case KEY_NAME_KEY1: 
        s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY1;   //��Ч��ѹ/����ʱ������ 1 �ĵ�λ����λ���α�Ϊ 0/1 
        break;
      
      case KEY_NAME_KEY2:                                  
        s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY2;   //��Ч��ѹ/����ʱ������ 2 �ĵ�λ����λ���α�Ϊ 0/1 
        break;
      
      case KEY_NAME_KEY3:                                  
        s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY3;   //��Ч��ѹ/����ʱ������ 3 �ĵ�λ����λ���α�Ϊ 0/1 
        break;
      
      default: 
        break; 
    }

  
    if(s_arrKeyVal[keyName] == s_iarrKeyDownLevel[keyName] && s_arrKeyFlag[keyName] == TRUE) 
      { 
        (*OnKeyOneDown)();                    //�����µ���Ӧ���� 
        s_arrKeyFlag[keyName] = FALSE;          //��ʾ�������ڰ���״̬,������־λ��Ϊ FALSE��ȥ����������Ӱ�� 
      } 

    //������־λΪFALSE ʱ����⵽��Ч�ĵ��� 
    else if(s_arrKeyVal[keyName] == (u8)(~s_iarrKeyDownLevel[keyName]) && s_arrKeyFlag[keyName] == FALSE) 
      { 
        (*OnKeyOneUp)();                      //���������Ӧ���� 
        s_arrKeyFlag[keyName] = TRUE;           //��ʾ�������ڵ���״̬,������־λ��Ϊ TRUE��ȥ������ͣ��Ӱ�졣 
      } 

}
