#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //GPIO_InitStructure
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出（节能）
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //IO 口速度为 50MHz（速度设置）
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能 PC 端口时钟（挂载在APB2总线上）
	
	//LED1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; //
	GPIO_Init(GPIOC, &GPIO_InitStructure); //          
	//GPIO_SetBits(GPIOC,GPIO_Pin_4); //            
	//LED2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //
	GPIO_Init(GPIOC, &GPIO_InitStructure); //
	//GPIO_SetBits(GPIOC,GPIO_Pin_5); //
}