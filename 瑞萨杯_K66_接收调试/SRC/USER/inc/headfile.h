#ifndef _headfile_h
#define _headfile_h


#include "port_cfg.h"
#include "common.h"
#include "misc.h"
#include <stdio.h>
#include <string.h>

//--------函数库--------
#include "gpio.h"
#include "systick.h"
#include "port.h"
#include "uart.h"
#include "pit.h"
#include "misc.h"
#include "ftm.h"
#include "dma.h"
#include "isr.h"

//智能车初始化
#include "smartcar_init.h"

//智能车模块
#include "buzzer.h"       //蜂鸣器
#include "key.h"          //按键
#include "nrf2401.h"		  //无线通讯模块
#include "oled.h"         //oled显示屏



//宏定义函数名
#define DelayMs systick_delay_ms

#endif
