#include "Ultrasonic.h"

//uint16_t Uls_Distance=0;
static uint8_t Uls_dat[20];
static uint8_t Uls_Num;
//uint8_t Uls_receive_flag = 0;
//int last_distance[2];
//uint16_t uwb_Distance=0;
////�㳵��ʱ
//uint32_t seecar_timer = 0;//��ֵΪ0ֱ�ӿ�ʼ�㳵���
//uint32_t seecar_timer_set = 10;//�洢����ʱ��

void Ultrasonic_Init(void)//������ģ���ʼ��
{
		//��ʼ��uart0��ʹ���ж�
		uart_init(UART_Ultrasonic_Rec,115200);//���ڳ�ʼ��
		NVIC_DisableIRQ(UART0_RX_TX_IRQn);
		set_irq_priority(UART0_RX_TX_IRQn,0);//uart0�ж�
		uart_rx_irq_en(UART_Ultrasonic_Rec);//ʹ���ж�	
}

void Ultrasonic_Receive(void)//�������жϽ���ģ��
{
	//��������������
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
      NRF2401TXBuffer[4]=Uls_dat[9];   //��վ01,��λ��ǰ
      
//      NRF2401TXBuffer[5]=Uls_dat[10];
//      NRF2401TXBuffer[6]=Uls_dat[11];  //��վ02

      NRF2401TXBuffer[7]=Uls_dat[12];
      NRF2401TXBuffer[8]=Uls_dat[13];  //��վ12
      
      OLED_Print_Num(0, 5, Uls_dat[0]);
      OLED_Print_Num(0, 6, Uls_dat[1]);
		}
    if(Uls_dat[2]==2)
		{
//      NRF2401TXBuffer[7]=Uls_dat[8];    //��ǩ��1
//      NRF2401TXBuffer[8]=Uls_dat[9];
      
      NRF2401TXBuffer[7]=Uls_dat[10];   //��ǩ��2
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

//void Ultrasonic_seecar(void)//�������㳵���
//{
//	
//}


////�ᳵ���ж�����
////�ᳵ����ʱ�ж�ʱ���趨
//uint32_t check_dashedfield_timer_set[2] = {7,7};//��ʱ���еڶ��� �����λ㳵�ж�
//uint32_t check_dashedfield_timer = 10000;//�㳵���о���ʱ

