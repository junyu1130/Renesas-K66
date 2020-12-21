
#ifndef _OLED_H_
#define _OLED_H_

#include "headfile.h"
#include "stdint.h"
//#include "MK60DZ10.h"
//#include "extern.h"
//#include "data.h"

/******************** Device Config ********************/

/* OLED�˿ں궨�� */
//#define OLED_PORT  PTE
//#define OLED_SCL   28     //D0
//#define OLED_SDA   27    //D1
//#define OLED_RST   26    //RST
//#define OLED_DC    25    //DC

#define OLED_SCL   E28     //D0
#define OLED_SDA   E27    //D1
#define OLED_RST   E26    //RST
#define OLED_DC    E25    //DC

#define Font   0//1�Ǵ����� 0��С���� С����Ƚ�ʡ�ռ�


/* OLED�ֱ��ʺ궨�� */
#define  X_WIDTH  128
#define  Y_WIDTH  64



/* OLED IO������滻 */
//#define OLED_D0_OL		OLED_PORT->PCOR |= (1<<OLED_SCL)//D0 IO������͵�ƽ
//#define OLED_D0_OH		OLED_PORT->PSOR |= (1<<OLED_SCL)//D0 IO������ߵ�ƽ

//#define OLED_D1_OL		OLED_PORT->PCOR |= (1<<OLED_SDA)	//D1 IO������͵�ƽ
//#define OLED_D1_OH		OLED_PORT->PSOR |= (1<<OLED_SDA)	//D1 IO������ߵ�ƽ

//#define OLED_RST_OL		OLED_PORT->PCOR |= (1<<OLED_RST)	//RST IO������͵�ƽ
//#define OLED_RST_OH		OLED_PORT->PSOR |= (1<<OLED_RST)	//RST IO������ߵ�ƽ

//#define OLED_DC_OL		OLED_PORT->PCOR |= (1<<OLED_DC)	//DC IO������͵�ƽ
//#define OLED_DC_OH		OLED_PORT->PSOR |= (1<<OLED_DC)	//DC IO������ߵ�ƽ

#define OLED_D0_OL		gpio_set(OLED_SCL,0)//D0 IO������͵�ƽ
#define OLED_D0_OH		gpio_set(OLED_SCL,1)//D0 IO������ߵ�ƽ

#define OLED_D1_OL		gpio_set(OLED_SDA,0)	//D1 IO������͵�ƽ
#define OLED_D1_OH		gpio_set(OLED_SDA,1)	//D1 IO������ߵ�ƽ

#define OLED_RST_OL		gpio_set(OLED_RST,0)	//RST IO������͵�ƽ
#define OLED_RST_OH		gpio_set(OLED_RST,1)	//RST IO������ߵ�ƽ

#define OLED_DC_OL		gpio_set(OLED_DC ,0)	//DC IO������͵�ƽ
#define OLED_DC_OH		gpio_set(OLED_DC ,1)	//DC IO������ߵ�ƽ


/* Function Prototypes */
/*
 *  ����д��
 */
void OLED_WrDat(uint8_t data);

/*
 *  ����д��
 */
void OLED_WrCmd(uint8_t cmd);

/*
 *  ��������
 */
void OLED_Set_XY(uint8_t x, uint8_t y);

/*
 *  ��������
 */
void OLED_Clear(void);

/*
 *  ��ʱ����
 */
void OLED_Delay_ms(uint16_t ms);

/*
 *  Һ����ʼ��
 */
void OLED_Init(void);        
     

/******************************************************************************
 �������� ��OLED_Write_Char
 �������� ���ַ���ʾ
 ����˵�� ��x  SmallChars(С����) -- (0 - 20)��21��
               LargeChars(������) -- (0 - 15)��16��
            y  SmallChars(С����) -- (0 - 7) �� 8��
               LargeChars(������) -- (0 - 6) �� 4��  ** yֻ��ȡ"0, 2, 4, 6" 4�� **
            c  ������������ַ�
 �������� ����
 ʾ    �� ��OLED_Write_Char(0, 0, 'A'); -- ��0��0�п�ʼ��� A
*******************************************************************************/
void OLED_Write_Char(uint8_t x, uint8_t y, uint8_t c);


/******************************************************************************
 �������� ��OLED_Write_String
 �������� ���ַ�����ʾ
 ����˵�� ��x  SmallChars(С����) -- (0 - 20)��21��
               LargeChars(������) -- (0 - 15)��16��
            y  SmallChars(С����) -- (0 - 7) �� 8��
               LargeChars(������) -- (0 - 6) �� 4��  ** yֻ��ȡ"0, 2, 4, 6" 4�� **
            *s �ַ���
 �������� ����
 ʾ    �� ��OLED_Write_String(0, 2, (uint8_t*)"Hello"); -- ��2��0�п�ʼ��� Hello
*******************************************************************************/   
void OLED_Write_String(uint8_t x, uint8_t y, uint8_t *s);


/******************************************************************************
 �������� ��OLED_Write_Num3
 �������� ��3λ������ʾ
 ����˵�� ��x   SmallChars(С����) -- (0 - 20)��21��
                LargeChars(������) -- (0 - 15)��16��
            y   SmallChars(С����) -- (0 - 7) �� 8��
                LargeChars(������) -- (0 - 6) �� 4��  ** yֻ��ȡ"0, 2, 4, 6" 4�� **
            num xxx  3λ����
 �������� ����
 ʾ    �� ��OLED_Write_Num3(0, 0, 199);
*******************************************************************************/
void OLED_Write_Num3(uint8_t x, uint8_t y, uint16_t num);


/******************************************************************************
 �������� ��OLED_Write_Char
 �������� ��4λ������ʾ
 ����˵�� ��x   SmallChars(С����) -- (0 - 20)��21��
                LargeChars(������) -- (0 - 15)��16��
            y   SmallChars(С����) -- (0 - 7) �� 8��
                LargeChars(������) -- (0 - 6) �� 4��  ** yֻ��ȡ"0, 2, 4, 6" 4�� **
            num xxxx  4λ����
 �������� ����
 ʾ    �� ��OLED_Write_Num4(0, 0, 1999);
*******************************************************************************/ 
void OLED_Write_Num4(uint8_t x, uint8_t y, uint16_t num);

void OLED_Write_Num5(uint8_t x, uint8_t y, uint16_t num);

void OLED_Write_Num2(uint8_t x, uint8_t y, uint8_t num);

void OLED_Write_Value(uint8_t x, uint8_t y, uint16_t num,uint8_t Type);

//void OLED_Pin_Init(GPIO_Type *GPIOx,uint8_t GPIO_Pin);
void OLED_Pin_Init(PTX_n ptx_n);

void OLED_Write_Chinese(uint8_t x,uint16_t y,uint8_t num,uint8_t* chn );
/*
	��ʾ����ʱ������xֻ��ȡ0��2��4��6
							����yֻ��ȡ0��2��4��6��8��10��12��14

*/

void OLED_Write_Chinese_Big(uint8_t x,uint16_t y,uint8_t num,uint8_t* chn);
/*
	��ʾ�������ʱ������xȡ3��6��9��12
									����yȡ0��3
*/


void OLED_Write_General(uint8_t x,uint16_t y,uint16_t xSize,uint16_t ySize,uint8_t* chn);


void OLED_Write_Chinese_General(uint8_t x,uint16_t y,uint8_t num,uint16_t wordSize,uint8_t* chn);

void OLED_Write_Num3withSymbol(uint8_t x, uint8_t y, int num);

#endif /* _OLED_H_ */

