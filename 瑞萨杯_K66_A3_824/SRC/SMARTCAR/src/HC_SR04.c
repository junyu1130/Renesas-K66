#include "HC_SR04.h"

uint16_t Distance;                    //������
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

//���뷶Χ��28-33cm
#define DistanceMax 65000
#define DistanceUp 33000//���ټ�࣬�����ܼ��ٺ�̫��
#define DistanceMin 10000
#define DistanceDown 28000//�ڹ��䴦�ܹ���ʱ����
#define SpeedNormal -131

#define SpeedControl 1
#define SpeedTest 0

int SpeedDown1=SpeedNormal+81;//�����С���� 71
int SpeedDown2=SpeedNormal+47;//��־����� 46
int SpeedUp1=SpeedNormal-32;//ת�����
int SpeedUp2=SpeedNormal-23;//���̾������ 19

int wanted_speed = SpeedNormal;
extern uint16_t distance_true;
extern int signFrameFlag;

void distanceControl(void)
{
#if SpeedControl
    if(distance_true<DistanceDown && distance_true>DistanceMin)//�������ȼ���ߣ�����ɲ��
    {
        wanted_speed = SpeedDown1;
    }
    else if(turn_flag)//ת�����
    {
        wanted_speed = SpeedUp1;
    }
    else if(signFrameFlag)//������־��Ҳ����
    {
        wanted_speed = SpeedDown2;
    }
    else if(distance_true>DistanceUp && distance_true<DistanceMax)//����̫�����
    {
        wanted_speed = SpeedUp2;
    }
    else
    {
        wanted_speed = SpeedNormal;
    }
#endif
    
#if SpeedTest
    if(turn_flag)//ת�����
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



