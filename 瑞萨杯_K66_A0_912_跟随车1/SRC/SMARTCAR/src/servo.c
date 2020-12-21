#include "servo.h"

//�����ֵ
int servo_Mid = 715;
uint16_t servo_Min = 615;
uint16_t servo_Max = 815;

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
uint16_t  servo_KP=25;//����������ϵ����Ӱ�����Ĵ�Ƿ�Χ
uint16_t  servo_KD=0;//10//7.5//�������΢��ϵ��,Ӱ�����Ĵ�Ƿ�Ӧ
float  SteerPwmAdd=0.0;//���pwm����
float  servoError;//ƫ��ֵ
float  servoLastError;//�ϴε�ƫ��
float  WeightSum=0;
float  CenterMeanValue=0;
float  CenterSum=0;
uint32_t SteerPwm=0,LastSteerSwm=0;//�����pwmֵ���ϴζ����pwmֵ
uint16_t controlCnt = 0;

float Weight[64] = {
    0,0,0,0,0,0,0,0,0,0,              //0-9�У������ò���

    0,0,0,0,0,0,0,0,0,0,              //0-19�У������ò���

    0,0,0,0,0,0,0,0,0,0,              //20-29��

    0,0,0,0,0,0,0,0,0,0,              //30-39��

    1,1,1,1,1,2,2,2,2,2,              //40-49��

    2,2,2,2,1,1,1,1,1,0,              //50-59��

    0,0,0,0,                          //60-63��
};

static void CalculateError(void)
{
    int i;
    CenterSum=0;
    CenterMeanValue=0;
    WeightSum=0;
    for(i=StartRow; i>25; i--)
    {
        if(ABS(MiddleLine[i]-MiddleLine[i+1])>3 && i!=StartRow)//ƽ������
        {
            MiddleLine[i]=MiddleLine[i+1];
        }
        CenterSum+=MiddleLine[i]*Weight[i];
        WeightSum+=Weight[i];
    }
    if(WeightSum!=0)
    {
        CenterMeanValue=CenterSum/WeightSum;//�����Ȩƽ�������ߵ�ֵ
    }
    servoLastError=servoError;
    servoError=MidLine-CenterMeanValue;// һ��ͼ��ƫ��ֵ
}
//���ܣ��������
void servoControl(void)
{
    if(turn_flag==0) {
        CalculateError();
        SteerPwmAdd=(servo_KP/10.0*servoError)+servo_KD/10.0*(servoError-servoLastError);//�����pd������
        if(SteerPwmAdd>=100)
            SteerPwmAdd=100;
        if(SteerPwmAdd<=-100)
            SteerPwmAdd=-100;
        SteerPwm=(uint32_t)(SteerPwmAdd+servo_Mid);
    }
    else if(turn_flag == turn_right) {
        SteerPwm = servo_Min+10;
        controlCnt++;
    }
    else if(turn_flag == turn_left) {
        SteerPwm = servo_Max-10;
        controlCnt++;
    }
    if(SteerPwm>=servo_Max)//�޷�
        SteerPwm=servo_Max;
    if(SteerPwm<=servo_Min)
        SteerPwm=servo_Min;

    ftm_pwm_duty(servo_Port, servo_Ch, SteerPwm);//���pwm����
    LastSteerSwm=SteerPwm;//��¼pwmֵ
}

