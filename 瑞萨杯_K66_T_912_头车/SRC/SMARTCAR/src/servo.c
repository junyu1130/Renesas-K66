#include "servo.h"

//�����ֵ
int servo_Mid = 685;
uint16_t servo_Min = 585;
uint16_t servo_Max = 785;

void Servo_Init(void)//�ŷ�����ʼ��
{
    ftm_pwm_init(servo_Port, servo_Ch, servo_Frequency, servo_Mid);
}

int Server_Control(int change_InitalDuty)//������ƣ�����������ƫ�Ƶ�ռ�ձ�
{
    int servo_InitalDuty = 0;
    servo_InitalDuty = change_InitalDuty;
    //�����ޱ���
    if(servo_InitalDuty > servo_Max)
    {
        servo_InitalDuty = servo_Max;
    }
    else if(servo_InitalDuty < servo_Min)
    {
        servo_InitalDuty = servo_Min;
    }
    //Ӧ��
    ftm_pwm_duty(servo_Port,servo_Ch,servo_InitalDuty);

    return 0;
}


void Servo_test(void)
{
    ftm_pwm_duty(servo_Port,servo_Ch,servo_Min);
    DelayMs(500);
    ftm_pwm_duty(servo_Port,servo_Ch,servo_Max);
    DelayMs(500);
}

/*********define for SteerControl**********/
#define MidLine 86
#define turn_right 1
#define turn_left  2

int16_t  servo_KP=25;//����������ϵ����Ӱ�����Ĵ�Ƿ�Χ
int16_t  servo_KD=35;//�������΢��ϵ��,Ӱ�����Ĵ�Ƿ�Ӧ
int32_t  SteerPwmAdd=0;//���pwm����
int32_t  servoError;//ƫ��ֵ
int32_t  servoLastError;//�ϴε�ƫ��
uint8_t  CenterMeanValue=0;
int32_t SteerPwm=0;//�����pwmֵ
uint8_t turn_flag = 0;

//���ܣ��������
void servoControl(void)
{
    if(CenterMeanValue!=0)
    {
        if(turn_flag==0) {
            servoLastError=servoError;
            servoError=MidLine-CenterMeanValue;// һ��ͼ��ƫ��ֵ
            SteerPwmAdd=(servo_KP/10.0*servoError)+servo_KD/10.0*(servoError-servoLastError);//�����pd������
            if(SteerPwmAdd>=100)
                SteerPwmAdd=100;
            if(SteerPwmAdd<=-100)
                SteerPwmAdd=-100;
            SteerPwm=(uint32_t)(SteerPwmAdd+servo_Mid);
        }
        else if(turn_flag == turn_right) {
            SteerPwm = servo_Min+25;//17
        }
        else if(turn_flag == turn_left) {
            SteerPwm = servo_Max-15;
        }
        if(SteerPwm>=servo_Max)//�޷�
            SteerPwm=servo_Max;
        if(SteerPwm<=servo_Min)
            SteerPwm=servo_Min;

        ftm_pwm_duty(servo_Port, servo_Ch, SteerPwm);//���pwm����
    }
}
