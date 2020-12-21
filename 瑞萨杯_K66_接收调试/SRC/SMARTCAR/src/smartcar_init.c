#include "smartcar_init.h"

static void car_interrupt_init(void)
{
  pit_init_ms(pit2, 10);//调速加舵机中断
  set_irq_priority(PIT2_IRQn, 1);
  EnableInterrupts;//使能总中断
}
                                                                       
static void car_communication_init(void)
{
  if(NRF2401_Init() == NRF_OK) {
    OLED_P6x8Str(0, 1, "NRF2401 OK");
  } else {
    OLED_P6x8Str(0, 1, "NRF2401 FAIL");
  }
  NRF2401_SetRXMode();//接收模式
}

void car_init(void)
{
  buzzer_Init();//蜂鸣器初始化
  buzzer_Ring();
  Key_Init();//按键
  
  car_interrupt_init();//中断初始化
  //OLED_Init();//oled初始化
	//OLED_P6x8Str(0, 0, "Initializing...");
  car_communication_init();//NRF2401通信初始化

  OLED_P6x8Str(0, 2, "Initializ OK!");
}
