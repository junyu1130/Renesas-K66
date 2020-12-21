#include "servo.h"

//舵机中值
int servo_Mid = 715;
uint16_t servo_Min = 615;
uint16_t servo_Max = 815;

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
uint16_t  servo_KP=25;//舵机方向比例系数，影响舵机的打角范围
uint16_t  servo_KD=0;//10//7.5//舵机方向微分系数,影响舵机的打角反应
float  SteerPwmAdd=0.0;//舵机pwm增量
float  servoError;//偏差值
float  servoLastError;//上次的偏差
float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;
uint32_t SteerPwm=0,LastSteerSwm=0;//舵机的pwm值和上次舵机的pwm值
uint16_t controlCnt = 0;

float Weight[64] = {
    0,0,0,0,0,0,0,0,0,0,              //0-9行，基本用不到

    0,0,0,0,0,0,0,0,0,0,              //0-19行，基本用不到

    0,0,0,0,0,0,0,0,0,0,              //20-29行

    0,0,0,0,0,0,0,0,0,0,              //30-39行

    1,1,1,1,1,2,2,2,2,2,              //40-49行

    2,2,2,2,1,1,1,1,1,0,              //50-59行

    0,0,0,0,                          //60-63行
};

static void CalculateError(void)
{
    int i;
    CenterSum=0;
    CenterMeanValue=0;
    WeightSum=0;
    for(i=StartRow; i>25; i--)
    {
        if(ABS(MiddleLine[i]-MiddleLine[i+1])>3 && i!=StartRow)//平滑中线
        {
            MiddleLine[i]=MiddleLine[i+1];
        }
        CenterSum+=MiddleLine[i]*Weight[i];
        WeightSum+=Weight[i];
    }
    if(WeightSum!=0)
    {
        CenterMeanValue=CenterSum/WeightSum;//算出加权平均后中线的值
    }
    servoLastError=servoError;
    servoError=MidLine-CenterMeanValue;// 一场图像偏差值
}
//功能：舵机控制
void servoControl(void)
{
    if(turn_flag==0) {
        CalculateError();
        SteerPwmAdd=(servo_KP/10.0*servoError)+servo_KD/10.0*(servoError-servoLastError);//舵机的pd控制器
        if(SteerPwmAdd>=100)
            SteerPwmAdd=100;
        if(SteerPwmAdd<=-100)
            SteerPwmAdd=-100;
        SteerPwm=(uint32_t)(SteerPwmAdd+servo_Mid);
    }
    else if(turn_flag == turn_right) {
        SteerPwm = servo_Min+10;
        controlCnt++;
    }
    else if(turn_flag == turn_left) {
        SteerPwm = servo_Max-10;
        controlCnt++;
    }
    if(SteerPwm>=servo_Max)//限幅
        SteerPwm=servo_Max;
    if(SteerPwm<=servo_Min)
        SteerPwm=servo_Min;

    ftm_pwm_duty(servo_Port, servo_Ch, SteerPwm);//舵机pwm更新
    LastSteerSwm=SteerPwm;//记录pwm值
}

