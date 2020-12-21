#ifndef _SMARTCAR_INIT_H
#define _SMARTCAR_INIT_H

#include "headfile.h"

//编码器保护开关 1开 0关
#define encoderProtect_switch 0
//电机开关
#define motorControl_switch   1 
//速度PID开关
#define speedControl_switch   1 

//通信接收到的指令
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

void car_init(void);//智能车初始化
	
#endif
