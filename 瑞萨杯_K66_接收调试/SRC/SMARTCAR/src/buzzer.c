#include "headfile.h"

void buzzer_Init(void)//·äÃùÆ÷³õÊ¼»¯
{
    gpio_init(buzzer_Port,GPO,0);
}

void buzzer_Ring(void)//·äÃùÆ÷ÏìÒ»Éù
{
    buzzer = 1;
    DelayMs(100);
    buzzer = 0;
}
