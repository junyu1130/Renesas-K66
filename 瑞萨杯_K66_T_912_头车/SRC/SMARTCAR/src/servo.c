#include "servo.h"

//舵机中值
int servo_Mid = 685;
uint16_t servo_Min = 585;
uint16_t servo_Max = 785;

void Servo_Init(void)//伺服器初始化
{
    ftm_pwm_init(servo_Port, servo_Ch, servo_Frequency, servo_Mid);
}

int Server_Control(int change_InitalDuty)//舵机控制，输入向左右偏移的占空比
{
    int servo_InitalDuty = 0;
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

/*********define for SteerControl**********/
#define MidLine 86
#define turn_right 1
#define turn_left  2

int16_t  servo_KP=25;//舵机方向比例系数，影响舵机的打角范围
int16_t  servo_KD=35;//舵机方向微分系数,影响舵机的打角反应
int32_t  SteerPwmAdd=0;//舵机pwm增量
int32_t  servoError;//偏差值
int32_t  servoLastError;//上次的偏差
uint8_t  CenterMeanValue=0;
int32_t SteerPwm=0;//舵机的pwm值
uint8_t turn_flag = 0;

//功能：舵机控制
void servoControl(void)
{
    if(CenterMeanValue!=0)
    {
        if(turn_flag==0) {
            servoLastError=servoError;
            servoError=MidLine-CenterMeanValue;// 一场图像偏差值
            SteerPwmAdd=(servo_KP/10.0*servoError)+servo_KD/10.0*(servoError-servoLastError);//舵机的pd控制器
            if(SteerPwmAdd>=100)
                SteerPwmAdd=100;
            if(SteerPwmAdd<=-100)
                SteerPwmAdd=-100;
            SteerPwm=(uint32_t)(SteerPwmAdd+servo_Mid);
        }
        else if(turn_flag == turn_right) {
            SteerPwm = servo_Min+25;//17
        }
        else if(turn_flag == turn_left) {
            SteerPwm = servo_Max-15;
        }
        if(SteerPwm>=servo_Max)//限幅
            SteerPwm=servo_Max;
        if(SteerPwm<=servo_Min)
            SteerPwm=servo_Min;

        ftm_pwm_duty(servo_Port, servo_Ch, SteerPwm);//舵机pwm更新
    }
}
