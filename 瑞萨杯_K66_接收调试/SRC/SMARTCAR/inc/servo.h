#ifndef _SERVO_h
#define _SERVO_h

#include "headfile.h"

//伺服器管脚		PTB18
#define servo_Port ftm2
#define servo_Ch	 ftm_ch0

//舵机频率
#define servo_Frequency  50

//舵机中值
extern int servo_Mid;
extern uint16_t servo_Min, servo_Max;

void Servo_Init(void);//伺服器初始化
int Server_Control(int change_InitalDuty);//舵机控制，输入向左右偏移的占空比
void Servo_test(void);
	
#endif
