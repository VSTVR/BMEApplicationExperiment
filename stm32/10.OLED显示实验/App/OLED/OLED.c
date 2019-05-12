/*********************************************************************************************************
* 模块名称: 
* 摘    要: 
* 当前版本: 1.0.0
* 作    者: SZLY(COPYRIGHT 2018 SZLY. All rights reserved.)
* 完成日期: 2018年01月01日 
* 内    容:
* 注    意: none                                                                  
**********************************************************************************************************
* 取代版本: 
* 作    者:
* 完成日期: 
* 修改内容:
* 修改文件: 
*********************************************************************************************************/

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include <stm32f10x_conf.h> 
#include "OLED.h" 
#include "OLEDFont.h" 
#include "SysTick.h" 
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
#define OLED_CMD    0 //写命令 
#define OLED_DATA   1 //写数据 

//OLED 端口定义   
#define CLR_OLED_CS()   GPIO_ResetBits(GPIOB, GPIO_Pin_12)  //CS 
#define SET_OLED_CS()   GPIO_SetBits  (GPIOB, GPIO_Pin_12) 

#define CLR_OLED_RES()  GPIO_ResetBits(GPIOB, GPIO_Pin_14)  //RES 
#define SET_OLED_RES()  GPIO_SetBits  (GPIOB, GPIO_Pin_14) 

#define CLR_OLED_DC()   GPIO_ResetBits(GPIOC, GPIO_Pin_3)   //DC，命令数据标志（0-命令/1-数据） 
#define SET_OLED_DC()   GPIO_SetBits  (GPIOC, GPIO_Pin_3) 

//使用 4 线串行接口时使用  
#define CLR_OLED_SCK() GPIO_ResetBits(GPIOB, GPIO_Pin_13)   //SCK 
#define SET_OLED_SCK() GPIO_SetBits  (GPIOB, GPIO_Pin_13) 

#define CLR_OLED_DIN() GPIO_ResetBits(GPIOB, GPIO_Pin_15)   //DIN 
#define SET_OLED_DIN() GPIO_SetBits  (GPIOB, GPIO_Pin_15) 


/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
 
 static  u8  s_iOLEDGRAM[128][8];    //OLED 显存更新缓冲区 //GARM
 
/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/

static  void  ConfigOLEDGPIO(void);             //配置 OLED 的端口 
static  void  ConfigOLEDReg(void);              //配置 OLED 的寄存器 
static  void  OLEDWriteByte(u8 dat, u8 cmd);    //向 OLED 写入一个字节 
static  void  OLEDDrawPoint(u8 x, u8 y, u8 t);  //在 OLED 屏指定位置画点 
static  u32   CalcPow(u8 m, u8 n);              //计算指定位置点数 

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: ConfigOLEDGPIO
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
static  void  ConfigOLEDGPIO(void) 
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13; 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;      //推挽输出 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOB, GPIO_Pin_13); 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15; 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;      //推挽输出 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOB, GPIO_Pin_15); 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14; 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;      //推挽输出 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOB, GPIO_Pin_14); 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;      //推挽输出 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOB, GPIO_Pin_12); 
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3; 
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;      //推挽输出 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  GPIO_SetBits(GPIOC, GPIO_Pin_3); 
  
  
}
/*********************************************************************************************************
* 函数名称: ConfigOLEDReg 
* 函数功能: 配置OLED寄存器
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
static  void  ConfigOLEDReg( void ) 
{
  OLEDWriteByte(0xAE, OLED_CMD); //关闭显示 
  OLEDWriteByte(0xD5, OLED_CMD); //设置时钟分频因子,振荡频率 
  OLEDWriteByte(80,   OLED_CMD); //[3:0],分频因子;[7:4],振荡频率 
  OLEDWriteByte(0xA8, OLED_CMD); //设置驱动路数 
  OLEDWriteByte(0X3F, OLED_CMD); //默认0X3F(1/64)  
  OLEDWriteByte(0xD3, OLED_CMD); //设置显示偏移 
  OLEDWriteByte(0X00, OLED_CMD); //默认为0 
  
  OLEDWriteByte(0x40, OLED_CMD); //设置显示开始行 [5:0],行数.
  
  OLEDWriteByte(0x8D, OLED_CMD); //电荷泵设置 
  OLEDWriteByte(0x14, OLED_CMD); //bit2，开启/关闭 
  OLEDWriteByte(0x20, OLED_CMD); //设置内存地址模式 
  OLEDWriteByte(0x02, OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认 10; 
  OLEDWriteByte(0xA1, OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127; 
  OLEDWriteByte(0xC0, OLED_CMD); //设置COM 扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数 
  OLEDWriteByte(0xDA, OLED_CMD); //设置COM 硬件引脚配置 
  OLEDWriteByte(0x12, OLED_CMD); //[5:4]配置 
  
  OLEDWriteByte(0x81, OLED_CMD); //对比度设置 
  OLEDWriteByte(0xEF, OLED_CMD); //1～255;默认 0X7F (亮度设置,越大越亮) 
  OLEDWriteByte(0xD9, OLED_CMD); //设置预充电周期 
  OLEDWriteByte(0xf1, OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2; 
  OLEDWriteByte(0xDB, OLED_CMD); //设置VCOMH 电压倍率 
  OLEDWriteByte(0x30, OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
  
  OLEDWriteByte(0xA4, OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏) 
  OLEDWriteByte(0xA6, OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示    
  OLEDWriteByte(0xAF, OLED_CMD); //开启显示 
}
/*********************************************************************************************************
* 函数名称: OLEDWriteByte
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
static  void  OLEDWriteByte(u8 dat, u8 cmd) 
{
  u8 i;                         //循环计数器
  
  //判断要写入数据还是写入命令     
  if( OLED_CMD == cmd )         //标志 cmd 为传入命令时
  {
    CLR_OLED_DC(); //DC 输出低电平用来读写命令 
  }
  else if( OLED_DATA == cmd )  //标志 cmd 为传入数据时
  {
    SET_OLED_DC();              //DC 输出高电平用来读写数据 
  }
  
  CLR_OLED_CS();                //CS 输出低电平为写入数据或命令作准备
  
  for( i = 0; i < 8; i++ )      //循环八次，从高到低取出要写入的数据或命令的 8 个位 
  {
    CLR_OLED_SCK();             //SCK 输出低电平为写入数据作准备
    
    if( dat & 0x80 )            //判断要写入的数据或命令的最高位是“1”还是“0” 
    {
      SET_OLED_DIN();           //要写入的数据或命令的最高位是“1”，DIN 输出高电平表示“1”
    }
    else
    {
      CLR_OLED_DIN();           //要写入的数据或命令的最高位是“0”，DIN 输出低电平表示“0”
    }
  
    SET_OLED_SCK();             //SCK 输出高电平，DIN 的状态不再变化，此时写入数据线的数据 
    dat <<= 1;                  //左移一位，次高位移到最高位
  }
  
  SET_OLED_CS();                //OLED 的 CS 输出高电平，不再写入数据或命令   
  SET_OLED_DC();                //OLED 的 DC 输出高电平 
}

/*********************************************************************************************************
* 函数名称: OLEDDrawPoint
* 函数功能: 
* 输入参数: x:0～127; y:0～63; t:1-填充，0-清空
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:（1）x-y 轴体系的原点坐标在屏幕左上角； *           
（2）物理显示的 y 与 SSD1306 显存的 COMn 的 n 相加为 63，当然，屏幕的行号（按字节）与 SSD1306 的行号（按 *                字节）相加为 7。 
*********************************************************************************************************/
static  void  OLEDDrawPoint(u8 x, u8 y, u8 t) 
{
  u8 pos;                           //存放点所在的页数 
  u8 bx;                            //存放点所在的屏幕的行号 
  u8 temp = 0;                      //用来存放画点位置相对于字节的位 
  
  if(x > 127 || y > 63)             //如果指定位置超过额定范围 
  {
    return;
  }
  
  pos = 7 - y / 8;                  //求指定位置所在页数 
  bx = y % 8;                       //求指定位置在上面求出页数中的行号 
  temp = 1 << (7 - bx);             //（7 - bx）求出相应 SSD1306 的行号，并在字节中相应的位置为“1”
  
  if(t)
  {
    s_iOLEDGRAM[x][pos] |= temp;    //如果填充标志为‘1’，指定点填充
  }
  else
  {
    s_iOLEDGRAM[x][pos] &= ~temp; //如果填充标志为‘0’，指定点清空 
  }

}
/*********************************************************************************************************
* 函数名称: CalcPow 
* 函数功能: m^n函数
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
static  u32 CalcPow(u8 m, u8 n)
{
  u32 result = 1;     //定义用来存放结果的变量 
  
  while(n--)          //随着每次循环，n 递减，直至为 0 
  {
    result *= m;      //循环 n 次，相当于 n 个 m 相乘
  }

  return result;
  
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitOLED 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  InitOLED(void) 
{
  ConfigOLEDGPIO();     //配置OLED的GPIO 
  CLR_OLED_RES(); 
  DelayNms(10);   
  SET_OLED_RES();       //RES 引脚务必拉高 
  DelayNms(10); 
  ConfigOLEDReg();      //配置OLED的寄存器 
  OLEDClear();          //清除OLED屏内容 
}
/*********************************************************************************************************
* 函数名称: OLEDDisplayOn
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  OLEDDisplayOn( void ) 
{
  //打开关闭电荷泵，第一个字节为命令字，0x8D，第二个字节设置值，0x10-关闭电荷泵，0x14-打开电荷泵 
  OLEDWriteByte(0X8D, OLED_CMD);  //第一个字节 0x8D 为命令 
  OLEDWriteByte(0X14, OLED_CMD);  //0x14-打开电荷泵 
  
  //设置显示开关，0xAE-关闭显示，0xAF-开启显示 
  OLEDWriteByte(0XAF, OLED_CMD);  //开启显示 
}
/*********************************************************************************************************
* 函数名称: OLEDDisplayOff 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  OLEDDisplayOff( void ) 
{
  //打开关闭电荷泵，第一个字节为命令字，0x8D，第二个字节设置值，0x10-关闭电荷泵，0x14-打开电荷泵 
  OLEDWriteByte(0X8D, OLED_CMD);  //第一个字节为命令字，0x8D 
  OLEDWriteByte(0X10, OLED_CMD);  //0x10-关闭电荷泵
  
  //设置显示开关，0xAE-关闭显示，0xAF-开启显示 
  OLEDWriteByte(0XAE, OLED_CMD);  //关闭显示 
}
/*********************************************************************************************************
* 函数名称: OLEDRefreshGRAM 
* 函数功能: 
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  OLEDRefreshGRAM(void)
{
  u8 i;                                   //循环计数器1 
  u8 n;                                   //循环计数器2

  for(i = 0; i < 8; i++)                  //遍历每一页 
  {
    OLEDWriteByte(0xb0 + i, OLED_CMD);    //设置页地址（0～7） 
    OLEDWriteByte(0x00, OLED_CMD);        //设置显示位置—列低地址 
    OLEDWriteByte(0x10, OLED_CMD);        //设置显示位置—列高地址 
    for( n = 0; n < 128; n++ )            //遍历每一列 
    {
      //通过循环将OLED显存更新缓冲区的数据更新OLED屏的显存上
      OLEDWriteByte(s_iOLEDGRAM[n][i], OLED_DATA);  
    }
  }  
}
/*********************************************************************************************************
* 函数名称: OLEDClear 
* 函数功能: 清屏
* 输入参数: 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  OLEDClear(void)  
{
  u8 i;                           //循环计数器 1 
  u8 n;                           //循环计数器 2 
  
  for(i = 0; i < 8; i++)          //遍历每一页
  {
    for(n = 0; n < 128; n++)      //遍历每一列 
    {
      s_iOLEDGRAM[n][i] = 0X00;   //将指定点清零 
    }
  }

  OLEDRefreshGRAM();              //更新 OLED GRAM 
}
/*********************************************************************************************************
* 函数名称: OLEDShowChar 
* 函数功能: 在指定位置显示一个字符,包括部分字符
* 输入参数: x:0～127;y:0～63;size:选择字体 16/12;mode:0,反白显示;1,正常显示;ptn:选择取模方式 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void  OLEDShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode) 
{
  u8  temp;                           //用来存放字符顺向逐列式的相对位置 
  u8  t;                              //循环计数器 1 
  u8  t1;                             //循环计数器 2 
  u8  y0 = y;                         //当前操作的行数 
  
  chr = chr - ' ';                    //得到相对于‘’偏移的值，从而求出要打印字符在 asc1 中的行 
  
  for(t = 0; t < size; t++)           //循环逐列显示
  {
    if( size == 12 )                  //判断字号大小，选择相对的顺向逐列式 
    {
      temp = g_iASCII1206[chr][t];    //取出字符在 asc1_1206 中的第t 列 
    }
    else
    {
      temp = g_iASCII1608[chr][t];    //取出字符在 asc1_1608 中的第t 列 
    }
  
    for(t1 = 0; t1 < 8; t1++)         //#在一个字符的第 t 列的横向范围（8 个像素）内显示点
    {
      if(temp & 0x80)                 //取出 temp 的最高位，并判断为‘0’还是‘1’ 
      {
        OLEDDrawPoint(x, y, mode);    //如果 temp 的最高位为‘1’填充指定位置的点 
      }
      else
      {
        OLEDDrawPoint(x, y, !mode);   //如果 temp 的最高位为‘0’清除指定位置的点
      }
    
      temp <<= 1;                     //左移一位，次高位移到最高位 
      y++;                            //进入下一行 
    
      if((y - y0) == size)            //如果显示完一列 
      {
        y = y0;                       //行号回到原来的位置 
        x++;                          //进入下一列 
        break;                        //跳出上面带#的循环 
      }
    }
  }
}
/*********************************************************************************************************
* 函数名称: OLEDShowNum 
* 函数功能: 显示数字
* 输入参数: x,y :起点坐标;len :数字的位数;size:字体大小;mode:模式 0,填充模式;1,叠加模式 
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意:num:数值(0～4294967295) 
*********************************************************************************************************/
void  OLEDShowNum(u8 x, u8 y, u32 num, u8 len, u8 size) 
{
  u8 t;                                                         //循环计数器 
  u8 temp;                                                      //用来存放要显示数字的各个位 
  u8 enshow = 0;                                                //区分‘0’是否为高位0标志位 
  
  for(t = 0; t < len; t++)  
  {
    temp = (num / CalcPow(10, len - t - 1) ) % 10;              //按从高到低取出要显示数字的各个位，存到temp 
    
    if(enshow == 0 && t < (len - 1))                            //如果标记enshow为‘0’并且还未取到最后一位 
    {
      if(temp == 0 )                                            //如果temp等于‘0’ 
      {
        OLEDShowChar(x + (size / 2) * t, y, ' ', size, 1);      //此时的‘0’在高位，用空格替代 
        continue;                                               //提前结束本次循环，进入下一次循环 
      }
      else
      {
        enshow = 1;                                             //否则将标记enshow置为‘1’
      }
      
    }
    OLEDShowChar(x + (size / 2) * t, y, temp + '0', size, 1);   //在指定位置显示得到的数字 
  }
}
/*********************************************************************************************************
* 函数名称: OLEDShowString 
* 函数功能: 显示字符串
* 输入参数: x,y:起点坐标;*p:字符串起始地址
* 输出参数: 
* 返 回 值: 
* 创建日期: 2018年01月01日
* 注    意: 16号字体
*********************************************************************************************************/
void  OLEDShowString(u8 x, u8 y, const u8* p) 
{
  #define MAX_CHAR_POSX 122             //OLED 屏幕横向的最大范围 
  #define MAX_CHAR_POSY 58              //OLED 屏幕纵向的最大范围 
  
  while(*p != '\0')                   //指针不等于结束符时，循环进入 
  {
    if(x > MAX_CHAR_POSX)             //如果 x 超出指定最大范围，x 赋值为 0 
    {
      x  = 0;                          
      y += 16;                        //显示到下一行左端 
    }
    
    if(y > MAX_CHAR_POSY)             //如果 y 超出指定最大范围，x 和y 均赋值为 0 
    {
      y = x = 0;                      //清除 OLED 屏幕内容 
      OLEDClear();                    //显示到 OLED 屏幕左上角
    }
    
    OLEDShowChar(x, y, *p, 16, 1);    //指定位置显示一个字符 
    
    x += 8;                           //一个字符横向占 8 个像素点 
    p++;                              //指针指向下一个字符 
  }
}


