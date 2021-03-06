/*********************************************************************************************************
* 模块名称: 
* 摘    要: 
* 当前版本: 1.0.0
* 作    者: SZLY(COPYRIGHT 2018 SZLY. All rights reserved.)
* 完成日期: 2018年01月01日
* 内    容: 
* 注    意:                                                                  
**********************************************************************************************************
* 取代版本: 
* 作    者:
* 完成日期: 
* 修改内容:
* 修改文件: 
*********************************************************************************************************/

#ifndef _KEY_ONE_H_ 
#define _KEY_ONE_H_ 

/*********************************************************************************************************
*                                              包含头文件
*********************************************************************************************************/
#include "DataType.h" 
/*********************************************************************************************************
*                                              宏定义
*********************************************************************************************************/

#define  KEY_DOWN_LEVEL_KEY1    0x00     //0x00 表示按下为低电平 
#define  KEY_DOWN_LEVEL_KEY2    0x00     //0x00 表示按下为低电平 
#define  KEY_DOWN_LEVEL_KEY3    0x00     //0x00 表示按下为低电平 

/*********************************************************************************************************
*                                              枚举结构体定义
*********************************************************************************************************/
typedef enum 
{ 
  KEY_NAME_KEY1 = 0,  //按键1 
  KEY_NAME_KEY2,      //按键 2 对应1
  KEY_NAME_KEY3,      //按键 3 对应2
  KEY_NAME_MAX 
}EnumKeyOneName; 
/*********************************************************************************************************
*                                              API函数声明
*********************************************************************************************************/
void  InitKeyOne(void);                                                          //初始化驱动 
void  ScanKeyOne(u8 keyName, void(*OnKeyOneUp)(void), void(*OnKeyOneDown)(void));//扫描按键，2ms 或 10ms 调用 
#endif
