/**
  ******************************************************************************
  * @file    nrf2401.h
  * @author  YANDLD
  * @version V2.4
  * @date    2013.5.23
  * @brief   ����ԭ�Ӻ˺��İ� BSP���� NRF2401����
  ******************************************************************************
  */
#ifndef __NRF2401_H__
#define __NRF2401_H__

#include "headfile.h"

//���Ŷ���
//IO ��
#define NRF2401_CE_PORT    C8
#define NRF2401_CSN_PORT   C4
#define NRF2401_IRQ_PORT   C9
#define NRF2401_SCK_PORT   C5
#define NRF2401_SDI_PORT   C7
#define NRF2401_SDO_PORT   C6

//����
#define NRF2401_CE_PIN    (8)
#define NRF2401_CSN_PIN   (4)
#define NRF2401_IRQ_PIN   (9)
#define NRF2401_SCK_PIN   (5)
#define NRF2401_SDO_PIN   (6)
#define NRF2401_SDI_PIN   (7)
//λ��
#define NRF2401_CE    PCout(NRF2401_CE_PIN) 
#define NRF2401_CSN   PCout(NRF2401_CSN_PIN)
#define NRF2401_IRQ   PCin(NRF2401_IRQ_PIN)
#define NRF2401_SCK   PCout(NRF2401_SCK_PIN)
#define NRF2401_SDO   PCout(NRF2401_SDO_PIN)
#define NRF2401_SDI   PCin(NRF2401_SDI_PIN)


#define NRF2401_PORT_DATA     (SPI0_SCK_PA15_SOUT_PA16_SIN_PA17)
#define NRF2401_PORT_CS       (SPI0_PCS0_PA14)

//���س���
#define NRF_OK      (0x00)
#define NRF_ERR     (0x01)
#define MAX_TX  		(0x10)  //�ﵽ����ʹ����ж�
#define TX_OK   		(0x20)  //TX��������ж�
#define RX_OK   		(0x40)  //���յ������ж�


//********************************************************************************************************************// 
// SPI(nRF24L01) ָ��
#define READ_REG    0x00   // �����üĴ���
#define WRITE_REG   0x20   // д���üĴ���
#define RD_RX_PLOAD 0x61   // ��ȡRX FIFO�е�����
#define WR_TX_PLOAD 0xA0   // ��TX FIFO��д������
#define FLUSH_TX    0xE1   // ���TX FIFO�е����� Ӧ���ڷ���ģʽ��
#define FLUSH_RX    0xE2   // ���RX FIFO�е����� Ӧ���ڽ���ģʽ��
#define REUSE_TX_PL 0xE3   // ����ʹ����һ����Ч����
#define NOP         0xFF   // ����
//********************************************************************************************************************// 
// SPI(nRF24L01) �Ĵ���(��ַ)
#define CONFIG      0x00  //���÷���״̬��CRCУ��ģʽ�Լ����շ�״̬��Ӧ��ʽ
#define EN_AA       0x01  //�Զ�Ӧ��������
#define EN_RXADDR   0x02  //�����ŵ�����
#define SETUP_AW    0x03  //�շ���ַ�������
#define SETUP_RETR  0x04  //�Զ��ط�����������
#define RF_CH       0x05  //����Ƶ���趨
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17

#define STATUS_MAX_RT 0x10
#define STATUS_TX_DS  0x20
#define STATUS_RX_DR  0x40


//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32  	//32�ֽڵ��û����ݿ��


extern uint8_t NRF2401TXBuffer[TX_PLOAD_WIDTH]; //���߷�������
extern uint8_t NRF2401RXBuffer[RX_PLOAD_WIDTH]; //���߽�������
extern uint8_t WirelessReceiveStorage[32];
extern uint8_t NRF2401Channel;

//��������ʵ�ֵĽӿں���
uint8_t NRF2401_Init(void);
void NRF2401_SetRXMode(void);
void NRF2401_SetTXMode(void);
uint8_t NRF2401_SendData(uint8_t *txbuf);
uint8_t NRF2401_RecData(uint8_t *rxbuf);
uint8_t NRF2401_SendData_Duplex(uint8_t *txbuf);//2401˫������
#endif



