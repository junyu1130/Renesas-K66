#include "FindTrack.h"

int   MiddleLine[ROW];
int   RightEdge[ROW];
int   LeftEdge[ROW];
int   Width[ROW];//��̬·��

int   LeftLose       = 0;
int   RightLose      = 0;
int   AllLose        = 0;
int   signFrameFlag  = 0;

//�������ߣ����ߣ����ߵĳ�ʼ��ֵ
//����ÿһ�ж�Ӧ���������
static void SetInitVal(void)
{
    int i;

    for(i=0; i<ROW; i++)//����ֵ
    {
        RightEdge[i]  = COL - 1;
        LeftEdge[i]   = 0;
        MiddleLine[i] = MidLine;
        Width[i]  = 39.372 + 1.0405*i;     //��̬·��
    }
}
//ȫ��ɨ��ͱ�Ե�����ȡ����������
//ǰʮ��ȫ��ɨ��
//����54�У�������һ�е�Ѱ�������������ǰ�е�Ѱ�߷�ʽ�����
static void SearchCenterBlackline(void)
{
    int16 i         = 0;
    int16 j         = 0;
    uint8 jj        = 0;

    LeftLose        = 0;//��������
    RightLose       = 0;
    AllLose         = 0;
    signFrameFlag   = 0;
    SetInitVal();
    //ǰ���У����ô��м�������ɨ��ķ���
    for(i=StartRow; i>=ROW-10; i--) //������ǰ�У�ȫ��ɨ��
    {
        if(i == StartRow)//���о���ͼ��������Ϊɨ�����
        {
            j = MidLine;
        }
        else
        {
            j = MiddleLine[i+1];//���������һ���е��λ����Ϊ����ɨ�����
        }
        if(j <= effective_left)
        {
            j = effective_left;
        }
        while(j >= effective_left)//j>=effective_left��Ч��Χ�ڽ�����Ѱ
        {
            if(bin_image[i][j]==WHITE && bin_image[i][j-1]==BLACK && bin_image[i][j-2]==BLACK)//���������ҵ��׺ں�����
            {
                LeftEdge[i] = j;//�ҵ���ֵ �Ҳ�������ԭֵ0
                break;//��������Ѱ��
            }
            j--;//���������ƶ�
        }
        if(i==StartRow) //�����ұ߽�
        {
            j = MidLine;//������У���ͼ�����Ŀ�ʼ��Ѱ
        }
        else
        {
            j = MiddleLine[i+1];//�������һ������λ�ÿ�ʼ��Ѱ
        }
        if(j >= effective_right)//��Ч��Χ����Ѱ����
        {
            j = effective_right;
        }
        while(j <= effective_right)
        {
            if(bin_image[i][j]==WHITE && bin_image[i][j+1]==BLACK && bin_image[i][j+2]==BLACK)//���������ҵ��װ׺������
            {
                RightEdge[i] = j;//�ҵ���ֵ   �Ҳ�������ԭֵ
                break;//��������Ѱ��
            }
            j++;//���������ƶ�
        }
        if (LeftEdge[i] > 61) {
          LeftEdge[i] = MidLine;//��־��Ӱ��߽��ж�������
          signFrameFlag = 1;
        }
        if (RightEdge[i] < 116) {
          RightEdge[i] = MidLine;//��־��Ӱ��߽��ж�������
          signFrameFlag = 1;
        }
        if(LeftEdge[i]==MidLine||RightEdge[i]==MidLine)//��־��
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
        else if(LeftEdge[i]>=effective_left && RightEdge[i]<=effective_right)//�����жϣ�û�ж���
        {
            MiddleLine[i] = (LeftEdge[i] + RightEdge[i])/2;
        }
        else if(LeftEdge[i]>=effective_left && RightEdge[i]>effective_right)//��������,û�ж�����
        {
            RightLose++;
            //������е�
            if(i!=StartRow && ((RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5)))//ͻ��
            {
                MiddleLine[i] = MiddleLine[i+1];
            }
            else
            {
                MiddleLine[i] = LeftEdge[i] + Width[i]/2;//�����Ļ����ð��
            }
        }
        else if(LeftEdge[i]<effective_left && RightEdge[i]<=effective_right)//��������
        {
            LeftLose++;
            if(i!=StartRow && (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5))//ͻ��
            {
                MiddleLine[i] = MiddleLine[i+1];
            }
            else
            {
                MiddleLine[i] = RightEdge[i] - Width[i]/2;//�߿�
            }
        }
        else if(LeftEdge[i]<effective_left && RightEdge[i]>effective_right)//���߶����˵Ļ�
        {
            AllLose++;
            if(i == StartRow)//��������о���ͼ��������Ϊ�е�
            {
                MiddleLine[i] = MidLine;
            }
            else
            {
                MiddleLine[i] = MiddleLine[i+1];//����������о�����һ�е�������Ϊ�����е�
            }
        }
    }
    for(i=ROW-11; i>=25; i--)//����ʣ����
    {
        if(LeftEdge[i+1]>=effective_left && RightEdge[i+1]<=effective_right) //��һ�����߶��ҵ� ���ñ���ɨ��
        {
            j = ((LeftEdge[i+1]+10) > effective_right)? effective_right:(LeftEdge[i+1]+10);//������߽�
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
            j = ((RightEdge[i+1]-10) < effective_left)? effective_left:(RightEdge[i+1]-10); //�����ұ߽�
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
        else if(LeftEdge[i+1]>=effective_left && RightEdge[i+1]>effective_right)//��һ��ֻ�ҵ���߽�
        {
            j  = ((LeftEdge[i+1]+10) > effective_right)? effective_right:(LeftEdge[i+1]+10);//��߽��ñ���ɨ��
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
            j = MiddleLine[i+1];//��һ�ж����ұ߽���ȫ��ɨ��
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
        else if(LeftEdge[i+1]<effective_left && RightEdge[i+1]<=effective_right) //��һ��ֻ�ҵ��ұ߽�
        {
            j = ((RightEdge[i+1]-10) < effective_left)? effective_left:(RightEdge[i+1]-10);//��Ե׷�����ұ߽�
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
            j = MiddleLine[i+1]; //ȫ��ɨ������߽�
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
        else if(LeftEdge[i+1]<effective_left && RightEdge[i+1]>effective_right)  //��һ��û�ҵ��߽�
        {
            j = MiddleLine[i+1];   //ȫ������߽�
            while(j >= effective_left)
            {
                if(bin_image[i][j]==WHITE && bin_image[i][j-1]==BLACK && bin_image[i][j-2]==BLACK)
                {
                    LeftEdge[i] = j;
                    break;
                }
                j--;
            }
            j = MiddleLine[i+1];   //ȫ�����ұ߽�
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
          LeftEdge[i] = MidLine;//��־��Ӱ��߽磬ֱ���ж�Ϊ����
          if(i>40) signFrameFlag = 1;
        }
        if (RightEdge[i] < 111) {
          RightEdge[i] = MidLine;
          if(i>40) signFrameFlag = 1;
        }
        if( (RightEdge[i]-LeftEdge[i]) >= (RightEdge[i+1]-LeftEdge[i+1]+5) )//ͻ��
        {
            MiddleLine[i] = MiddleLine[i+1];//����һ��
        }
        else
        {
            if(LeftEdge[i]==MidLine||RightEdge[i]==MidLine)//��־��
            {
                MiddleLine[i] = MiddleLine[i+1];
            }
            else if(LeftEdge[i]>=effective_left && RightEdge[i]<=effective_right)//������
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
            else if(LeftEdge[i]<effective_left && RightEdge[i]>effective_right)//���߶���
            {
                AllLose++;
                MiddleLine[i] = MiddleLine[i+1];
            }
        }
    }
}

void findTrack(void)
{
    SearchCenterBlackline();//��ȡ����������
}
