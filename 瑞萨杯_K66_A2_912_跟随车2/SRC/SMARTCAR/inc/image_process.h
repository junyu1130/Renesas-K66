#ifndef _IMAGE_PROCESS_h
#define _IMAGE_PROCESS_h

#include "headfile.h"

#define COL     188
#define ROW     64

#define WHITE 1
#define BLACK 0

extern uint8_t bin_image[ROW][COL];//��ֵͼ
extern uint8_t threshold;

void imageProcess(void);//ͼ����

#endif
