/*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
-*
-*  MK66-杭电智能车专用
-*  文件功能：      dma脉冲计数
-*  文件类型：      函数文件
-*  内容说明：      适配k66 dmacnt脉冲计数功能
-*                 
-*  编写人：        高源辰
-*  工程授权给：    杭电智能车专用
-*  更新日志：
-*  最后编辑日期：  2018/2/27
-*
*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#include "dmacnt.h"

uint8_t Dummy = 0;

/***********************************************************************************************
 功能：获得 DMA MINOR LOOP LENGTH
 形参：DMAChl: DMA0_CH0 - DMA_CH15
 返回：当前剩余的循环次数
 详解：0
************************************************************************************************/
uint16_t DMA_GetCurrDataCounter(uint8_t DMAChl)
{
	//检测参数
	//assert_param(IS_DMA_CH(DMAChl));
	return (DMA0->TCD[DMAChl].CITER_ELINKNO & DMA_CITER_ELINKNO_CITER_MASK);
}

/***********************************************************************************************
 功能：设置DMA MINOR LOOP LENGTH
 形参：DMAChl: DMA0_CH0 - DMA_CH15
			 DataNumber: 循环长度
 返回：0
 详解：0
************************************************************************************************/
void DMA_SetCurrDataCounter(uint8_t DMAChl,uint16_t DataNumber)
{
	//检测参数
//	assert_param(IS_DMA_CH(DMAChl));
//	assert_param(IS_DMA_MINOR_LOOP(DataNumber));
	
	DMA0->TCD[DMAChl].CITER_ELINKNO = DMA_CITER_ELINKNO_CITER(DataNumber);
}


//以下为接口函数
//DMA 脉冲计数初始化
void DMACNT_Init(DMA_CHn CHn, PTX_n ptxn)	//输入使用的dma通道以及编码器计数引脚
{
	dma_portx2buff_init_new2(CHn, &Dummy, &Dummy, ptxn, DMA_BYTE1, 30000, DADDR_RECOVER);
	//dma_portx2buff_init(CHn, &C_IN_DATA(1), &Dummy, ptxn, DMA_BYTE1, 30000, DADDR_RECOVER);
	DMA_EN(CHn);                                    //使能通道CHn 硬件请求
  DMA_IRQ_EN(CHn);                                //允许DMA通道传输
	//port
	port_init(ptxn, ALT1 | DMA_FALLING | PULLDOWN );
}


//DMA脉冲计数 获得计数值
uint16_t DMACNT_GetValue(uint8_t DMA_Chl)
{
	uint16_t DMA_Value = 0;
	DMA_Value = DMA_GetCurrDataCounter(DMA_Chl);
	DMA_SetCurrDataCounter(DMA_Chl, 30000);
	return (30000-DMA_Value);
}

