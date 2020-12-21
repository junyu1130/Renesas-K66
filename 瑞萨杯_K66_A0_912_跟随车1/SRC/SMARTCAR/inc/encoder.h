#ifndef _ENCODER_h
#define _ENCODER_h

#include "headfile.h"

//�궨��������ܽ�

//������������
#define Left_Encoder_Pulse_PORT C18
//������������ 
#define Left_Encoder_Direction_PORT C19
//�ұ����������
#define Right_Encoder_Pulse_PORT D8
//�ұ���������� 
#define Right_Encoder_Direction_PORT D9

//�����ұ����������
#define Left_Encoder_Direction PCin(19)  //����ת1
#define Right_Encoder_Direction PDin(9)  //����ת0

//����
extern int Speed_Left,Speed_Right; //����������ֵ

//�ӿں���
void Encoder_Init(void);
void SpeedGet(void);

#endif
