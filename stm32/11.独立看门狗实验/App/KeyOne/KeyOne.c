/*********************************************************************************************************
* 模块名称: KeyOne.c
* 摘    要: 独立按键模块，进行独立按键初始化，以及按键扫描函数实现
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
#include "KeyOne.h"

/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/
//定义KEY1为在输入状态下I/O口的高低电平
#define KEY1    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)) 
//定义KEY2为在输入状态下I/O口的高低电平
#define KEY2    (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)) 
//定义KEY3为在输入状态下I/O口的高低电平
#define KEY3    (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))  

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/

/*********************************************************************************************************
*                                              内部变量
*********************************************************************************************************/
//按键按下时的电压，0xFF表示按下为高电平，0x00表示按下为低电平
static  u8  s_iarrKeyDownLevel[KEY_NAME_MAX];      //使用前要在InitKeyOne函数中进行初始化   

/*********************************************************************************************************
*                                              内部函数声明
*********************************************************************************************************/
static  void  ConfigKeyOneGPIO(void);             //配置按键的GPIO 

/*********************************************************************************************************
*                                              内部函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: ConfigKeyOneGPIO
* 函数功能: 配置按键的GPIO 
* 输入参数: void 
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
static  void  ConfigKeyOneGPIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  //定义结构体GPIO_InitStructure，用来配置按键的GPIO
  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //使能KEY3的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);           //使能KEY1和KEY2的时钟
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;                     //设置KEY1的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;               //设置I/O口速率为10MHz
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;                  //设置KEY1的GPIO端口的模式
  GPIO_Init(GPIOC, &GPIO_InitStructure);                          //根据参数初始化KEY1的GPIO端口
  
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;                     //设置KEY2的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;               //设置I/O口速率为10MHz
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;                  //设置KEY2的GPIO端口的模式
  GPIO_Init(GPIOC, &GPIO_InitStructure);                          //根据参数初始化KEY2的GPIO端口

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;                     //设置KEY3的引脚
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;               //设置I/O口速率为10MHz
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;                  //设置KEY3的GPIO端口的模式
  GPIO_Init(GPIOA, &GPIO_InitStructure);                          //根据参数初始化KEY3的GPIO端口
}

/*********************************************************************************************************
*                                              API函数实现
*********************************************************************************************************/
/*********************************************************************************************************
* 函数名称: InitKeyOne
* 函数功能: 初始化KeyOne
* 输入参数: void
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意:
*********************************************************************************************************/
void InitKeyOne(void)
{
  ConfigKeyOneGPIO();                                           //配置按键的GPIO 
                                                                
  s_iarrKeyDownLevel[KEY_NAME_KEY1]   = KEY_DOWN_LEVEL_KEY1;    //按键KEY1按下时为低电平
  s_iarrKeyDownLevel[KEY_NAME_KEY2]   = KEY_DOWN_LEVEL_KEY2;    //按键KEY2按下时为低电平
  s_iarrKeyDownLevel[KEY_NAME_KEY3]   = KEY_DOWN_LEVEL_KEY3;    //按键KEY3按下时为低电平
}

/*********************************************************************************************************
* 函数名称: ScanKeyOne
* 函数功能: 按键扫描，2ms或10ms扫描一次
* 输入参数: keyName-从头文件的枚举获取,OnKeyOneUp-按键弹起的响应函数的指针,OnKeyOneDown-按键按下的响应函数的指针
* 输出参数: void
* 返 回 值: void
* 创建日期: 2018年01月01日
* 注    意: 如果s_arrKeyDownLevel[keyName] = 0xFF，对s_arrKeyDownLevel[keyName]直接取反得出的是256，而非0
*           正确的做法是(u8)(~s_arrKeyDownLevel[keyName])，这样得出的才是0。
*********************************************************************************************************/
void ScanKeyOne(u8 keyName, void(*OnKeyOneUp)(void), void(*OnKeyOneDown)(void))
{
  static  u8  s_arrKeyVal[KEY_NAME_MAX];    //定义一个u8类型的数组s_arrKeyVal[],存放按键的数值
  static  u8  s_arrKeyFlag[KEY_NAME_MAX];   //定义一个u8类型的数组s_arrKeyFlag[]，存放按键标志位
  
  s_arrKeyVal[keyName] = s_arrKeyVal[keyName] << 1;   //检查是否是有效操作，防抖动，80ms内的固定稳定操作才有效

  switch (keyName)
  {
    case KEY_NAME_KEY1:
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY1;   //有效按压/弹起时，按键1的低位到高位依次变为0/1（8位）
      break;                                            
    case KEY_NAME_KEY2:                                 
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY2;   //有效按压/弹起时，按键2的低位到高位依次变为0/1（8位）
      break;                                            
    case KEY_NAME_KEY3:                                 
      s_arrKeyVal[keyName] = s_arrKeyVal[keyName] | KEY3;   //有效按压/弹起时，按键3的低位到高位依次变为0/1（8位）
      break;                                            
    default:
      break;
  }  
  
  //按键标志位为TRUE时，检测到有效的按压
  if(s_arrKeyVal[keyName] == s_iarrKeyDownLevel[keyName] && s_arrKeyFlag[keyName] == TRUE)
  {
    (*OnKeyOneDown)();                    //处理按下的响应函数
    s_arrKeyFlag[keyName] = FALSE;          //表示按键处于按下状态,按键标志位变为FALSE，去除按键卡死影响
  }
  
  //按键标志位为FALSE时，检测到有效的弹起
  else if(s_arrKeyVal[keyName] == (u8)(~s_iarrKeyDownLevel[keyName]) && s_arrKeyFlag[keyName] == FALSE)
  {
    (*OnKeyOneUp)();                      //处理弹起的响应函数
    s_arrKeyFlag[keyName] = TRUE;           //表示按键处于弹起状态,按键标志位变为TRUE，去除按键停滞影响。
  }
}
