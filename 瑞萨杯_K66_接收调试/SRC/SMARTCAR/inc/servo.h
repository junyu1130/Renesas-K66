#ifndef _SERVO_h
#define _SERVO_h

#include "headfile.h"

//�ŷ����ܽ�		PTB18
#define servo_Port ftm2
#define servo_Ch	 ftm_ch0

//���Ƶ��
#define servo_Frequency  50

//�����ֵ
extern int servo_Mid;
extern uint16_t servo_Min, servo_Max;

void Servo_Init(void);//�ŷ�����ʼ��
int Server_Control(int change_InitalDuty);//������ƣ�����������ƫ�Ƶ�ռ�ձ�
void Servo_test(void);
	
#endif
