#ifndef _SMARTCAR_INIT_H
#define _SMARTCAR_INIT_H

#include "headfile.h"

//�������������� 1�� 0��
#define encoderProtect_switch 0
//�������
#define motorControl_switch   1 
//�ٶ�PID����
#define speedControl_switch   1 

//ͨ�Ž��յ���ָ��
#define signal_go         1
#define signal_stop       2
#define signal_straight   3
#define signal_right      4
#define signal_left       5
#define traffic_stop      6

extern int16_t speed_p_L;
extern int16_t speed_p_R;
extern int16_t speed_i;
extern int16_t speed_d;
extern int16_t SPEED;

void car_init(void);//���ܳ���ʼ��
	
#endif
