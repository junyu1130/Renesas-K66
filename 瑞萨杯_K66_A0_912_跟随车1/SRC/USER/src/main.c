#include "headfile.h"

static uint8_t keydown = 0;
static uint8_t camera_oled_switch = 0;
static uint8_t dis_image[64][128];

#define distanceCNTMax 3
#define StopDistance 40000
uint32_t distance_true;

extern int SpeedDown1;
extern int SpeedDown2;
extern int SpeedUp1;
extern int SpeedUp2;
uint16_t stop_cnt=0;
uint8_t stopsign_flag = 0;

int main(void)
{
  uint8_t num;
  int distanceCNT = 0;
  uint32_t distanceArray[distanceCNTMax] = {0};
  
  get_clk();//上电后必须运行一次这个函数，获取各个频率信息，便于后面各个模块的参数设置
  car_init();//智能车初始化
  
  while(1)
  {
//    for(int i=25;i<=ROW-1;i++)
//    {
//        printf("%d : %d,%d,%d\r\n", i, LeftEdge[i],RightEdge[i],MiddleLine[i]);
//        //printf("%d : %d\r\n", i, RightEdge[i]-LeftEdge[i]);
//    }
//    printf("left:%d right:%d all:%d\r\n", LeftLose,RightLose,AllLose);
    keydown = Key_Check();
    if(car_state == car_go) {
      enable_irq(PIT2_IRQn);//使能中断，触发PID速度的中断
      stopsign_flag = 0;
    } 
    else if(car_state == car_stop) {
      disable_irq(PIT2_IRQn);
      Motor_stop();
    }
    else if(car_state == sign_stop&&Distance<StopDistance) {//小于一定距离信号停车
      stopsign_flag = 1;
//      disable_irq(PIT2_IRQn);
//      Motor_stop();
    }
    if(stopsign_flag)//电机反转刹车控制
      {//小问题：有声音发出，应该是多次触发导致的，不过不清楚加条件后就不顶用了
        disable_irq(PIT2_IRQn);
        stop_cnt++;
        Motor_Control(1100, 1100);//电机控制
        if(stop_cnt>=3)
        {
            stop_cnt = 0;
            stopsign_flag = 0;
            Motor_stop();
        } 
    }
    if(keydown == Keydown_left) { 
//      camera_oled_switch = 0;
      SpeedDown2--;
      OLED_Print_Num1(0,5,SpeedDown2);
    }
    if(keydown ==Keydown_right) {
//      camera_oled_switch = 1;
      SpeedDown2++;
      OLED_Print_Num1(0,5,SpeedDown2);
    }
    if(keydown == Keydown_ex1) {
      SpeedDown1++;
      OLED_Print_Num1(0,6,SpeedDown1);
//      speed_p_L++;
//      pid_speed_L->Kp = speed_p_L;
//      OLED_Print_Num1(0,5,speed_p_L);
    }
    if(keydown ==Keydown_ex2) {
      SpeedDown1--;
      OLED_Print_Num1(0,6,SpeedDown1);
//      speed_p_L--;
//      pid_speed_L->Kp = speed_p_L;
//      OLED_Print_Num1(0,5,speed_p_L);
    } 
    if(keydown == Keydown_up) {
      SpeedUp2++;
      OLED_Print_Num1(0,4,SpeedUp2);
//      speed_p_R++;
//      pid_speed_R->Kp = speed_p_R;
//      OLED_Print_Num1(0,6,speed_p_R);
    }
    if(keydown == Keydown_down) {
      SpeedUp2--;
      OLED_Print_Num1(0,4,SpeedUp2);
//      speed_p_R--;
//      pid_speed_R->Kp = speed_p_R;
//      OLED_Print_Num1(0,6,speed_p_R);
    } 
    
    if(cameraControl_switch) {
      if(mt9v032_finish_flag) {//一场图像采集完成
        mt9v032_finish_flag = 0;
        imageProcess();//进行图像的处理
        findTrack();//寻找赛道的中线
        CrossRecognition(&Cross);//十字识别
        CrossProcess();//十字补线，得出舵机的控制中线
        servoControl(); //舵机控制
        
        distance();//超声波测距
        distanceArray[distanceCNT] = Distance;
        distanceCNT++;
        if(distanceCNT>=distanceCNTMax)
        {
            distanceCNT = 0;
            distance_true = 0;
            for(int i=0;i<distanceCNTMax;i++)
            {
                distance_true += distanceArray[i]/distanceCNTMax;
            }
            if(distance_true!=0)
            {
              distanceControl();//距离控制
//              printf("%d\r\n",distance_true);
              OLED_Print_Num(50,4,distance_true);//显示距离，显示范围0-65535
            }
        }
        
        if(camera_oled_switch) {//发送图像至oled显示
          for(num=0; num<64; num++) {
            memcpy(dis_image[num], &bin_image[num][30], 128);//取中间的128列，0-63行
          }
          dis_mybmp(64, 128, dis_image[0]);
        }
      }
    }
//    DelayMs(2);//延时
  }
}
