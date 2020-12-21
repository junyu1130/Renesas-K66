#include "FindTrack.h"

int   MiddleLine[ROW];
int   RightEdge[ROW];
int   LeftEdge[ROW];
int   Width[ROW];//动态路宽

int   LeftLose       = 0;
int   RightLose      = 0;
int   AllLose        = 0;
int   signFrameFlag  = 0;

//设置中线，左线，右线的初始化值
//设置每一行对应的赛道宽度
static void SetInitVal(void)
{
    int i;

    for(i=0; i<ROW; i++)//赋初值
    {
        RightEdge[i]  = COL - 1;
        LeftEdge[i]   = 0;
        MiddleLine[i] = MidLine;
        Width[i]  = 39.372 + 1.0405*i;     //动态路宽
    }
}
//全行扫描和边缘结合提取赛道的中线
//前十行全行扫描
//后面54行，根据上一行的寻线情况来决定当前行的寻线方式和起点
static void SearchCenterBlackline(void)
{
    int16 i         = 0;
    int16 j         = 0;
    uint8 jj        = 0;

    LeftLose        = 0;//变量清零
    RightLose       = 0;
    AllLose         = 0;
    signFrameFlag   = 0;
    SetInitVal();
    //前几行，采用从中间往两边扫描的方法
    for(i=StartRow; i>=ROW-10; i--) //首先找前行，全行扫描
    {
        if(i == StartRow)//首行就以图像中心作为扫描起点
        {
            j = MidLine;
        }
        else
        {
            j = MiddleLine[i+1];//否则就以上一行中点的位置作为本行扫描起点
        }
        if(j <= effective_left)
        {
            j = effective_left;
        }
        while(j >= effective_left)//j>=effective_left有效范围内进行搜寻
        {
            if(bin_image[i][j]==WHITE && bin_image[i][j-1]==BLACK && bin_image[i][j-2]==BLACK)//从右向左找到白黑黑跳变
            {
                LeftEdge[i] = j;//找到则赋值 找不到保持原值0
                break;//跳出本行寻线
            }
            j--;//列数往左移动
        }
        if(i==StartRow) //再找右边界
        {
            j = MidLine;//如果首行，从图像中心开始搜寻
        }
        else
        {
            j = MiddleLine[i+1];//否则从上一行中心位置开始搜寻
        }
        if(j >= effective_right)//有效范围内搜寻右线
        {
            j = effective_right;
        }
        while(j <= effective_right)
        {
            if(bin_image[i][j]==WHITE && bin_image[i][j+1]==BLACK && bin_image[i][j+2]==BLACK)//从左向右找到白白黑跳变点
            {
                RightEdge[i] = j;//找到则赋值   找不到保持原值
                break;//跳出本行寻线
            }
            j++;//列数往右移动
        }
        if (LeftEdge[i] > 61) {
          LeftEdge[i] = MidLine;//标志物影响边界判定到中心
          signFrameFlag = 1;
        }
        if (RightEdge[i] < 116) {
          RightEdge[i] = MidLine;//标志物影响边界判定到中心
          signFrameFlag = 1;
        }
        if(LeftEdge[i]==MidLine||RightEdge[i]==MidLine)//标志物
        {
            if(i == StartRow)
            {
                MiddleLine[i] = MidLine;
            }
            else
            {
                MiddleLine[i] = MiddleLine[i+1];
            }
        }
        else if(LeftEdge[i]>=effective_left && RightEdge[i]<=effective_right)//中线判断，没有丢线
        {
            MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;
        }
        else if(LeftEdge[i]>=effective_left && RightEdge[i]>effective_right)//丢了右线,没有丢左线
        {
            RightLose++;
            //如果当行的
            if(i!=StartRow && ((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5)))//突变
            {
                MiddleLine[i] = MiddleLine[i+1];
            }
            else
            {
                MiddleLine[i] = LeftEdge[i] + Width[i]/2;//正常的话就用半宽补
            }
        }
        else if(LeftEdge[i]<effective_left && RightEdge[i]<=effective_right)//丢了左线
        {
            LeftLose++;
            if(i!=StartRow && (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))//突变
            {
                MiddleLine[i] = MiddleLine[i+1];
            }
            else
            {
                MiddleLine[i] = RightEdge[i] - Width[i]/2;//线宽
            }
        }
        else if(LeftEdge[i]<effective_left && RightEdge[i]>effective_right)//两边都丢了的话
        {
            AllLose++;
            if(i == StartRow)//如果是首行就以图像中心作为中点
            {
                MiddleLine[i] = MidLine;
            }
            else
            {
                MiddleLine[i] = MiddleLine[i+1];//如果不是首行就用上一行的中线作为本行中点
            }
        }
    }
    for(i=ROW-11; i>=25; i--)//查找剩余行
    {
        if(LeftEdge[i+1]>=effective_left && RightEdge[i+1]<=effective_right) //上一行两边都找到 启用边沿扫描
        {
            j = ((LeftEdge[i+1]+10) > effective_right)? effective_right:(LeftEdge[i+1]+10);//先找左边界
            jj = ((LeftEdge[i+1]-5) < effective_left)? effective_left:(LeftEdge[i+1]-5);
            while(j >= jj)
            {
                if(bin_image[i][j]==WHITE && bin_image[i][j-1]==BLACK && bin_image[i][j-2]==BLACK)
                {
                    LeftEdge[i] = j;
                    break;
                }
                j--;
            }
            j = ((RightEdge[i+1]-10) < effective_left)? effective_left:(RightEdge[i+1]-10); //在找右边界
            jj = ((RightEdge[i+1]+5) > effective_right)? effective_right:(RightEdge[i+1]+5);
            while(j <= jj)
            {
                if(bin_image[i][j]==WHITE && bin_image[i][j+1]==BLACK && bin_image[i][j+2]==BLACK)
                {
                    RightEdge[i] = j;
                    break;
                }
                j++;
            }
        }
        else if(LeftEdge[i+1]>=effective_left && RightEdge[i+1]>effective_right)//上一行只找到左边界
        {
            j  = ((LeftEdge[i+1]+10) > effective_right)? effective_right:(LeftEdge[i+1]+10);//左边界用边沿扫描
            jj = ((LeftEdge[i+1]-5) < effective_left)? effective_left:(LeftEdge[i+1]-5);
            while(j >= jj)
            {
                if(bin_image[i][j]==WHITE && bin_image[i][j-1]==BLACK && bin_image[i][j-2]==BLACK)
                {
                    LeftEdge[i] = j;
                    break;
                }
                j--;
            }
            j = MiddleLine[i+1];//上一行丢了右边界用全行扫描
            if(j >= effective_right)
            {
                j = effective_right;
            }
            while(j <= effective_right)
            {
                if(bin_image[i][j]==WHITE && bin_image[i][j+1]==BLACK && bin_image[i][j+2]==BLACK)
                {
                    RightEdge[i] = j;
                    break;
                }
                j++;
            }
        }
        else if(LeftEdge[i+1]<effective_left && RightEdge[i+1]<=effective_right) //上一行只找到右边界
        {
            j = ((RightEdge[i+1]-10) < effective_left)? effective_left:(RightEdge[i+1]-10);//边缘追踪找右边界
            jj = ((RightEdge[i+1]+5) > effective_right)? effective_right:(RightEdge[i+1]+5);
            while(j <= jj)
            {
                if(bin_image[i][j]==WHITE&&bin_image[i][j+1]==BLACK&&bin_image[i][j+2]==BLACK)
                {
                    RightEdge[i] = j;
                    break;
                }
                j++;
            }
            j = MiddleLine[i+1]; //全行扫描找左边界
            if(j < effective_left)
            {
                j = effective_left;
            }
            while(j >= effective_left)
            {
                if(bin_image[i][j]==WHITE && bin_image[i][j-1]==BLACK && bin_image[i][j-2]==BLACK)
                {
                    LeftEdge[i] = j;
                    break;
                }
                j--;
            }
        }
        else if(LeftEdge[i+1]<effective_left && RightEdge[i+1]>effective_right)  //上一行没找到边界
        {
            j = MiddleLine[i+1];   //全行找左边界
            while(j >= effective_left)
            {
                if(bin_image[i][j]==WHITE && bin_image[i][j-1]==BLACK && bin_image[i][j-2]==BLACK)
                {
                    LeftEdge[i] = j;
                    break;
                }
                j--;
            }
            j = MiddleLine[i+1];   //全行找右边界
            while(j<=effective_right)
            {
                if(bin_image[i][j]==WHITE&&bin_image[i][j+1]==BLACK&&bin_image[i][j+2]==BLACK)
                {
                    RightEdge[i] = j;
                    break;
                }
                j++;
            }
        }
        if (LeftEdge[i] > 66) {
          LeftEdge[i] = MidLine;//标志物影响边界，直接判定为中线
          if(i>40) signFrameFlag = 1;
        }
        if (RightEdge[i] < 111) {
          RightEdge[i] = MidLine;
          if(i>40) signFrameFlag = 1;
        }
        if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5) )//突变
        {
            MiddleLine[i] = MiddleLine[i+1];//用上一行
        }
        else
        {
            if(LeftEdge[i]==MidLine||RightEdge[i]==MidLine)//标志物
            {
                MiddleLine[i] = MiddleLine[i+1];
            }
            else if(LeftEdge[i]>=effective_left && RightEdge[i]<=effective_right)//不丢线
            {
                MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;
            }
            else if(LeftEdge[i]>=effective_left && RightEdge[i]>effective_right)//lose right
            {
                RightLose++;
                if(LeftEdge[i+1] >= effective_left)
                {
                    MiddleLine[i] = MiddleLine[i+1]+LeftEdge[i]-LeftEdge[i+1];
                }
                else
                {
                    MiddleLine[i]  = LeftEdge[i] + Width[i]/2;
                }
            }
            else if(LeftEdge[i]<effective_left && RightEdge[i]<=effective_right)//lose left
            {
                LeftLose++;
                if(RightEdge[i+1] <= effective_right)
                {
                    MiddleLine[i] = MiddleLine[i+1]+RightEdge[i]-RightEdge[i+1];
                }
                else
                {
                    MiddleLine[i] = RightEdge[i] - Width[i]/2;
                }
            }
            else if(LeftEdge[i]<effective_left && RightEdge[i]>effective_right)//两边丢线
            {
                AllLose++;
                MiddleLine[i] = MiddleLine[i+1];
            }
        }
    }
}

void findTrack(void)
{
    SearchCenterBlackline();//提取赛道的中线
}
