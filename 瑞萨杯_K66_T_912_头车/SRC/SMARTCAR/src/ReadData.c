#include "ReadData.h"

extern uint8_t CenterMeanValue;//��������
extern uint8_t turn_flag;//ת������ź�
extern uint8_t signal_flag;//��־�ź�

uint8_t sign_buf = 0;//����ǰһ������

void readData_init(void)
{
    gpio_init(parallelData0_Port,GPI,0);
    gpio_init(parallelData1_Port,GPI,0);
    gpio_init(parallelData2_Port,GPI,0);
    gpio_init(parallelData3_Port,GPI,0);
    gpio_init(parallelData4_Port,GPI,0);
    gpio_init(parallelData5_Port,GPI,0);
    gpio_init(parallelData6_Port,GPI,0);
    gpio_init(parallelData7_Port,GPI,0);
    gpio_init(parallelData8_Port,GPI,0);
    gpio_init(parallelData9_Port,GPI,0);
    gpio_init(parallelData10_Port,GPI,0);
    gpio_init(parallelData11_Port,GPI,0);
    gpio_init(parallelData12_Port,GPI,0);
}

//���ڶ�ȡ����
static uint16_t readData(void)
{
    uint16_t data = 0;
    if(parallelData0)  data = data | 0x0001;
    if(parallelData1)  data = data | 0x0002;
    if(parallelData2)  data = data | 0x0004;
    if(parallelData3)  data = data | 0x0008;
    if(parallelData4)  data = data | 0x0010;
    if(parallelData5)  data = data | 0x0020;
    if(parallelData6)  data = data | 0x0040;
    if(parallelData7)  data = data | 0x0080;
    if(parallelData8)  data = data | 0x0100;
    if(parallelData9)  data = data | 0x0200;
    if(parallelData10) data = data | 0x0400;
    if(parallelData11) data = data | 0x0800;
    if(parallelData12) data = data | 0x1000;
  
    return data;
}

//��ȡ���ݲ������ݽ��н���
//ǰ��λ��ʶ�𵽵ı�־�ź� 2λ��ת������ź� 8λ������ֵ 
void dataReadAndAnalyze(void)
{
    uint16_t data = 0;
    uint8_t sign_receive = 0;
    data = readData();
    CenterMeanValue = data & 0xff;
    turn_flag = (data>>8) & 0x03;
    sign_receive  = (data>>10) & 0x07;
    if(sign_receive!=sign_buf)
    {
        signal_flag = sign_receive;
        sign_buf = sign_receive;
    }
//    printf("%d , %d , %d\r\n",CenterMeanValue,turn_flag,signal_flag);
}

