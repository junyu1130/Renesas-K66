/*****************************************************
���ݵ��ӿƼ���ѧ ���ܳ�ʵ����(�������ͷ) �ŷ�������ģ��
*****************************************************/
#include "servo.h"

int servo_InitalDuty = 0;
//�����ֵ
int servo_Mid = 720;
uint16_t servo_Min = 640;
uint16_t servo_Max = 840;

void Servo_Init(void)//�ŷ�����ʼ��
{
    ftm_pwm_init(servo_Port, servo_Ch, servo_Frequency, servo_Mid);
}

int Server_Control(int change_InitalDuty)//������ƣ�����������ƫ�Ƶ�ռ�ձ�
{
    servo_InitalDuty = change_InitalDuty;
    //�����ޱ���
    if(servo_InitalDuty > servo_Max)
    {
        servo_InitalDuty = servo_Max;
    }
    else if(servo_InitalDuty < servo_Min)
    {
        servo_InitalDuty = servo_Min;
    }
    //Ӧ��
    ftm_pwm_duty(servo_Port,servo_Ch,servo_InitalDuty);
    
    return 0;
}


void Servo_test(void)
{
  ftm_pwm_duty(servo_Port,servo_Ch,servo_Min);
  DelayMs(500);
  ftm_pwm_duty(servo_Port,servo_Ch,servo_Max);
  DelayMs(500);
}
