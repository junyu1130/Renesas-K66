#include "smartcar_init.h"

static void car_interrupt_init(void)
{
  pit_init_ms(pit2, 10);//���ټӶ���ж�
  set_irq_priority(PIT2_IRQn, 1);
  EnableInterrupts;//ʹ�����ж�
}
                                                                       
static void car_communication_init(void)
{
  if(NRF2401_Init() == NRF_OK) {
    OLED_P6x8Str(0, 1, "NRF2401 OK");
  } else {
    OLED_P6x8Str(0, 1, "NRF2401 FAIL");
  }
  NRF2401_SetRXMode();//����ģʽ
}

void car_init(void)
{
  buzzer_Init();//��������ʼ��
  buzzer_Ring();
  Key_Init();//����
  
  car_interrupt_init();//�жϳ�ʼ��
  //OLED_Init();//oled��ʼ��
	//OLED_P6x8Str(0, 0, "Initializing...");
  car_communication_init();//NRF2401ͨ�ų�ʼ��

  OLED_P6x8Str(0, 2, "Initializ OK!");
}
