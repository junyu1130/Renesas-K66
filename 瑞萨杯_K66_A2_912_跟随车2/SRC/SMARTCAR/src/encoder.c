#include "encoder.h"

int Speed_Left = 0,Speed_Right = 0; //����������ֵ

//��������ʼ��
void Encoder_Init()
{
    //��ʼ��dmacnt
    //������������
    DMACNT_Init(DMA_CH1,Left_Encoder_Pulse_PORT);
    //������������
    gpio_init(Left_Encoder_Direction_PORT,GPI,2);
    //�ұ����������
    DMACNT_Init(DMA_CH2,Right_Encoder_Pulse_PORT);
    //�ұ����������
    gpio_init(Right_Encoder_Direction_PORT,GPI,2);
}

extern PID *pid_speed_R, *pid_speed_L; //PID����
//�����ȿ���
uint8_t stop_flag = 0;
//��ȡ����ת��
void SpeedGet(void)
{
    //������������
    static uint8_t left_cnt = 0,right_cnt = 0;

    Speed_Left = DMACNT_GetValue(DMA_CH1);
    Speed_Right = DMACNT_GetValue(DMA_CH2);
    //�Ի�ȡ�ı�����ֵ������    //����ת1  ����ת0
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


    /********����������*********/
    if(encoderProtect_switch)
    {
        if(Speed_Left > -2 && Speed_Left < 2)
        {
            //����ֹͣ
            left_cnt++;
        }
        else
        {
            left_cnt = 0;
        }

        if(Speed_Right > -2 && Speed_Right < 2)
        {
            //����ֹͣ
            right_cnt++;
        }
        else
        {
            right_cnt = 0;
        }

        if((left_cnt > 10 || right_cnt > 10) && stop_flag == 0)
        {
            //����
            Motor_stop();
            stop_flag = 1;
        }
    }

}




