#ifndef _MOTOR_h
#define _MOTOR_h

#include "headfile.h"

struct motor_port
{
	FTMn Port;
	FTM_CHn Ch;
};

//���Ƶ��
#define motor_Frequency  15000

//�ӿں���
void Motor_Init(void);//�����ʼ��
void Motor_stop(void);//ͣ��
void Motor_Control(int left_speed,int right_speed);//���ת�ٸı�

extern struct motor_port motor_LU, motor_LD, motor_RU,motor_RD;

#endif
