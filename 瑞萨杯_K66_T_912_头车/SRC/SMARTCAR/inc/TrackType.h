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
 unsigned char InflectionPointRow;//�յ��������
 unsigned char InflectionPointCol;//�յ��������
 unsigned char InflectionPointFlag;//�յ�ı�־λ
 unsigned char ScanInflectionPointStart;//ɨ��յ����ʼ��
 unsigned char ScanInflectionPointEnd;//ɨ��յ������ 
 unsigned char InflectionPointNumber;//�յ�ĸ���
 
}InflectionPoint;//�����յ�ṹ��

typedef struct 
{
  unsigned char CrossLR;//ʮ��
  unsigned char CrossL;//���·
  unsigned char CrossR;//�Ҳ�·
}CrossType;//����ʮ�����͵Ľṹ��


extern CrossType Cross;//�ⲿ����ʮ�ֽṹ�����
extern InflectionPoint  InflectionPointL;//�ⲿ������߹յ�ṹ�����
extern InflectionPoint  InflectionPointR;//�ⲿ�����ұ߹յ�ṹ�����
void CrossProcess(void);
void CrossRecognition(CrossType *prt);
void FindInflectionPointR(InflectionPoint *prt ,unsigned char Start ,unsigned char End );
void FindInflectionPointL(InflectionPoint *prt ,unsigned char Start ,unsigned char End );

#endif
