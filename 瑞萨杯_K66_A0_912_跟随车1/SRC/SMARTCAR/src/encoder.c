#include "encoder.h"

int Speed_Left = 0,Speed_Right = 0; //编码器现在值

//编码器初始化
void Encoder_Init()
{
    //初始化dmacnt
    //左编码器脉冲口
    DMACNT_Init(DMA_CH1,Left_Encoder_Pulse_PORT);
    //左编码器方向口
    gpio_init(Left_Encoder_Direction_PORT,GPI,2);
    //右编码器脉冲口
    DMACNT_Init(DMA_CH2,Right_Encoder_Pulse_PORT);
    //右编码器方向口
    gpio_init(Right_Encoder_Direction_PORT,GPI,2);
}

extern PID *pid_speed_R, *pid_speed_L; //PID调速
//保护等开关
uint8_t stop_flag = 0;
//获取左右转速
void SpeedGet(void)
{
    //编码器保护项
    static uint8_t left_cnt = 0,right_cnt = 0;

    Speed_Left = DMACNT_GetValue(DMA_CH1);
    Speed_Right = DMACNT_GetValue(DMA_CH2);
    //对获取的编码器值分正负    //左正转1  右正转0
    if(Left_Encoder_Direction == 1)
    {
        Speed_Left = -Speed_Left;
    }
    if(Right_Encoder_Direction == 0)
    {
        Speed_Right = -Speed_Right;
    }

    pid_speed_L->ActualValue = Speed_Left;
    pid_speed_R->ActualValue = Speed_Right;


    /********编码器保护*********/
    if(encoderProtect_switch)
    {
        if(Speed_Left > -2 && Speed_Left < 2)
        {
            //左轮停止
            left_cnt++;
        }
        else
        {
            left_cnt = 0;
        }

        if(Speed_Right > -2 && Speed_Right < 2)
        {
            //右轮停止
            right_cnt++;
        }
        else
        {
            right_cnt = 0;
        }

        if((left_cnt > 10 || right_cnt > 10) && stop_flag == 0)
        {
            //保护
            Motor_stop();
            stop_flag = 1;
        }
    }

}




