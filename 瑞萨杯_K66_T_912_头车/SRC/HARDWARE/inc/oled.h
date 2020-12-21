/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2017,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		SEEKFREE_OLED.h
 * @brief      		0.96OLED������
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK66FX
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2017-09-19
 * @note	
					OLED���߶��壺
					------------------------------------ 
					OLEDҺ��    ��Ƭ��                        
          OLED_SCL_PIN	E28
          OLED_SDA_PIN	E27
          OLED_RST_PIN	E26
          OLED_DC_PIN	  E25
          OLED_CS_PIN	  A14

					------------------------------------ 
 ********************************************************************************************************************/



#ifndef _OELD_H
#define _OELD_H

#include "headfile.h"



//----�궨��OLED����----	 
#define  OLED_SCL_PIN	E28
#define  OLED_SDA_PIN	E27
#define  OLED_RST_PIN	E26
#define  OLED_DC_PIN	E25
#define  OLED_CS_PIN	E24


void Dly_ms(uint16_t ms);
void OLED_Init(void);
void OLED_Fill(uint8_t dat);
void OLED_WrDat(uint8_t data);
void OLED_Wr6Dat(uint8_t data);
void OLED_PutPixel(uint8_t x,uint8_t y);
void OLED_ClrPixel(uint8_t x,uint8_t y);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_P6x8Str(uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_P8x16Str(uint8_t x,uint8_t y,uint8_t ch[]);
void OLED_HEXACSII(uint16_t hex,uint8_t* Print);
void OLED_Print_Num(uint8_t x, uint8_t y, uint16_t num);
void OLED_Print_Num1(uint8_t x, uint8_t y, int16 num);
void dis_bmp(uint16_t high, uint16_t width, uint8_t *p,uint8_t value);
void dis_mybmp(uint16_t high, uint16_t width, uint8_t *p);


/********************************************************************/

#endif
