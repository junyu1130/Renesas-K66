/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
-*
-*  MK66-�������ܳ�ר��
-*  �ļ����ܣ�      dma�������
-*  �ļ����ͣ�      �����ļ�
-*  ����˵����      ����k66 dmacnt�����������
-*                 
-*  ��д�ˣ�        ��Դ��
-*  ������Ȩ����    �������ܳ�ר��
-*  ������־��
-*  ���༭���ڣ�  2018/2/27
-*
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#include "dmacnt.h"

uint8_t Dummy = 0;

/***********************************************************************************************
 ���ܣ���� DMA MINOR LOOP LENGTH
 �βΣ�DMAChl: DMA0_CH0 - DMA_CH15
 ���أ���ǰʣ���ѭ������
 ��⣺0
************************************************************************************************/
uint16_t DMA_GetCurrDataCounter(uint8_t DMAChl)
{
	//������
	//assert_param(IS_DMA_CH(DMAChl));
	return (DMA0->TCD[DMAChl].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK);
}

/***********************************************************************************************
 ���ܣ�����DMA MINOR LOOP LENGTH
 �βΣ�DMAChl: DMA0_CH0 - DMA_CH15
			 DataNumber: ѭ������
 ���أ�0
 ��⣺0
************************************************************************************************/
void DMA_SetCurrDataCounter(uint8_t DMAChl,uint16_t DataNumber)
{
	//������
//	assert_param(IS_DMA_CH(DMAChl));
//	assert_param(IS_DMA_MINOR_LOOP(DataNumber));
	
	DMA0->TCD[DMAChl].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(DataNumber);
}


//����Ϊ�ӿں���
//DMA ���������ʼ��
void DMACNT_Init(DMA_CHn CHn, PTX_n ptxn)	//����ʹ�õ�dmaͨ���Լ���������������
{
	dma_portx2buff_init_new2(CHn, &Dummy, &Dummy, ptxn, DMA_BYTE1, 30000, DADDR_RECOVER);
	//dma_portx2buff_init(CHn, &C_IN_DATA(1), &Dummy, ptxn, DMA_BYTE1, 30000, DADDR_RECOVER);
	DMA_EN(CHn);                                    //ʹ��ͨ��CHn Ӳ������
  DMA_IRQ_EN(CHn);                                //����DMAͨ������
	//port
	port_init(ptxn, ALT1 | DMA_FALLING | PULLDOWN );
}


//DMA������� ��ü���ֵ
uint16_t DMACNT_GetValue(uint8_t DMA_Chl)
{
	uint16_t DMA_Value = 0;
	DMA_Value = DMA_GetCurrDataCounter(DMA_Chl);
	DMA_SetCurrDataCounter(DMA_Chl, 30000);
	return (30000-DMA_Value);
}

