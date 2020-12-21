#ifndef _SMARTCAR_INIT_H
#define _SMARTCAR_INIT_H

#include "headfile.h"

//编码器保护开关 1开 0关
#define encoderProtect_switch 0
//电机开关
#define motorControl_switch   1 
//速度PID开关
#define speedControl_switch   1 
//摄像头开关
#define cameraControl_switch  1

//通信接收到的指令
#define car_go      1
#define car_stop    2
#define sign_stop   3
//岔路口需要的指令
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

void car_init(void);//智能车初始化
	
#endif
