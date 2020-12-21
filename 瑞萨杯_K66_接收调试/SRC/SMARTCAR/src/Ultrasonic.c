#include "Ultrasonic.h"

//uint16_t Uls_Distance=0;
static uint8_t Uls_dat[20];
static uint8_t Uls_Num;
//uint8_t Uls_receive_flag = 0;
//int last_distance[2];
//uint16_t uwb_Distance=0;
////汇车计时
//uint32_t seecar_timer = 0;//初值为0直接开始汇车检测
//uint32_t seecar_timer_set = 10;//存储秒数时间

void Ultrasonic_Init(void)//超声波模块初始化
{
		//初始化uart0并使能中断
		uart_init(UART_Ultrasonic_Rec,115200);//串口初始化
		NVIC_DisableIRQ(UART0_RX_TX_IRQn);
		set_irq_priority(UART0_RX_TX_IRQn,0);//uart0中断
		uart_rx_irq_en(UART_Ultrasonic_Rec);//使能中断	
}

void Ultrasonic_Receive(void)//超声波中断接受模块
{
	//超声波接受数据
	uart_getchar(UART_Ultrasonic_Rec,&Uls_dat[Uls_Num]);
	if(Uls_Num==0)
	{
		if(Uls_dat[0]==0xD6)
      Uls_Num++;
		else
			Uls_Num=0;
	}
	else if(Uls_Num==1)
	{
		if(Uls_dat[1]==0x6D)
      Uls_Num++;
		else
			Uls_Num=0;
	}	
	else if (Uls_Num<13) 
  {
		Uls_Num++;
  }
	if(Uls_Num==13)
	{
    Uls_Num=0;	
		if(Uls_dat[2]==1)
		{
      NRF2401TXBuffer[3]=Uls_dat[8];
      NRF2401TXBuffer[4]=Uls_dat[9];   //基站01,低位在前
      
//      NRF2401TXBuffer[5]=Uls_dat[10];
//      NRF2401TXBuffer[6]=Uls_dat[11];  //基站02

      NRF2401TXBuffer[7]=Uls_dat[12];
      NRF2401TXBuffer[8]=Uls_dat[13];  //基站12
      
      OLED_Print_Num(0, 5, Uls_dat[0]);
      OLED_Print_Num(0, 6, Uls_dat[1]);
		}
    if(Uls_dat[2]==2)
		{
//      NRF2401TXBuffer[7]=Uls_dat[8];    //标签到1
//      NRF2401TXBuffer[8]=Uls_dat[9];
      
      NRF2401TXBuffer[7]=Uls_dat[10];   //标签到2
      NRF2401TXBuffer[8]=Uls_dat[11];
		}
	}
}


//uint8_t in_cnt = 0,out_cnt = 0,out_cnt_flag = 0;
//uint16_t Uls_Distance_last = 0;
//uint8_t startline_cnt = 0,startline_cnt_flag = 0;
//uint8_t startline_give = 0;
//uint8_t seecar_cnt = 0;
//uint8_t seecar_process = 0;

//void Ultrasonic_seecar(void)//超声波汇车检测
//{
//	
//}


////会车区判定限制
////会车区延时判断时间设定
//uint32_t check_dashedfield_timer_set[2] = {7,7};//延时进行第二次 第三次汇车判断
//uint32_t check_dashedfield_timer = 10000;//汇车区判决计时

