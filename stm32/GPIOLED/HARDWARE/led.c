#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //GPIO_InitStructure��3���ӣ�pin,mode,speed
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //������������ܣ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO ���ٶ�Ϊ 50MHz���ٶ����ã�
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 
	
	//LED1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOC,GPIO_Pin_4);               
	//LED2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	GPIO_SetBits(GPIOC,GPIO_Pin_5); 
}
