#include"head.h"

void showvoltage(u16 adc_x,float temp)
{
	lcdwrite(60,130,"CH1_VAL: ",BLUE);
	lcdwrite(60,150,"CH1_VOL: 0.000V",BLUE);
	
	while(1)
	{
		adc_x=Get_Adc_Average(ADC_Channel_1,10); //取（10次*5ms ）ms的ch1端口值的平均值
		LCD_ShowxNum(132,130,adc_x,4,16,0);
		temp=(float)adc_x*(3.3/4096);//电压的计算公式
		adc_x=temp;//adc_x为temp（电压值）的整数部分
		LCD_ShowxNum(132,150,adc_x,1,16,0);//显示电压值（取代该位1个）
		temp-=adc_x; //小数部分
		temp*=1000;
		LCD_ShowxNum(148,150,temp,3,16,0X80);//接小数部分（取代该位3个）
		LED0=!LED0;
		delay_ms(250);
	}
}