#ifndef _ENCODER_h
#define _ENCODER_h

#include "headfile.h"

//宏定义编码器管脚

//左编码器脉冲口
#define Left_Encoder_Pulse_PORT C18
//左编码器方向口 
#define Left_Encoder_Direction_PORT C19
//右编码器脉冲口
#define Right_Encoder_Pulse_PORT D8
//右编码器方向口 
#define Right_Encoder_Direction_PORT D9

//读左右编码器方向口
#define Left_Encoder_Direction PCin(19)  //左正转1
#define Right_Encoder_Direction PDin(9)  //右正转0

//变量
extern int Speed_Left,Speed_Right; //编码器现在值

//接口函数
void Encoder_Init(void);
void SpeedGet(void);

#endif
