#include "delay.h"

void delay_ms(u16 cnt)
{
  static u16 scount=0;
  
  while(scount<cnt/2)
  {
    if(Get2msFlag())  //检查2ms标志状态
    {        
      scount++;
      Clr2msFlag();   //清除2ms标志
    }    
  }
  scount=0;
  
}