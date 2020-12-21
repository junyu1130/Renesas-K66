#include "headfile.h"

static uint8_t keydown = 0;

void sendToSlaveCar(void)
{
    NRF2401Channel = 10;
    NRF2401_SetTXMode();
    if(NRF2401_SendData(NRF2401TXBuffer) == TX_OK) {
      printf("T TX OK\r\n");
    } else {
      printf("T TX ERROR\r\n");
    }
//    NRF2401Channel = 30;
//    NRF2401_SetTXMode();
//    if(NRF2401_SendData(NRF2401TXBuffer) == TX_OK) {
//      printf("A0 TX OK\r\n");
//    } else {
//      printf("A0 TX ERROR\r\n");
//    }
//    NRF2401Channel = 50;
//    NRF2401_SetTXMode();
//    if(NRF2401_SendData(NRF2401TXBuffer) == TX_OK) {
//      printf("A1 TX OK\r\n");
//    } else {
//      printf("A1 TX ERROR\r\n");
//    }
//    NRF2401Channel = 70;
//    NRF2401_SetTXMode();
//    if(NRF2401_SendData(NRF2401TXBuffer) == TX_OK) {
//      printf("A2 TX OK\r\n");
//    } else {
//      printf("A2 TX ERROR\r\n");
//    }
    NRF2401Channel = 50;
    NRF2401_SetRXMode();
}

int main(void)
{
  get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
  car_init();//智能车初始化
  
  while(1)
  {
    keydown = Key_Check();
    
    if(keydown == Keydown_up) {
      NRF2401TXBuffer[0] = 36;
      NRF2401TXBuffer[1] = 1;
      sendToSlaveCar();
      NRF2401TXBuffer[1] = 0;
      sendToSlaveCar();//接收端不允许重复数据，发0进行重置
    }
    if(keydown == Keydown_down) {
      NRF2401TXBuffer[0] = 36;
      NRF2401TXBuffer[1] = 2;
      sendToSlaveCar();
      NRF2401TXBuffer[1] = 0;
      sendToSlaveCar();
    }
    
    if(keydown == Keydown_right) {//右转
      NRF2401TXBuffer[0] = 36;
      NRF2401TXBuffer[2] = 2;
      sendToSlaveCar();
      NRF2401TXBuffer[2] = 0;
      sendToSlaveCar();
    }
    if(keydown == Keydown_left) {//左转
      NRF2401TXBuffer[0] = 36;
      NRF2401TXBuffer[2] = 3;
      sendToSlaveCar();
      NRF2401TXBuffer[2] = 0;
      sendToSlaveCar();
    }
    if(keydown == Keydown_ex1) {//直行
      NRF2401TXBuffer[0] = 36;
      NRF2401TXBuffer[2] = 1;
      sendToSlaveCar();
      NRF2401TXBuffer[2] = 0;
      sendToSlaveCar();
    }
  }
}
