#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include "headfile.h"

////超声波发送初始化引脚宏定义
//#define PORT_Ultrasonic_Send A15//暂时使用uart0，如果今后继续使用 需用C13引脚
////超声波接收引脚宏定义
#define UART_Ultrasonic_Rec uart0//UART2_RX_D2_TX_D3


void Ultrasonic_Init(void);//超声波模块初始化
void Ultrasonic_Receive(void);//超声波中断接受模块

//void Ultrasonic_seecar(void);//超声波汇车检测

//extern uint8_t Uls_receive_flag;
//extern uint16_t Uls_Distance;
//extern uint8_t startline_cnt,startline_cnt_flag;
//extern uint8_t out_cnt,out_cnt_flag;
//extern uint8_t startline_give;
//extern uint32_t seecar_timer;
//extern uint32_t seecar_timer_set;

//extern uint32_t check_dashedfield_timer_set[2];//延时进行第二次 第三次汇车判断
//extern uint32_t check_dashedfield_timer;//汇车区判决计时
//extern uint8_t seecar_cnt;
//extern uint8_t seecar_process;

#endif
