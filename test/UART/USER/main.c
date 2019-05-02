#include"head.h"

int main(void)
{
	u8 t;u8 len;u16 time=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置优先级
	delay_init();
	uart_init(115200);
	LED_Init();
	
	while(1)
	{
		if(USART_RX_STA&0x8000)       //判断最高位是1还是0，如果端口有数据
		{
			len=USART_RX_STA&0x3fff;  //获得数据的长度
			printf("\n您发送的消息为：\n");
      
			u8 num=1;
      u8 i;
      for(i=0;i<len;i++)
      {
        u16 tempval;
        tempval=(u16)USART_RX_BUF[i];
        num+=pow(10,len-i-1)*tempval;
      }
      
			USART1->DR=(u8)num; //把数据发送到输出端口，直接显示
			 //每次录入数组都得等上一个录完
			
			
      
			printf("\n");
			USART_RX_STA=0; //清空缓存区
      LED2=!LED2;
			delay_ms(500);
		}else{
			time++;
			if(time%5000==0)
			{
				printf("请输入想要发送的数据：\n");
			}
			if(time%500==0) LED1=!LED1;
			delay_ms(1);    //1个time为1ms
		}
	}

}

/*
u8 t;u8 len;u16 time=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置优先级
	delay_init();
	uart_init(115200);
	LED_Init();
	
	while(1)
	{
		if(USART_RX_STA&0x8000)       //如果端口有数据
		{
			len=USART_RX_STA&0x3fff;  //获得数据的长度
			printf("\n您发送的消息为：\n");
			for(t=0;t<len;t++)
			{
				USART1->DR=USART_RX_BUF[t];
				while((USART1->SR&0X40)==0); //每次录入数组都得等上一个录完
			}
			printf("\n");
			USART_RX_STA=0;
			delay_ms(500);
		}else{
			time++;
			if(time%5000==0)
			{
				printf("请输入想要发送的数据：\n");
			}
			if(time%500==0) LED1=!LED1;
			delay_ms(1);    //1个time为1ms
		}
	}

*/
