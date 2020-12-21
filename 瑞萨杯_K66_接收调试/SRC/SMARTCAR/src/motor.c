#include "motor.h"

struct motor_port motor_LU, motor_LD, motor_RU,motor_RD;

int motor_left_speed_u = 0,motor_left_speed_d = 0,motor_right_speed_u = 0,motor_right_speed_d = 0;//电机占空比

void Motor_Init(void)//电机初始化
{
    ftm_pwm_init(motor_LU.Port, motor_LU.Ch, motor_Frequency, 0);
    ftm_pwm_init(motor_LD.Port, motor_LD.Ch, motor_Frequency, 0);
    ftm_pwm_init(motor_RU.Port, motor_RU.Ch, motor_Frequency, 0);
    ftm_pwm_init(motor_RD.Port, motor_RD.Ch, motor_Frequency, 0);
}

void Motor_stop(void)//停车
{
    motor_right_speed_u = 0;
    motor_left_speed_u = 0;
    motor_left_speed_d = 0;
    motor_right_speed_d = 0;
    //应用转速
    ftm_pwm_duty(motor_LU.Port, motor_LU.Ch, motor_left_speed_u);
    ftm_pwm_duty(motor_RU.Port, motor_RU.Ch, motor_right_speed_u);
    ftm_pwm_duty(motor_LD.Port, motor_LD.Ch, motor_left_speed_d);
    ftm_pwm_duty(motor_RD.Port, motor_RD.Ch, motor_right_speed_d);
}

void Motor_Control(int left_speed,int right_speed)//电机转速改变
{

    //左轮
    if(left_speed >= 0 && left_speed <= 9999)
    {
        motor_left_speed_u = left_speed;
        motor_left_speed_d = 0;
    }
    else if(left_speed < 0 && left_speed >= -9999)
    {
        motor_left_speed_d = -left_speed;
        motor_left_speed_u = 0;
    }
    else
    {
        //溢出
        Motor_stop();
//        OLED_Clear();
//        OLED_Write_String(0,1,(uint8_t*)"left ");
//        OLED_Write_String(2,3,(uint8_t*)"Motor Control");
//        OLED_Write_String(1,5,(uint8_t*)"has a big error");
        while(1);
    }
    //右轮
    if(right_speed >= 0 && right_speed <= 9999)
    {
        motor_right_speed_u = right_speed;
        motor_right_speed_d = 0;
    }
    else if(right_speed < 0 && right_speed >= -9999)
    {
        motor_right_speed_d = -right_speed;
        motor_right_speed_u = 0;
    }
    else
    {
        //溢出
        Motor_stop();
//        OLED_Clear();
//        OLED_Write_String(0,1,(uint8_t*)"right");
//        OLED_Write_String(2,3,(uint8_t*)"Motor Control");
//        OLED_Write_String(1,5,(uint8_t*)"has a big error");
        while(1);
    }
    //应用转速
    ftm_pwm_duty(motor_LU.Port, motor_LU.Ch, motor_left_speed_u);
    ftm_pwm_duty(motor_RU.Port, motor_RU.Ch, motor_right_speed_u);
    ftm_pwm_duty(motor_LD.Port, motor_LD.Ch, motor_left_speed_d);
    ftm_pwm_duty(motor_RD.Port, motor_RD.Ch, motor_right_speed_d);
}
