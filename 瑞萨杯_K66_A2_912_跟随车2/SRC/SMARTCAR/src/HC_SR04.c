#include "HC_SR04.h"

uint32_t Distance;                    //检测距离
uint32_t timevar = 0;

/***超声波引脚连接： ECHO--PE9,TRIG--PE8  ***/

/********ECHO和TRIG引脚I/O初始化******/
void HC_SR04_Init(void)
{
    gpio_init(TRIG_Port,GPO,0);
    gpio_init(ECHO_Port,GPI,0);
}

void distance(void)
{
    uint32_t Time = 0;
    TRIG = 0;
    pit_delay_us(pit0,10);
    TRIG = 1;               //产生触发脉冲
    pit_delay_us(pit0,20);
    TRIG = 0;           //产生一个20us的高电平脉冲

    while(ECHO == 0);             //等待电平变高，低电平一直等待
    pit_time_start  (pit0); //开始计时
    while(ECHO == 1)              //等待电平变低，高电平一直等待
    {
        Time++;
        systick_delay(1);
        if(Time>240000) break; //24MS还没有检测到
    };
    timevar = pit_time_get(pit0);    //停止计时，获取计时时间
    //timevar = timevar * 340 /2/10;
    Distance = timevar*(331.4+0.607*10)/2000;  //加上温度补偿
    //DelayMs(60); //延时60MS，测距更精确，可以不用,2.27号：有反映说加上之后测距不精确，注释之后就好了，自行测试
}

#define SpeedControl 1

#if SpeedControl
//距离范围在28-35cm
#define DistanceMax 100000
#define DistanceUp 35000
#define DistanceDown 20000
#define DistanceDown2 28000
#define DistanceSignDown 40000

#define SpeedNormal -130
int SpeedDown0=SpeedNormal+130;//急刹车
int SpeedDown1=SpeedNormal+60;//控制距离减速 40
int SpeedDown2=SpeedNormal+40;//信号转弯减速
int SpeedDown3=SpeedNormal+10;//岔路口减速
int SpeedUp1=SpeedNormal-30;//转弯加速
int SpeedUp2=SpeedNormal-30;//控制距离加速 40
#endif

int wanted_speed = SpeedNormal;
uint8_t turn_wait = 0;
uint8_t normalSpeedFlag = 0;
extern int distance_true;
extern int signFrameFlag;
extern float  servoError;
extern uint32_t signCnt;

void distanceControl(void)
{
    signCnt++;
#if SpeedControl
    //在岔路口拐弯的速度控制
    if((cross_state==cross_right||cross_state==cross_left)
      &&(distance_true<DistanceSignDown))
    {
        turn_wait = 1;
    }
    else if((cross_state==cross_right||cross_state==cross_left)//这种情况微减速拐弯
            &&signFrameFlag
            &&signCnt>20)// 60/3
    {
        normalSpeedFlag = 1;
    }
    else if(turn_flag)
    {
        turn_wait = 0;
        normalSpeedFlag = 0;
    }
    //速度控制
    if(turn_flag)//转弯速度
    {
        wanted_speed = SpeedUp1;
    }
    else if(distance_true<DistanceDown)//急刹车
    {
        wanted_speed = SpeedDown0;
    }
//    else if(turn_wait)//转弯信号减速到转弯为止
//    {
//        wanted_speed = SpeedDown2;
//    }
    else if(distance_true<DistanceDown2)//控制距离减速
    {
        wanted_speed = SpeedDown1;
    }
    else if(turn_wait)//转弯信号减速到转弯为止
    {
        wanted_speed = SpeedDown2;
    }
    else if(distance_true>DistanceUp//控制距离加速
            &&distance_true<DistanceMax
            &&(!normalSpeedFlag)
            &&ABS(servoError)<=5)//直道
    {
        wanted_speed = SpeedUp2;
    }
    else if(normalSpeedFlag)//岔路口减速
    {
        wanted_speed = SpeedDown3;
    } 
    else
    {
        wanted_speed = SpeedNormal;
    }
#endif
    pid_speed_L->wantout = pid_speed_R->wantout = wanted_speed;
}

