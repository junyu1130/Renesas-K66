#include "headfile.h"

void buzzer_Init(void)//��������ʼ��
{
    gpio_init(buzzer_Port,GPO,0);
}

void buzzer_Ring(void)//��������һ��
{
    buzzer = 1;
    DelayMs(100);
    buzzer = 0;
}
