#include "HC_SR04.h"

uint32_t Distance;                    //������
uint32_t timevar = 0;

/***�������������ӣ� ECHO--PE9,TRIG--PE8  ***/

/********ECHO��TRIG����I/O��ʼ��******/
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
    TRIG = 1;               //������������
    pit_delay_us(pit0,20);
    TRIG = 0;           //����һ��20us�ĸߵ�ƽ����

    while(ECHO == 0);             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
    pit_time_start  (pit0); //��ʼ��ʱ
    while(ECHO == 1)              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
    {
        Time++;
        systick_delay(1);
        if(Time>240000) break; //24MS��û�м�⵽
    };
    timevar = pit_time_get(pit0);    //ֹͣ��ʱ����ȡ��ʱʱ��
    //timevar = timevar * 340 /2/10;
    Distance = timevar*(331.4+0.607*10)/2000;  //�����¶Ȳ���
    //DelayMs(60); //��ʱ60MS��������ȷ�����Բ���,2.27�ţ��з�ӳ˵����֮���಻��ȷ��ע��֮��ͺ��ˣ����в���
}

#define SpeedControl 1

#if SpeedControl
//���뷶Χ��28-35cm
#define DistanceMax 100000
#define DistanceUp 35000
#define DistanceDown 20000
#define DistanceDown2 28000
#define DistanceSignDown 40000

#define SpeedNormal -130
int SpeedDown0=SpeedNormal+130;//��ɲ��
int SpeedDown1=SpeedNormal+60;//���ƾ������ 40
int SpeedDown2=SpeedNormal+40;//�ź�ת�����
int SpeedDown3=SpeedNormal+10;//��·�ڼ���
int SpeedUp1=SpeedNormal-30;//ת�����
int SpeedUp2=SpeedNormal-30;//���ƾ������ 40
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
    //�ڲ�·�ڹ�����ٶȿ���
    if((cross_state==cross_right||cross_state==cross_left)
      &&(distance_true<DistanceSignDown))
    {
        turn_wait = 1;
    }
    else if((cross_state==cross_right||cross_state==cross_left)//�������΢���ٹ���
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
    //�ٶȿ���
    if(turn_flag)//ת���ٶ�
    {
        wanted_speed = SpeedUp1;
    }
    else if(distance_true<DistanceDown)//��ɲ��
    {
        wanted_speed = SpeedDown0;
    }
//    else if(turn_wait)//ת���źż��ٵ�ת��Ϊֹ
//    {
//        wanted_speed = SpeedDown2;
//    }
    else if(distance_true<DistanceDown2)//���ƾ������
    {
        wanted_speed = SpeedDown1;
    }
    else if(turn_wait)//ת���źż��ٵ�ת��Ϊֹ
    {
        wanted_speed = SpeedDown2;
    }
    else if(distance_true>DistanceUp//���ƾ������
            &&distance_true<DistanceMax
            &&(!normalSpeedFlag)
            &&ABS(servoError)<=5)//ֱ��
    {
        wanted_speed = SpeedUp2;
    }
    else if(normalSpeedFlag)//��·�ڼ���
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

