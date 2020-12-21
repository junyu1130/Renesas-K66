/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2016,��ɿƼ�
 * All rights reserved.
 * ��������QQȺ��179029047
 *
 * �����������ݰ�Ȩ������ɿƼ����У�δ��������������ҵ��;��
 * ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
 *
 * @file       		isr.c
 * @brief      		�жϺ�����
 * @company	   		�ɶ���ɿƼ����޹�˾
 * @author     		Go For It(1325536866)
 * @version    		v1.0
 * @Software 		IAR 7.7 or MDK 5.17
 * @Target core		MK60DN512VLL10
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2016-02-25
 ********************************************************************************************************************/



#include "isr.h"

int PwmL_Forward = 0, PwmR_Forward = 0; //���ռ�ձ�
int scount = 0;
//int cnt = 0;

void PIT0_IRQHandler(void)
{
    PIT_FlAG_CLR(pit0);
    
}

void PIT1_IRQHandler(void)
{
    PIT_FlAG_CLR(pit1);
}

void PIT2_IRQHandler(void)
{
    PIT_FlAG_CLR(pit2);
    
    //���ٶ�
    scount++;
    if(scount>=5)
    {
        scount=0;
        SpeedGet();
        if(speedControl_switch)
        {
            //PID���*******************
            PwmL_Forward = PID_Calc(pid_speed_L, 300) + PwmL_Forward;         //���ٶȵ�
            PwmR_Forward = PID_Calc(pid_speed_R, 300) + PwmR_Forward;
            //����ģʽ
            if(PwmL_Forward > 2000)
            {
                PwmL_Forward = 2000;
            }
            if(PwmR_Forward > 2000)
            {
                PwmR_Forward = 2000;
            }
            if(PwmL_Forward < -2000)
            {
                PwmL_Forward = -2000;
            }
            if(PwmR_Forward < -2000)
            {
                PwmR_Forward = -2000;
            }
            if(motorControl_switch) {
                Motor_Control(PwmL_Forward, PwmR_Forward);//�������
            }
//            printf("motor pwmL: %d ", PwmL_Forward);
//            printf("pwmR: %d ", PwmR_Forward);
//            printf("speedL: %d ", Speed_Left);
//            printf("speedR: %d\r\n", Speed_Right);
//            printf("%d\r\n", Speed_Right);
//            cnt++;
//            if(cnt>=2)
//            {
//                cnt = 0;
//                NRF2401TXBuffer[0] = 36 & 0xff;
//                NRF2401TXBuffer[1] = (-Speed_Left) & 0xff;
//                NRF2401TXBuffer[2] = (-Speed_Right) & 0xff;
//                NRF2401_SetTXMode();
//                NRF2401_SendData(NRF2401TXBuffer);
//                NRF2401_SetRXMode();
//            }
        }
    }
}


//���߽����ж�	PTC9
void PORTC_IRQHandler(void)
{
  int i;
  SIM->SCGC5|=SIM_SCGC5_PORTC_MASK;
  PORTC->ISFR |= (1<<9);
  if(NRF2401_RecData(NRF2401RXBuffer) == 0)//������յ�����
  {
    if(NRF2401RXBuffer[0] == 36)
    {
      for(i=0; i<15; i++)
      {
        WirelessReceiveStorage[i] = NRF2401RXBuffer[i+1];
      }
              
      if(WirelessReceiveStorage[0]==1) {//����ָ��
        car_state = car_go;
        WirelessReceiveStorage[0] = 0;
      } else if(WirelessReceiveStorage[0]==2) {//����ͣ���ź�
        car_state = car_stop;
        WirelessReceiveStorage[0] = 0;
      } else if(WirelessReceiveStorage[0]==3) {//�ź�ͣ���ź�
        car_state = sign_stop;
        WirelessReceiveStorage[0] = 0;
      }
      
      if(WirelessReceiveStorage[1]==1) {//ֱ��
        cross_nextstate = cross_straight;
        WirelessReceiveStorage[1] = 0;
      } else if(WirelessReceiveStorage[1]==2) {//��ת
        cross_nextstate = cross_right;
        WirelessReceiveStorage[1] = 0;
      } else if(WirelessReceiveStorage[1]==3) {//��ת
        cross_nextstate = cross_left;
        WirelessReceiveStorage[1] = 0;
      }
//      NRF2401TXBuffer[0] = 36 & 0xff;
//      NRF2401_SetTXMode();
//      NRF2401_SendData(NRF2401TXBuffer);
//      NRF2401_SetRXMode();
      
//      for(i = 0; i < 10; i++) {
//          NRF2401TXBuffer[i] = 0;
//      }
      
    }
  }
}


/*******************�����*******************/
//���ж� E12
void PORTE_IRQHandler(void)
{
    //����жϱ�־��һ�ַ���ֱ�Ӳ����Ĵ�����ÿһλ��Ӧһ������
    PORTE->ISFR = 0xffffffff;
    VSYNC();
}

void DMA0_IRQHandler(void)
{
    DMA_IRQ_CLEAN(DMA_CH0);
    row_finished();
}

void UART0_RX_TX_IRQHandler(void)
{
    if(UART0->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        //�û���Ҫ�����������
    }
    if(UART0->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������

    }
}
//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART3�ж�ִ�к���
//  @return     void   
//  @since      v1.0
//  Sample usage:               ��UART3�����жϹ����ҷ����жϵ�ʱ����Զ�ִ�иú���
//-------------------------------------------------------------------------------------------------------------------
void UART3_RX_TX_IRQHandler(void)
{
    if(UART3->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        //�û���Ҫ�����������
        mt9v032_cof_uart_interrupt();
    }
    if(UART3->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������

    }
}

void UART4_RX_TX_IRQHandler(void)
{
    if(UART4->S1 & UART_S1_RDRF_MASK)                                     //�������ݼĴ�����
    {
        
    }
    if(UART4->S1 & UART_S1_TDRE_MASK )                                    //�������ݼĴ�����
    {
        //�û���Ҫ����������

    }
}


/*

�жϺ������ƣ��������ö�Ӧ���ܵ��жϺ���
Sample usage:��ǰ������DMA0�жϣ�Ȼ��͵�����ȥ���ĸ���DMA0���жϺ������ƣ��ҵ���дһ�������Ƶĺ�������
void DMA0_IRQHandler(void)
{
    ;
}
�ǵý����жϺ������־λ

DMA0_IRQHandler  
DMA1_IRQHandler  
DMA2_IRQHandler  
DMA3_IRQHandler  
DMA4_IRQHandler  
DMA5_IRQHandler  
DMA6_IRQHandler  
DMA7_IRQHandler  
DMA8_IRQHandler  
DMA9_IRQHandler  
DMA10_IRQHandler 
DMA11_IRQHandler 
DMA12_IRQHandler 
DMA13_IRQHandler 
DMA14_IRQHandler 
DMA15_IRQHandler 
DMA_Error_IRQHandler      
MCM_IRQHandler            
FTFL_IRQHandler           
Read_Collision_IRQHandler 
LVD_LVW_IRQHandler        
LLW_IRQHandler            
Watchdog_IRQHandler       
RNG_IRQHandler            
I2C0_IRQHandler           
I2C1_IRQHandler           
SPI0_IRQHandler           
SPI1_IRQHandler           
SPI2_IRQHandler           
CAN0_ORed_Message_buffer_IRQHandler    
CAN0_Bus_Off_IRQHandler                
CAN0_Error_IRQHandler                  
CAN0_Tx_Warning_IRQHandler             
CAN0_Rx_Warning_IRQHandler             
CAN0_Wake_Up_IRQHandler                
I2S0_Tx_IRQHandler                     
I2S0_Rx_IRQHandler                     
CAN1_ORed_Message_buffer_IRQHandler    
CAN1_Bus_Off_IRQHandler                
CAN1_Error_IRQHandler                  
CAN1_Tx_Warning_IRQHandler             
CAN1_Rx_Warning_IRQHandler             
CAN1_Wake_Up_IRQHandler                
Reserved59_IRQHandler                  
UART0_LON_IRQHandler                   
UART0_RX_TX_IRQHandler                 
UART0_ERR_IRQHandler                   
UART1_RX_TX_IRQHandler                 
UART1_ERR_IRQHandler  
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler  
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler  
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler  
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler  
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler 
RTC_IRQHandler 
RTC_Seconds_IRQHandler  
PIT0_IRQHandler  
PIT1_IRQHandler  
PIT2_IRQHandler  
PIT3_IRQHandler  
PDB0_IRQHandler  
USB0_IRQHandler  
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler  
ENET_Receive_IRQHandler
ENET_Error_IRQHandler  
Reserved95_IRQHandler  
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler 
LPTimer_IRQHandler 
Reserved102_IRQHandler 
PORTA_IRQHandler 
PORTB_IRQHandler 
PORTC_IRQHandler 
PORTD_IRQHandler 
PORTE_IRQHandler 
Reserved108_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler 
*/
