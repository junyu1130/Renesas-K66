#ifndef _BUZZER_H_
#define _BUZZER_H_

#include "headfile.h"

//蜂鸣器
#define buzzer_Port A19
#define buzzer PAout(19)

void buzzer_Init(void);//蜂鸣器初始化
void buzzer_Ring(void);//蜂鸣器响一声
#endif
