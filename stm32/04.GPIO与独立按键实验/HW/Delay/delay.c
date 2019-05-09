#include "delay.h"

void delay_ms(u16 cnt)
{
  static u16 scount=0;
  
  while(scount<cnt/2)
  {
    if(Get2msFlag())  //���2ms��־״̬
    {        
      scount++;
      Clr2msFlag();   //���2ms��־
    }    
  }
  scount=0;
  
}