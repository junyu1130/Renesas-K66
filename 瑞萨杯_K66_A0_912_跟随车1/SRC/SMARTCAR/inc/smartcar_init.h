#ifndef _SMARTCAR_INIT_H
#define _SMARTCAR_INIT_H

#include "headfile.h"

//�������������� 1�� 0��
#define encoderProtect_switch 0
//�������
#define motorControl_switch   1 
//�ٶ�PID����
#define speedControl_switch   1 
//����ͷ����
#define cameraControl_switch  1

//ͨ�Ž��յ���ָ��
#define car_go      1
#define car_stop    2
#define sign_stop   3
//��·����Ҫ��ָ��
#define cross_straight  1
#define cross_right     2
#define cross_left      3

extern int car_state;
extern int cross_state;
extern int cross_nextstate;
extern int16_t speed_p_L;
extern int16_t speed_p_R;
extern int16_t speed_i;
extern int16_t speed_d;
extern int16_t SPEED;

void car_init(void);//���ܳ���ʼ��
	
#endif
