#ifndef _IMAGE_PROCESS_h
#define _IMAGE_PROCESS_h

#include "headfile.h"

#define COL     188
#define ROW     70

extern uint8_t bin_image[ROW][COL];//二值图
extern uint8_t Ostu_Threshold;

void imageProcess(void);//图像处理

#endif
