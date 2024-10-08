#include "smartcar_init.h"

PID speed_R, speed_L;
PID *pid_speed_R = &speed_R, *pid_speed_L = &speed_L; //PID调速

int car_state = 0;
int cross_state = 0;
int cross_nextstate = 0;
int16_t speed_p_L = 60;
int16_t speed_p_R = 30;
int16_t speed_i = 10;
int16_t speed_d = 0;
int16_t SPEED = -131;

static void motorSet(void)
{
    //电机正反转的端口定义
    motor_LU.Port = ftm1;
    motor_LU.Ch = ftm_ch0;
    motor_LD.Port = ftm1;
    motor_LD.Ch = ftm_ch1;
    motor_RU.Port = ftm0;
    motor_RU.Ch = ftm_ch4;
    motor_RD.Port = ftm0;
    motor_RD.Ch = ftm_ch3;
}

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
    motorSet();//电机正反转的端口定义
    Servo_Init();//舵机初始化
    Motor_Init();//电机初始化
    Encoder_Init();//编码器
    Key_Init();//按键

    PID_init(pid_speed_R, SPEED, speed_p_R/10.0, speed_i/10.0, speed_d/10.0);
    PID_init(pid_speed_L, SPEED, speed_p_L/10.0, speed_i/10.0, speed_d/10.0);//电机PID

    car_interrupt_init();//中断初始化
    OLED_Init();//oled初始化
    OLED_P6x8Str(0, 0, "Initializing...");
    car_communication_init();//NRF2401通信初始化
    camera_init();//摄像头初始化
    HC_SR04_Init();//超声波初始化

    OLED_P6x8Str(0, 2, "Initializ OK!");
}
