#include "TrackType.h"

InflectionPoint  InflectionPointL;//建立左边拐点结构体变量
InflectionPoint  InflectionPointR;//建立右边拐点结构体变量

CrossType Cross;//建立十字结构体变量

//十字用到的变量
//竖向扫描，记录行坐标的数组
uint8_t RCross_flag = 0;
uint8_t LCross_flag = 0;
uint8_t LRCross_flag = 0;
uint8_t turn_flag = 0;
int normal_flag = 1;
int count = 0;
unsigned char LeftScanCol[COL];
unsigned char RightScanCol[COL];

//对右边线进行操作，如果存在多个拐点寻找最近的拐点
//prt:传入一个结构体变量的地址
//Start：扫描的起始行
//End ：扫描的结束行
void FindInflectionPointR(InflectionPoint *prt,unsigned char Start,unsigned char End )
{
    int i;
    //初始化结构体变量
    prt->ScanInflectionPointStart=Start;//设置扫描的起始行
    prt->ScanInflectionPointEnd=End;//设置扫描的结束行
    prt->InflectionPointRow=0;
    prt->InflectionPointCol=0;
    prt->InflectionPointFlag=0;

    for(i=prt->ScanInflectionPointStart; i>=prt->ScanInflectionPointEnd; i--) //不能扫描太远，否则会误判
    {
        if(RightEdge[i]!=COL-1&&RightEdge[i+1]!=COL-1)
        {
            if((RightEdge[i]-RightEdge[i+1]<=0)
              &&(RightEdge[i-1]-RightEdge[i]>=1)
              &&(RightEdge[i-2]-RightEdge[i]>=2)
              &&(RightEdge[i-3]-RightEdge[i]>=4)
              &&(RightEdge[i]<=151)
              &&(RightEdge[i]>127))//找到右边线有拐点
            {
                prt->InflectionPointRow=i;//记录拐点的行
                prt->InflectionPointCol=RightEdge[i];//记录拐点的列
                prt->InflectionPointFlag=1;//标记找到右拐点
                break;//退出for
            }
        }
    }
}

//对左边线进行操作，如果存在多个拐点寻找最近的拐点
//prt:传入一个结构体变量的地址
//Start：扫描的起始行
//End ：扫描的结束行
void FindInflectionPointL(InflectionPoint *prt,unsigned char Start,unsigned char End )
{
    int i;
    //初始化结构体变量
    prt->ScanInflectionPointStart=Start;//设置扫描的起始行
    prt->ScanInflectionPointEnd=End;//设置扫描的结束行
    prt->InflectionPointRow=0;
    prt->InflectionPointCol=0;
    prt->InflectionPointFlag=0;

    //左拐点
    for(i= prt->ScanInflectionPointStart; i>prt->ScanInflectionPointEnd; i--)
    {
        if(LeftEdge[i]!=0&&LeftEdge[i+1]!=0)
        {
            if((LeftEdge[i]-LeftEdge[i+1]>=0)
              &&(LeftEdge[i]-LeftEdge[i-1]>=1)
              &&(LeftEdge[i]-LeftEdge[i-2]>=2)
              &&(LeftEdge[i]-LeftEdge[i-3]>=4)
              &&(LeftEdge[i]>=37)
              &&(LeftEdge[i]<63))//找到拐点
            {
                prt->InflectionPointRow=i;//记录该拐点的行
                prt->InflectionPointCol=LeftEdge[i];//记录该拐点的列
                prt->InflectionPointFlag=1;//标记找到左拐点
                break;//找到退出
            }
        }
    }
}



void CrossRecognition(CrossType *prt)
{
    //初始化变量
    prt->CrossLR=0;
    prt->CrossL=0;
    prt->CrossR=0;

    //寻找左右拐点
    FindInflectionPointR(&InflectionPointR,StartRow-1,40);
    FindInflectionPointL(&InflectionPointL,StartRow-1,40);
    //如果左边界和右边界同时丢线大于3行，就认为遇到了十字
    if( InflectionPointR.InflectionPointFlag
        &&InflectionPointL.InflectionPointFlag
        &&(AllLose>=0 || (LeftLose>=0&&RightLose>=0)))
    {
        prt->CrossLR=1;//十字
    }
    //判断右分岔路
    else if(InflectionPointR.InflectionPointFlag //存在右边拐点
            &&RightLose>=0&&LeftLose<3&&AllLose<3)
    {
        prt->CrossR=1;//标志右岔路
    }
    //判断左分岔路
    else if(InflectionPointL.InflectionPointFlag//存在左边拐点
            &&LeftLose>=0&&RightLose<3&&AllLose<3)
    {
        prt->CrossL=1;//标志左岔路
    }
}

//得到十字类型的相关标志作为控制标志位
static void Crossflag(void)
{
    if(cross_state==0&&normal_flag&&cross_nextstate)
    {
        cross_state = cross_nextstate;
        cross_nextstate = 0;
        normal_flag = 0;
    }
    if(Cross.CrossLR)//十字
    {
        //printf("十字\r\n");
//        NRF2401TXBuffer[3] = (10) & 0xff;
        if(cross_state==cross_straight)
        {
            //printf("十字控制直行\r\n");
//            NRF2401TXBuffer[4] = (11) & 0xff;
            LRCross_flag = cross_straight;
            cross_state = 0;
        }
        else if(cross_state==cross_right)//右转信号
        {
            //printf("十字控制右转\r\n");
//            NRF2401TXBuffer[4] = (12) & 0xff;
            LRCross_flag = cross_right;
            cross_state = 0;
        }
        else if(cross_state==cross_left)//左转信号
        {
            //printf("十字控制左转\r\n");
//            NRF2401TXBuffer[4] = (13) & 0xff;
            LRCross_flag = cross_left;
            cross_state = 0;
        }
    }
    else if(Cross.CrossR)//右分叉
    {
//        printf("右分叉\r\n");
//        NRF2401TXBuffer[3] = (20) & 0xff;
        if(cross_state==cross_straight)//直行信号
        {
            //printf("右分叉控制直行\r\n");
//            NRF2401TXBuffer[4] = (21) & 0xff;
            RCross_flag = cross_straight;
            cross_state = 0;
        }
        else if(cross_state==cross_right)//右转信号
        {
            //printf("右分叉控制右转\r\n");
//            NRF2401TXBuffer[4] = (22) & 0xff;
            RCross_flag = cross_right;
            cross_state = 0;
        }
    }
    else if(Cross.CrossL)//左分叉
    {
        //printf("左分叉\r\n");
//        NRF2401TXBuffer[3] = (30) & 0xff;
        if(cross_state==cross_straight)//直行信号
        {
            //printf("左分叉控制直行\r\n");
//            NRF2401TXBuffer[4] = (31) & 0xff;
            LCross_flag = cross_straight;
            cross_state = 0;
        }
        else if(cross_state==cross_left)//左转信号
        {
            //printf("左分叉控制左转\r\n");
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

//对分叉进行处理，得出舵机的控制中线
void CrossProcess(void)
{
    turn_flag = 0;
    Crossflag();
    //十字
    if(LRCross_flag==cross_straight) {//直行

    } 
    else if(LRCross_flag==cross_right) {//右转
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
    else if(LRCross_flag==cross_left) {//左转
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
    //右岔路
    if(RCross_flag==cross_straight) {//直行

    } 
    else if(RCross_flag==cross_right) {//右转
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
    //左岔路
    if(LCross_flag==cross_straight) {//直行
        
    } 
    else if(LCross_flag==cross_left) {//左转
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
    
    //在图像恢复正常后重置标志位
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
        //printf("重置成功\r\n");
//        NRF2401TXBuffer[0] = 36 & 0xff;
//        NRF2401TXBuffer[3] = 88 & 0xff;
//        NRF2401_SetTXMode();
//        NRF2401_SendData(NRF2401TXBuffer);
//        NRF2401TXBuffer[3] = 0;
//        NRF2401_SetRXMode();
    }
}

