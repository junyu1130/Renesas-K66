#include "HC_SR04.h"

uint16_t Distance;                    //检测距离
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

//距离范围在28-33cm
#define DistanceMax 65000
#define DistanceUp 33000//减少间距，但不能加速后太近
#define DistanceMin 10000
#define DistanceDown 28000//在拐弯处能够及时减速
#define SpeedNormal -131

#define SpeedControl 1
#define SpeedTest 0

int SpeedDown1=SpeedNormal+81;//距离过小减速 71
int SpeedDown2=SpeedNormal+47;//标志物减速 46
int SpeedUp1=SpeedNormal-32;//转弯加速
int SpeedUp2=SpeedNormal-23;//缩短距离加速 19

int wanted_speed = SpeedNormal;
extern uint16_t distance_true;
extern int signFrameFlag;

void distanceControl(void)
{
#if SpeedControl
    if(distance_true<DistanceDown && distance_true>DistanceMin)//减速优先级最高，紧急刹车
    {
        wanted_speed = SpeedDown1;
    }
    else if(turn_flag)//转弯加速
    {
        wanted_speed = SpeedUp1;
    }
    else if(signFrameFlag)//看到标志物也减速
    {
        wanted_speed = SpeedDown2;
    }
    else if(distance_true>DistanceUp && distance_true<DistanceMax)//距离太大加速
    {
        wanted_speed = SpeedUp2;
    }
    else
    {
        wanted_speed = SpeedNormal;
    }
#endif
    
#if SpeedTest
    if(turn_flag)//转弯加速
    {
        wanted_speed = SpeedUp1;
    }
    else
    {
        wanted_speed = SpeedNormal;
    }
#endif
    pid_speed_L->wantout = pid_speed_R->wantout = wanted_speed;
}



