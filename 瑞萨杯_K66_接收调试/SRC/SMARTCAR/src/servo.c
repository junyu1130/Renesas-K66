/*****************************************************
杭州电子科技大学 智能车实验室(光电摄像头) 伺服器控制模块
*****************************************************/
#include "servo.h"

int servo_InitalDuty = 0;
//舵机中值
int servo_Mid = 720;
uint16_t servo_Min = 640;
uint16_t servo_Max = 840;

void Servo_Init(void)//伺服器初始化
{
    ftm_pwm_init(servo_Port, servo_Ch, servo_Frequency, servo_Mid);
}

int Server_Control(int change_InitalDuty)//舵机控制，输入向左右偏移的占空比
{
    servo_InitalDuty = change_InitalDuty;
    //上下限保护
    if(servo_InitalDuty > servo_Max)
    {
        servo_InitalDuty = servo_Max;
    }
    else if(servo_InitalDuty < servo_Min)
    {
        servo_InitalDuty = servo_Min;
    }
    //应用
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
