#ifndef _headfile_h
#define _headfile_h


#include "port_cfg.h"
#include "common.h"
#include "misc.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

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
#include "servo.h"		    //伺服器
#include "motor.h"		    //电机
#include "control.h"		  //控制
#include "dmacnt.h"			  //dma脉冲计数
#include "encoder.h"		  //编码器
#include "buzzer.h"       //蜂鸣器
#include "key.h"          //按键
#include "nrf2401.h"		  //无线通讯模块
#include "MT9V032.h"      //摄像头
#include "oled.h"         //oled显示屏
#include "image_process.h"//图像处理部分
#include "FindTrack.h"    //赛道处理边线  
#include "TrackType.h"    //判断赛道类型
#include "HC_SR04.h"      //超声波测距



//宏定义函数名
#define DelayMs systick_delay_ms
#define DelayUs systick_delay_us

#endif
