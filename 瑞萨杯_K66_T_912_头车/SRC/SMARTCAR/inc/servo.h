#ifndef _SERVO_h
#define _SERVO_h

#include "headfile.h"

//伺服器管脚		PTB18
#define servo_Port ftm2
#define servo_Ch	 ftm_ch0

//舵机频率
#define servo_Frequency  50

extern int16_t  servo_KP;
extern int16_t  servo_KD;

void Servo_Init(void);//伺服器初始化
int Server_Control(int change_InitalDuty);//舵机控制，输入向左右偏移的占空比
void Servo_test(void);
void servoControl(void);//按中线控制舵机
	
#endif
