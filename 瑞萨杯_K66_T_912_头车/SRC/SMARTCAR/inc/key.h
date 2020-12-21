#ifndef _KEY_H_
#define _KEY_H_

#include "headfile.h"

#define Key_up_Port		  D14
#define Key_up				  PDin(14)

#define Key_left_Port		D15
#define Key_left				PDin(15)

#define Key_right_Port	D13
#define Key_right				PDin(13)

#define Key_down_Port		D12
#define Key_down				PDin(12)

#define Key_1_Port			D10
#define Key_1						PDin(10)

#define Key_2_Port			D11
#define Key_2						PDin(11)

#define Keydown_left	1
#define Keydown_right	2
#define Keydown_up  	3
#define Keydown_down	4
#define Keydown_ex1		5
#define Keydown_ex2		6
#define Keydown_nokey 0

void Key_Init(void);						//按键初始化
uint8_t Key_Check(void);				//检查按下了哪个按键，并返回

#endif
