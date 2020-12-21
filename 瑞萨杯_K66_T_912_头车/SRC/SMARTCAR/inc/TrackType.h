#ifndef __TRACKTYPE_H__
#define __TRACKTYPE_H__

#include "headfile.h"

#define turn_right  1
#define turn_left   2


extern uint8_t RCross_flag;
extern uint8_t LCross_flag;
extern uint8_t LRCross_flag;
extern uint8_t turn_flag;

typedef struct 
{
 unsigned char InflectionPointRow;//拐点的行坐标
 unsigned char InflectionPointCol;//拐点的列坐标
 unsigned char InflectionPointFlag;//拐点的标志位
 unsigned char ScanInflectionPointStart;//扫描拐点的起始行
 unsigned char ScanInflectionPointEnd;//扫描拐点结束行 
 unsigned char InflectionPointNumber;//拐点的个数
 
}InflectionPoint;//建立拐点结构体

typedef struct 
{
  unsigned char CrossLR;//十字
  unsigned char CrossL;//左岔路
  unsigned char CrossR;//右岔路
}CrossType;//建立十字类型的结构体


extern CrossType Cross;//外部申明十字结构体变量
extern InflectionPoint  InflectionPointL;//外部申明左边拐点结构体变量
extern InflectionPoint  InflectionPointR;//外部申明右边拐点结构体变量
void CrossProcess(void);
void CrossRecognition(CrossType *prt);
void FindInflectionPointR(InflectionPoint *prt ,unsigned char Start ,unsigned char End );
void FindInflectionPointL(InflectionPoint *prt ,unsigned char Start ,unsigned char End );

#endif
