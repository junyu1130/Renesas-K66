#ifndef _ULTRASONIC_H
#define _ULTRASONIC_H

#include "headfile.h"

////���������ͳ�ʼ�����ź궨��
//#define PORT_Ultrasonic_Send A15//��ʱʹ��uart0�����������ʹ�� ����C13����
////�������������ź궨��
#define UART_Ultrasonic_Rec uart0//UART2_RX_D2_TX_D3


void Ultrasonic_Init(void);//������ģ���ʼ��
void Ultrasonic_Receive(void);//�������жϽ���ģ��

//void Ultrasonic_seecar(void);//�������㳵���

//extern uint8_t Uls_receive_flag;
//extern uint16_t Uls_Distance;
//extern uint8_t startline_cnt,startline_cnt_flag;
//extern uint8_t out_cnt,out_cnt_flag;
//extern uint8_t startline_give;
//extern uint32_t seecar_timer;
//extern uint32_t seecar_timer_set;

//extern uint32_t check_dashedfield_timer_set[2];//��ʱ���еڶ��� �����λ㳵�ж�
//extern uint32_t check_dashedfield_timer;//�㳵���о���ʱ
//extern uint8_t seecar_cnt;
//extern uint8_t seecar_process;

#endif
