#include "TrackType.h"

InflectionPoint  InflectionPointL;//������߹յ�ṹ�����
InflectionPoint  InflectionPointR;//�����ұ߹յ�ṹ�����

CrossType Cross;//����ʮ�ֽṹ�����

//ʮ���õ��ı���
//����ɨ�裬��¼�����������
uint8_t RCross_flag = 0;
uint8_t LCross_flag = 0;
uint8_t LRCross_flag = 0;
uint8_t turn_flag = 0;
int normal_flag = 1;
int count = 0;
unsigned char LeftScanCol[COL];
unsigned char RightScanCol[COL];

//���ұ��߽��в�����������ڶ���յ�Ѱ������Ĺյ�
//prt:����һ���ṹ������ĵ�ַ
//Start��ɨ�����ʼ��
//End ��ɨ��Ľ�����
void FindInflectionPointR(InflectionPoint *prt,unsigned char Start,unsigned char End )
{
    int i;
    //��ʼ���ṹ�����
    prt->ScanInflectionPointStart=Start;//����ɨ�����ʼ��
    prt->ScanInflectionPointEnd=End;//����ɨ��Ľ�����
    prt->InflectionPointRow=0;
    prt->InflectionPointCol=0;
    prt->InflectionPointFlag=0;

    for(i=prt->ScanInflectionPointStart; i>=prt->ScanInflectionPointEnd; i--) //����ɨ��̫Զ�����������
    {
        if(RightEdge[i]!=COL-1&&RightEdge[i+1]!=COL-1)
        {
            if((RightEdge[i]-RightEdge[i+1]<=0)
              &&(RightEdge[i-1]-RightEdge[i]>=1)
              &&(RightEdge[i-2]-RightEdge[i]>=2)
              &&(RightEdge[i-3]-RightEdge[i]>=4)
              &&(RightEdge[i]<=151)
              &&(RightEdge[i]>127))//�ҵ��ұ����йյ�
            {
                prt->InflectionPointRow=i;//��¼�յ����
                prt->InflectionPointCol=RightEdge[i];//��¼�յ����
                prt->InflectionPointFlag=1;//����ҵ��ҹյ�
                break;//�˳�for
            }
        }
    }
}

//������߽��в�����������ڶ���յ�Ѱ������Ĺյ�
//prt:����һ���ṹ������ĵ�ַ
//Start��ɨ�����ʼ��
//End ��ɨ��Ľ�����
void FindInflectionPointL(InflectionPoint *prt,unsigned char Start,unsigned char End )
{
    int i;
    //��ʼ���ṹ�����
    prt->ScanInflectionPointStart=Start;//����ɨ�����ʼ��
    prt->ScanInflectionPointEnd=End;//����ɨ��Ľ�����
    prt->InflectionPointRow=0;
    prt->InflectionPointCol=0;
    prt->InflectionPointFlag=0;

    //��յ�
    for(i= prt->ScanInflectionPointStart; i>prt->ScanInflectionPointEnd; i--)
    {
        if(LeftEdge[i]!=0&&LeftEdge[i+1]!=0)
        {
            if((LeftEdge[i]-LeftEdge[i+1]>=0)
              &&(LeftEdge[i]-LeftEdge[i-1]>=1)
              &&(LeftEdge[i]-LeftEdge[i-2]>=2)
              &&(LeftEdge[i]-LeftEdge[i-3]>=4)
              &&(LeftEdge[i]>=37)
              &&(LeftEdge[i]<63))//�ҵ��յ�
            {
                prt->InflectionPointRow=i;//��¼�ùյ����
                prt->InflectionPointCol=LeftEdge[i];//��¼�ùյ����
                prt->InflectionPointFlag=1;//����ҵ���յ�
                break;//�ҵ��˳�
            }
        }
    }
}



void CrossRecognition(CrossType *prt)
{
    //��ʼ������
    prt->CrossLR=0;
    prt->CrossL=0;
    prt->CrossR=0;

    //Ѱ�����ҹյ�
    FindInflectionPointR(&InflectionPointR,StartRow-1,40);
    FindInflectionPointL(&InflectionPointL,StartRow-1,40);
    //�����߽���ұ߽�ͬʱ���ߴ���3�У�����Ϊ������ʮ��
    if( InflectionPointR.InflectionPointFlag
        &&InflectionPointL.InflectionPointFlag
        &&(AllLose>=0 || (LeftLose>=0&&RightLose>=0)))
    {
        prt->CrossLR=1;//ʮ��
    }
    //�ж��ҷֲ�·
    else if(InflectionPointR.InflectionPointFlag //�����ұ߹յ�
            &&RightLose>=0&&LeftLose<3&&AllLose<3)
    {
        prt->CrossR=1;//��־�Ҳ�·
    }
    //�ж���ֲ�·
    else if(InflectionPointL.InflectionPointFlag//������߹յ�
            &&LeftLose>=0&&RightLose<3&&AllLose<3)
    {
        prt->CrossL=1;//��־���·
    }
}

//�õ�ʮ�����͵���ر�־��Ϊ���Ʊ�־λ
static void Crossflag(void)
{
    if(cross_state==0&&normal_flag&&cross_nextstate)
    {
        cross_state = cross_nextstate;
        cross_nextstate = 0;
        normal_flag = 0;
    }
    if(Cross.CrossLR)//ʮ��
    {
        //printf("ʮ��\r\n");
//        NRF2401TXBuffer[3] = (10) & 0xff;
        if(cross_state==cross_straight)
        {
            //printf("ʮ�ֿ���ֱ��\r\n");
//            NRF2401TXBuffer[4] = (11) & 0xff;
            LRCross_flag = cross_straight;
            cross_state = 0;
        }
        else if(cross_state==cross_right)//��ת�ź�
        {
            //printf("ʮ�ֿ�����ת\r\n");
//            NRF2401TXBuffer[4] = (12) & 0xff;
            LRCross_flag = cross_right;
            cross_state = 0;
        }
        else if(cross_state==cross_left)//��ת�ź�
        {
            //printf("ʮ�ֿ�����ת\r\n");
//            NRF2401TXBuffer[4] = (13) & 0xff;
            LRCross_flag = cross_left;
            cross_state = 0;
        }
    }
    else if(Cross.CrossR)//�ҷֲ�
    {
//        printf("�ҷֲ�\r\n");
//        NRF2401TXBuffer[3] = (20) & 0xff;
        if(cross_state==cross_straight)//ֱ���ź�
        {
            //printf("�ҷֲ����ֱ��\r\n");
//            NRF2401TXBuffer[4] = (21) & 0xff;
            RCross_flag = cross_straight;
            cross_state = 0;
        }
        else if(cross_state==cross_right)//��ת�ź�
        {
            //printf("�ҷֲ������ת\r\n");
//            NRF2401TXBuffer[4] = (22) & 0xff;
            RCross_flag = cross_right;
            cross_state = 0;
        }
    }
    else if(Cross.CrossL)//��ֲ�
    {
        //printf("��ֲ�\r\n");
//        NRF2401TXBuffer[3] = (30) & 0xff;
        if(cross_state==cross_straight)//ֱ���ź�
        {
            //printf("��ֲ����ֱ��\r\n");
//            NRF2401TXBuffer[4] = (31) & 0xff;
            LCross_flag = cross_straight;
            cross_state = 0;
        }
        else if(cross_state==cross_left)//��ת�ź�
        {
            //printf("��ֲ������ת\r\n");
//            NRF2401TXBuffer[4] = (32) & 0xff;
            LCross_flag = cross_left;
            cross_state = 0;
        }
    }
//    if(NRF2401TXBuffer[3])
//    {
//        NRF2401TXBuffer[0] = 36 & 0xff;
//        NRF2401_SetTXMode();
//        NRF2401_SendData(NRF2401TXBuffer);
//        NRF2401TXBuffer[3] = 0;
//        NRF2401TXBuffer[4] = 0;
//        NRF2401_SetRXMode();
//    }
}

//�Էֲ���д����ó�����Ŀ�������
void CrossProcess(void)
{
    turn_flag = 0;
    Crossflag();
    //ʮ��
    if(LRCross_flag==cross_straight) {//ֱ��

    } 
    else if(LRCross_flag==cross_right) {//��ת
        if(count>=8) 
        {
            turn_flag = turn_right;
        }
        else
        {
            if((InflectionPointR.InflectionPointFlag==0&&InflectionPointL.InflectionPointFlag==0)
              &&(RightLose>=0||AllLose>=0)) {
                count++;
            } else {
                count=0;
            }
        }
    } 
    else if(LRCross_flag==cross_left) {//��ת
        if(count>=2) 
        {
            turn_flag = turn_left;
        }
        else
        {
            if((InflectionPointL.InflectionPointFlag==0&&InflectionPointR.InflectionPointFlag==0)
              &&(LeftLose>=0||AllLose>=0)) 
            {
                count++;
            } 
            else 
            {
                count=0;
            }
        }
    }
    //�Ҳ�·
    if(RCross_flag==cross_straight) {//ֱ��

    } 
    else if(RCross_flag==cross_right) {//��ת
        if(count>=5)
        {
            turn_flag = turn_right;
        }
        else
        {
            if(InflectionPointR.InflectionPointFlag==0
              &&RightLose>=0&&LeftLose<3) 
            {
                count++;
            } 
            else 
            {
                count=0;
            }
        }
    }
    //���·
    if(LCross_flag==cross_straight) {//ֱ��
        
    } 
    else if(LCross_flag==cross_left) {//��ת
        if(count>=2)
        {
            turn_flag = turn_left;
        }
        else
        {
            if(InflectionPointL.InflectionPointFlag==0
              &&LeftLose>=0&&RightLose<3) 
            {
                count++;
            } 
            else 
            {
                count=0;
            }
        }
    }
    
    //��ͼ��ָ����������ñ�־λ
    if(LeftLose<1 && RightLose<1 && AllLose<1
      && ((ABS(LeftEdge[35]  + Width[35] / 2 - MidLine) < 18) || LeftEdge[35] ==MidLine)
      && ((ABS(RightEdge[35] - Width[35] / 2 - MidLine) < 18) || RightEdge[35]==MidLine)
      && ((ABS(LeftEdge[40]  + Width[40] / 2 - MidLine) < 18) || LeftEdge[40] ==MidLine)
      && ((ABS(RightEdge[40] - Width[40] / 2 - MidLine) < 18) || RightEdge[40]==MidLine)
      && ((ABS(LeftEdge[45]  + Width[45] / 2 - MidLine) < 18) || LeftEdge[45] ==MidLine)
      && ((ABS(RightEdge[45] - Width[45] / 2 - MidLine) < 18) || RightEdge[45]==MidLine)
      && ((ABS(LeftEdge[55]  + Width[55] / 2 - MidLine) < 18) || LeftEdge[55] ==MidLine)
      && ((ABS(RightEdge[55] - Width[55] / 2 - MidLine) < 18) || RightEdge[55]==MidLine)
      && (RCross_flag||LCross_flag||LRCross_flag)
      && (controlCnt>=15||LRCross_flag==cross_straight))
    {
        RCross_flag = 0; LCross_flag = 0; LRCross_flag = 0;
        count = 0;
        controlCnt = 0;
        normal_flag = 1;
        //printf("���óɹ�\r\n");
//        NRF2401TXBuffer[0] = 36 & 0xff;
//        NRF2401TXBuffer[3] = 88 & 0xff;
//        NRF2401_SetTXMode();
//        NRF2401_SendData(NRF2401TXBuffer);
//        NRF2401TXBuffer[3] = 0;
//        NRF2401_SetRXMode();
    }
}

