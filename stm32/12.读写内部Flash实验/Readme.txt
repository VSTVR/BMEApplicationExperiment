用按键向flash写入数据

ProcKeyOne.c 
Flash.c

任务
基于STM32核心板，编写程序实现密码解锁功能，具体而言：微控制器初始密码为“0x12345678”，该密码通过 STM32FlashWriteWord 函数写入到内部 Flash（切勿写入到代码 区），通过按下 Key1 模拟输入密码为“0x12345678”，通过按下 Key2 模拟输入密码为 “0x87654321” ， 通过按下 Key3 进行密码匹配， 如果密码正确， 则在 OLED 上显示 “Success！ ” ， 否则，如果密码不正确，则在 OLED 上显示“Failure！”