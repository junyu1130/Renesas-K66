/*****************************************************
               ͼ������
*****************************************************/
#include "image_process.h"

#define WHITE 1
#define BLACK 0
#define image_center COL/2
#define MID_LINE 94

extern PID *pid_speed_L, *pid_speed_R;
extern PID *p_pid_servo;

uint8_t bin_image[ROW][COL];//��ֵͼ
uint8_t Ostu_Threshold = 0;

static uint8_t fixValue [70] =
{
    241,239,237,235,233,231,229,227,225,223,
    221,219,217,215,213,211,209,207,205,203,
    201,199,197,195,193,191,189,187,185,183,
    181,179,178,177,175,173,172,170,169,167,
    172,170,167,154,151,147,142,138,130,126,
    131,127,122,123,119,116,104,96,92,89,
    93,89,81,73,71,71,65,58,54,50,
};
static uint8_t BinArray[256];
static int RowNum, ColNum;
static uint8_t L_black[COL], R_black[COL], LCenter[COL];

//���ȡ��ֵ
static void Ostu(void)
{
  float histogram[256];
  int size = 70 * 188;
  int i;
  float t;
  float variance;
  float maxVariance = 0, w0 = 0, avgValue = 0;
  float u = 0;// ����ͼ��ƽ���Ҷ�
	//��ջҶ�����
	for(i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
  //���ɻҶ�ֱ��ͼ
  uint8_t *p_image = &image[0][1];
  for (i = 0; i < 70; ++i)
  {
    for (int j = 0; j < 188; ++j)
    {//44-374
      ++histogram[*p_image];
      p_image++;
    }
    p_image += 2;
  }
		
  for (i = 0; i < 256; ++i)//��һ��
  {
    histogram[i] = histogram[i] / size;
    u += i * histogram[i];
  }
  
  for (i = 0; i < 256; ++i)
  {
    w0 += histogram[i];  //ǰ���Ҷ�
    avgValue += i * histogram[i];
    t = avgValue / w0 - u;
    variance = t * t * w0 / (1 - w0); // g = w0/(1-w0)*(u0-u1)^2
    if (variance > maxVariance)
    { //�ҵ�ʹ�ҶȲ�����ֵ
      maxVariance = variance;
      Ostu_Threshold = i;            //�Ǹ�ֵ������ֵ
    }
  }
  //printf("Ostu_Threshold = %d\r\n" , Ostu_Threshold);
  for (i = Ostu_Threshold; i < 256; i++)
  { //������ֵ���飬���ڶ�ֵ������
    *(BinArray + i) = 1;
  }
  for (i = 0; i < Ostu_Threshold; i++)
  { // ������ֵ���飬���ڶ�ֵ������
    *(BinArray + i) = 0;
  }
}
//��ֵ��ͼ�񣬴洢��bin_image��
static void binImage(void)
{
	uint8_t *p_image = &image[0][0];
	uint8_t *p_bin_image = &bin_image[0][0];
	uint8_t H, W;
	
	//��򷨸���image���������ֵ
	Ostu();
	//��ֵ��
	for(H = 0; H < 70; H++)
	{
		for(W = 0; W < 188; W++)
		{
      *p_bin_image = BinArray[*p_image];
      p_image++;
      p_bin_image++;
		}
	}
}

//��ֵͼ���˲�
static void Image_filter(void)
{
  for(RowNum = 1; RowNum < ROW-1; RowNum++)
  {
    for(ColNum = 1; ColNum < COL-1; ColNum++)
    {
      if((bin_image[RowNum][ColNum]+bin_image[RowNum][ColNum-1]+bin_image[RowNum][ColNum+1]+
        bin_image[RowNum-1][ColNum]+bin_image[RowNum-1][ColNum-1]+bin_image[RowNum-1][ColNum+1]+
        bin_image[RowNum+1][ColNum]+bin_image[RowNum+1][ColNum-1]+bin_image[RowNum+1][ColNum+1]) >= 1*5)//�ж�8-�ڽ����ص�ֵ
      {
        bin_image[RowNum][ColNum] = WHITE;
      }
      else
      {
        bin_image[RowNum][ColNum] = BLACK;
      }
    }
  }
} 

//���������
static uint8_t AvoidOverflow(int x)
{
    if (x < 0)
        return 0;
    else if (x > COL - 1)
        return (COL - 1);
    else
        return (uint8_t)x;
}
//���������
static uint8_t AvoidOverflow_Row(int x)
{
  if (x < 0)
    return 0;
  else if (x > ROW - 1)
    return (ROW - 1);
  else
    return (uint8_t)x;
}

uint8_t find_startlineFlag = 0, find_finishlineFlag = 0,
      find_leftstartlineFlag = 0, find_rightstartlineFlag = 0;
uint8_t start_line = 70, finish_line = 69;//��ʼ�С��ض���
uint8_t fix_flag = 0;//�����޲���־

//ͼ���д��� ����
static void ImageProcess_sideline(void)
{
  if (RowNum == 0 || find_startlineFlag == 0) {//��0�л��߻�û���ҵ���ʼ�У���������������Ѱ�Һڵ�
    if (bin_image[RowNum][image_center] == BLACK) {//�����Ǻ�ɫ
      //����Ѱ������
      for (ColNum = image_center; ColNum > 0; ColNum--) {
        if (bin_image[RowNum][ColNum] == WHITE) {//�ҵ����߽�
          L_black[RowNum] = AvoidOverflow(ColNum);
          break;
        }
      }
      if (ColNum == 0) {//û���ҵ���߽�
        //printf("��һ��û���ҵ���߽�\r\n");
        L_black[RowNum] = 0;
      }
      //����Ѱ��
      for (ColNum = image_center; ColNum < COL - 1; ColNum++) {
        if (bin_image[RowNum][ColNum] == WHITE) {//�ҵ��Ҳ�߽�
          R_black[RowNum] = AvoidOverflow(ColNum);
          break;
        }
      }
      if (ColNum == COL - 1) {//û���ҵ��ұ߽�
        //printf("��һ��û���ҵ��ұ߽�\r\n");
        R_black[RowNum] = COL - 1;
      }
    } else {
      //printf("��һ�����Ĳ��Ǻ�ɫ,�������!\r\n");
    }
  } else {//����һ�����߿�ʼɨ��
    //����
    if (bin_image[RowNum][L_black[RowNum - 1]] == BLACK) {//��ʱ��һ���Ǻ�ɫ,����Ѱ�Ұ׵�
      for (ColNum = L_black[RowNum - 1]; ColNum > 0; ColNum--) {
        if (bin_image[RowNum][ColNum] == WHITE) {//�ҵ�����
          L_black[RowNum] = AvoidOverflow(ColNum);
          break;
        }
      }
      if (ColNum == 0) {//û�ҵ�
        //printf("�ӱ���ɨ��ûɨ������ô��\r\n");
        L_black[RowNum] = 0;
      }
    } else {//��ʱ��һ���ǰ�ɫ,����Ѱ�Һڵ�
      for (ColNum = L_black[RowNum - 1]; ColNum < COL - 1; ColNum++)
      {
        if (bin_image[RowNum][ColNum] == BLACK) {//�ҵ�����
          L_black[RowNum] = AvoidOverflow(ColNum - 1);
          break;
        }
      }
      if (ColNum == COL - 1) {//û�ҵ�
        //printf("�ӱ���ɨ��ûɨ������ô��\r\n");
        L_black[RowNum] = COL - 1;
        //��¼�ض�
        if (find_startlineFlag == 1 && find_finishlineFlag == 0)
        {
          finish_line = AvoidOverflow(RowNum - 1);
          find_finishlineFlag = 1;
        }
      }
    }
    //����
    if (bin_image[RowNum][R_black[RowNum - 1]] == BLACK) {//��ʱ��һ���Ǻ�ɫ,����Ѱ�Ұ׵�
      for (ColNum = R_black[RowNum - 1]; ColNum < COL - 1; ColNum++) {
        if (bin_image[RowNum][ColNum] == WHITE) {//�ҵ�����
          R_black[RowNum] = AvoidOverflow(ColNum);
          break;
        }
      }
      if (ColNum == COL - 1) {//û�ҵ�
        //printf("�ӱ���ɨ��ûɨ������ô��\r\n");
        R_black[RowNum] = COL - 1;
      }
    } else {//��ʱ��һ���ǰ�ɫ,����Ѱ�Һڵ�
      for (ColNum = R_black[RowNum - 1]; ColNum > 0; ColNum--) {
        if (bin_image[RowNum][ColNum] == BLACK) {//�ҵ�����
          R_black[RowNum] = AvoidOverflow(ColNum + 1);
          break;
        }
      }
      if (ColNum == 0) {//û�ҵ�
        //printf("�ӱ���ɨ��ûɨ������ô��\r\n");
        R_black[RowNum] = 0;
        //��¼�ض�
        if (find_startlineFlag == 1 && find_finishlineFlag == 0)
        {
          finish_line = AvoidOverflow(RowNum - 1);
          find_finishlineFlag = 1;
        }
      }
    }
  }
  /******************������ȡ����һ�д����׼��***********************/
  if (L_black[RowNum] != 0 && R_black[RowNum] != COL - 1) {//û�ж���
    if (find_startlineFlag == 0) {
      start_line = RowNum;
      find_startlineFlag = 1;//�ҵ�����ʼ��
    }
    if (find_leftstartlineFlag == 0) {
        find_leftstartlineFlag = 1;//�ҵ�������ʼ
    }
    if (find_rightstartlineFlag == 0) {
        find_rightstartlineFlag = 1;//�ҵ�������ʼ
    }
  } else if (L_black[RowNum] != 0 && R_black[RowNum] == COL - 1) {//��ʧ����
    if (find_startlineFlag == 0) {
      start_line = RowNum;
      find_startlineFlag = 1;//�ҵ���ʼ��
    }
    if (find_leftstartlineFlag == 0) {
      find_leftstartlineFlag = 1;//�ҵ�������ʼ
    }
  } else if (L_black[RowNum] == 0 && R_black[RowNum] != COL - 1) {//��ʧ����
    if (find_startlineFlag == 0) {
      start_line = RowNum;
      find_startlineFlag = 1;//�ҵ���ʼ��
    }
    if (find_rightstartlineFlag == 0) {
      find_rightstartlineFlag = 1;//�ҵ�������ʼ
    }
  } else {
     //printf("�߶�������,�Ͻ�����\r\n");
  }
  //���������
  LCenter[RowNum] = AvoidOverflow((L_black[RowNum] + R_black[RowNum]) / 2);
  //����е��Ƿ�׵� ����Ǿ��ж�����
  if(bin_image[RowNum][LCenter[RowNum]] == WHITE && find_finishlineFlag == 0) {
      finish_line = AvoidOverflow_Row(RowNum - 1);
  }
  //�����һ���ǵ�һ��ͬʱ�ҵ����ߵı��� ��startline�����н������ߵ��޲�
  if(RowNum <= finish_line) {
    if (fix_flag == 0 && find_leftstartlineFlag == 1 && find_rightstartlineFlag == 1) {
      fix_flag = 1;
      //�ȼ�������޲���fix_value֮��Ĳ�ֵ
      int temp_fix = 0;
      temp_fix = L_black[RowNum] - R_black[RowNum] - fixValue[RowNum];
      for (uint8_t i = start_line; i < RowNum; i++) {
        if (L_black[i] == 0) {//���������޲� 
          LCenter[i] = AvoidOverflow(R_black[i] + (temp_fix + fixValue[i]) / 2);
        } else {//���������޲�
          LCenter[i] = AvoidOverflow(L_black[i] - (temp_fix + fixValue[i]) / 2);
        }
      }
    }
  }
}

void imageProcess(void)//ͼ����
{
  binImage();//��ֵ��ͼ��
  Image_filter();//ͼ���˲�
  
  for (RowNum = 0; RowNum < 70; RowNum++) {//��ʼ������
    L_black[RowNum] = 0;
    R_black[RowNum] = COL - 1;
    LCenter[RowNum] = image_center;
  }
  for(RowNum = 0; RowNum < ROW; RowNum++)//ѭ������
  {
    ImageProcess_sideline(); //ͼ���� ���� ����
  }
  for(RowNum = 0; RowNum < ROW; RowNum++)//ѭ������
  {
    //printf("LCenter[%d]:%d\r\n", RowNum, LCenter[RowNum]);
  }
  
  p_pid_servo->ActualValue = 573 + 200/128.0f * LCenter[25];//���ʵ��ֵ
}

