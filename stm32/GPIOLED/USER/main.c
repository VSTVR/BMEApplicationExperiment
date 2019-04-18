#include"head.h"

int main(void)
{
	delay_init();
  LED_Init();
  
  while(1)
  {
    LED1=0;LED2=1;
    delay_ms(200);
    LED1=1;LED2=0;
    delay_ms(200);
  }
}

