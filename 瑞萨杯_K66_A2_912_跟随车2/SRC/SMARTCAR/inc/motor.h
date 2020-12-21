#ifndef _MOTOR_h
#define _MOTOR_h

#include "headfile.h"

struct motor_port
{
	FTMn Port;
	FTM_CHn Ch;
};

//电机频率
#define motor_Frequency  15000

//接口函数
void Motor_Init(void);//电机初始化
void Motor_stop(void);//停车
void Motor_Control(int left_speed,int right_speed);//电机转速改变

extern struct motor_port motor_LU, motor_LD, motor_RU,motor_RD;

#endif
