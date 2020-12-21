/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
-*
-*  MK66-杭电智能车专用
-*  文件功能：      dma脉冲计数
-*  文件类型：      头文件
-*  内容说明：      适配k66 dmacnt脉冲计数功能
-*                 
-*  编写人：        高源辰
-*  工程授权给：    杭电智能车专用
-*  更新日志：
-*  最后编辑日期：  2018/2/27
-*
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#ifndef _dmacnt_h
#define _dmacnt_h

#include "headfile.h"

void DMACNT_Init(DMA_CHn CHn, PTX_n ptxn);
uint16_t DMACNT_GetValue(uint8_t DMA_Chl);

#endif
