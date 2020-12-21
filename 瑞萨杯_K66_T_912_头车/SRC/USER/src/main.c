#include "headfile.h"

#define SpeedControl 1

#define SpeedNormal -129
int wanted_speed = SpeedNormal;
int SpeedUpRight = SpeedNormal-10;
int SpeedUpLeft = SpeedNormal-25;

static uint8_t keydown = 0;

extern uint8_t turn_flag;
uint8_t signal_flag = 0;//0:默认 1：启动 2：停止 3：直行 4：右转 5：左转
uint16_t stop_cnt=0;
uint8_t stopsign_flag = 0;
uint8_t distance_stop = 0;

void sendToSlaveCar(void)
{
    NRF2401Channel = 30;
    NRF2401_SetTXMode();
    while(NRF2401_SendData(NRF2401TXBuffer) != TX_OK)
    {
        DelayMs(2);
    }
//    if(NRF2401_SendData(NRF2401TXBuffer) == TX_OK) {
//      printf("A0 TX OK\r\n");
//    } else {
//      printf("A0 TX ERROR\r\n");
//    }
    NRF2401Channel = 50;
    NRF2401_SetTXMode();
    while(NRF2401_SendData(NRF2401TXBuffer) != TX_OK)
    {
        DelayMs(2);
    }
//    if(NRF2401_SendData(NRF2401TXBuffer) == TX_OK) {
//      printf("A1 TX OK\r\n");
//    } else {
//      printf("A1 TX ERROR\r\n");
//    }
    NRF2401Channel = 80;
    NRF2401_SetTXMode();
    while(NRF2401_SendData(NRF2401TXBuffer) != TX_OK)
    {
        DelayMs(2);
    }
//    if(NRF2401_SendData(NRF2401TXBuffer) == TX_OK) {
//      printf("A2 TX OK\r\n");
//    } else {
//      printf("A2 TX ERROR\r\n");
//    }
    NRF2401Channel = 10;
    NRF2401_SetRXMode();
}

int main(void)
{
  get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
  car_init();//智能车初始化
  
  while(1)
  {
    keydown = Key_Check();
    if(keydown == Keydown_left) { 
      SpeedUpRight--;
      OLED_Print_Num1(0,6,SpeedUpRight);
    }
    if(keydown ==Keydown_right) {
      SpeedUpRight++;
      OLED_Print_Num1(0,6,SpeedUpRight);
    }
    if(keydown == Keydown_ex1) {
      SpeedUpLeft++;
      OLED_Print_Num1(0,5,SpeedUpLeft);
    }
    if(keydown ==Keydown_ex2) {
      SpeedUpLeft--;
      OLED_Print_Num1(0,5,SpeedUpLeft);
    } 
    if(keydown == Keydown_up) {
      enable_irq(PIT2_IRQn);//使能中断，触发PID速度的中断
    }
    if(keydown == Keydown_down) {
      disable_irq(PIT2_IRQn);
      Motor_stop();
    } 

    distance();//超声波测距
    OLED_Print_Num(50,4,Distance);//显示距离，显示范围0-65535
    if(Distance<=40000&&distance_stop==0)
    {
        disable_irq(PIT2_IRQn);
        Motor_stop();
        NRF2401TXBuffer[0] = 36;
        NRF2401TXBuffer[1] = 3;
        sendToSlaveCar();
        NRF2401TXBuffer[1] = 0;
        sendToSlaveCar();//接收端不允许重复数据，发0进行重置
        distance_stop = 1;
    }
    else if(distance_stop==1&&Distance>40000)
    {
        enable_irq(PIT2_IRQn);//使能中断，触发PID速度的中断
        NRF2401TXBuffer[0] = 36;
        NRF2401TXBuffer[1] = 1;
        sendToSlaveCar();
        NRF2401TXBuffer[1] = 0;
        sendToSlaveCar();//接收端不允许重复数据，发0进行重置
        distance_stop = 0;
    }

    if(distance_stop==0)
    {
        dataReadAndAnalyze();//读取数据并将数据进行解析
        servoControl(); //舵机控制
#if SpeedControl
        if(turn_flag==1)//右转
        {
            wanted_speed = SpeedUpRight;
        }
        else if(turn_flag==2)//左转
        {
            wanted_speed = SpeedUpLeft;
        }
        else
        {
            wanted_speed = SpeedNormal;
        }
        pid_speed_L->wantout = pid_speed_R->wantout = wanted_speed;
#endif
        if(signal_flag == signal_go)//头车直接响应启停
        {
          enable_irq(PIT2_IRQn);//使能中断，触发PID速度的中断
          stopsign_flag = 0;//启动也要清零，否则可能因为信号突变无法启动
        } 
        else if(signal_flag == signal_stop)
        {
            stopsign_flag = 1;
    //        disable_irq(PIT2_IRQn);
    //        Motor_stop();
        }
        else if(signal_flag == traffic_stop)
        {
            stopsign_flag = 1;
    //        disable_irq(PIT2_IRQn);
    //        Motor_stop();
        }
        
        if(stopsign_flag)
        {
            disable_irq(PIT2_IRQn);
            stop_cnt++;
            Motor_Control(2000, 2000);//电机控制
            if(stop_cnt>=30)//280
            {
                stop_cnt = 0;
                stopsign_flag = 0;
                Motor_stop();
            } 
        }
        //传输数据给后车
        if(signal_flag==signal_go)//启动
        {
            signal_flag = 0;
            NRF2401TXBuffer[0] = 36;
            NRF2401TXBuffer[1] = 1;
            sendToSlaveCar();
            NRF2401TXBuffer[1] = 0;
            sendToSlaveCar();//接收端不允许重复数据，发0进行重置
        }
        if(signal_flag==signal_stop)//信号停止
        {
          signal_flag = 0;
          NRF2401TXBuffer[0] = 36;
          NRF2401TXBuffer[1] = 3;
          sendToSlaveCar();
          NRF2401TXBuffer[1] = 0;
          sendToSlaveCar();
        }
        if(signal_flag==traffic_stop)//交通信号停止
        {
          signal_flag = 0;
          NRF2401TXBuffer[0] = 36;
          NRF2401TXBuffer[1] = 3;
          sendToSlaveCar();
          NRF2401TXBuffer[1] = 0;
          sendToSlaveCar();
        }
        if(signal_flag==signal_straight)//直行
        {
          signal_flag = 0;
          NRF2401TXBuffer[0] = 36;
          NRF2401TXBuffer[2] = 1;
          sendToSlaveCar();
          NRF2401TXBuffer[2] = 0;
          sendToSlaveCar();
        }
        if(signal_flag==signal_right)//右转
        {
          signal_flag = 0;
          NRF2401TXBuffer[0] = 36;
          NRF2401TXBuffer[2] = 2;
          sendToSlaveCar();
          NRF2401TXBuffer[2] = 0;
          sendToSlaveCar();
        }
        if(signal_flag==signal_left)//左转
        {
          signal_flag = 0;
          NRF2401TXBuffer[0] = 36;
          NRF2401TXBuffer[2] = 3;
          sendToSlaveCar();
          NRF2401TXBuffer[2] = 0;
          sendToSlaveCar();
        }
    }
    DelayMs(2);//延时
  }
}
