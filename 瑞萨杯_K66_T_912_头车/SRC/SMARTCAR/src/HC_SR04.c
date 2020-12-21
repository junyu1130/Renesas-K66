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
        if(Time>340000) break; //24MS��û�м�⵽
    };
    timevar = pit_time_get(pit0);    //ֹͣ��ʱ����ȡ��ʱʱ��
    //timevar = timevar * 340 /2/10;
    Distance = timevar*(331.4+0.607*10)/2000;  //�����¶Ȳ���
    //DelayMs(60); //��ʱ60MS��������ȷ�����Բ���,2.27�ţ��з�ӳ˵����֮���಻��ȷ��ע��֮��ͺ��ˣ����в���
}

