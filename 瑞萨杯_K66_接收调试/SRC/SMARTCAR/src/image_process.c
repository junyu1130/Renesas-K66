/*****************************************************
               图像处理部分
*****************************************************/
#include "image_process.h"

#define WHITE 1
#define BLACK 0
#define image_center COL/2
#define MID_LINE 94

extern PID *pid_speed_L, *pid_speed_R;
extern PID *p_pid_servo;

uint8_t bin_image[ROW][COL];//二值图
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

//大津法取阈值
static void Ostu(void)
{
  float histogram[256];
  int size = 70 * 188;
  int i;
  float t;
  float variance;
  float maxVariance = 0, w0 = 0, avgValue = 0;
  float u = 0;// 整幅图的平均灰度
	//清空灰度数组
	for(i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}
  //生成灰度直方图
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
		
  for (i = 0; i < 256; ++i)//归一化
  {
    histogram[i] = histogram[i] / size;
    u += i * histogram[i];
  }
  
  for (i = 0; i < 256; ++i)
  {
    w0 += histogram[i];  //前景灰度
    avgValue += i * histogram[i];
    t = avgValue / w0 - u;
    variance = t * t * w0 / (1 - w0); // g = w0/(1-w0)*(u0-u1)^2
    if (variance > maxVariance)
    { //找到使灰度差最大的值
      maxVariance = variance;
      Ostu_Threshold = i;            //那个值就是阈值
    }
  }
  //printf("Ostu_Threshold = %d\r\n" , Ostu_Threshold);
  for (i = Ostu_Threshold; i < 256; i++)
  { //生成阈值数组，用于二值化加速
    *(BinArray + i) = 1;
  }
  for (i = 0; i < Ostu_Threshold; i++)
  { // 生成阈值数组，用于二值化加速
    *(BinArray + i) = 0;
  }
}
//二值化图像，存储在bin_image中
static void binImage(void)
{
	uint8_t *p_image = &image[0][0];
	uint8_t *p_bin_image = &bin_image[0][0];
	uint8_t H, W;
	
	//大津法根据image数组计算阈值
	Ostu();
	//二值化
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

//二值图像滤波
static void Image_filter(void)
{
  for(RowNum = 1; RowNum < ROW-1; RowNum++)
  {
    for(ColNum = 1; ColNum < COL-1; ColNum++)
    {
      if((bin_image[RowNum][ColNum]+bin_image[RowNum][ColNum-1]+bin_image[RowNum][ColNum+1]+
        bin_image[RowNum-1][ColNum]+bin_image[RowNum-1][ColNum-1]+bin_image[RowNum-1][ColNum+1]+
        bin_image[RowNum+1][ColNum]+bin_image[RowNum+1][ColNum-1]+bin_image[RowNum+1][ColNum+1]) >= 1*5)//判断8-邻接像素的值
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

//避免列溢出
static uint8_t AvoidOverflow(int x)
{
    if (x < 0)
        return 0;
    else if (x > COL - 1)
        return (COL - 1);
    else
        return (uint8_t)x;
}
//避免行溢出
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
uint8_t start_line = 70, finish_line = 69;//起始行、截断行
uint8_t fix_flag = 0;//边线修补标志

//图像行处理 边线
static void ImageProcess_sideline(void)
{
  if (RowNum == 0 || find_startlineFlag == 0) {//第0行或者还没有找到起始行，从行中心向两侧寻找黑点
    if (bin_image[RowNum][image_center] == BLACK) {//中心是黑色
      //向左寻找左线
      for (ColNum = image_center; ColNum > 0; ColNum--) {
        if (bin_image[RowNum][ColNum] == WHITE) {//找到左侧边界
          L_black[RowNum] = AvoidOverflow(ColNum);
          break;
        }
      }
      if (ColNum == 0) {//没有找到左边界
        //printf("第一行没有找到左边界\r\n");
        L_black[RowNum] = 0;
      }
      //向右寻找
      for (ColNum = image_center; ColNum < COL - 1; ColNum++) {
        if (bin_image[RowNum][ColNum] == WHITE) {//找到右侧边界
          R_black[RowNum] = AvoidOverflow(ColNum);
          break;
        }
      }
      if (ColNum == COL - 1) {//没有找到右边界
        //printf("第一行没有找到右边界\r\n");
        R_black[RowNum] = COL - 1;
      }
    } else {
      //printf("第一行中心不是黑色,请求调试!\r\n");
    }
  } else {//从上一行两边开始扫线
    //左线
    if (bin_image[RowNum][L_black[RowNum - 1]] == BLACK) {//此时这一行是黑色,向左寻找白点
      for (ColNum = L_black[RowNum - 1]; ColNum > 0; ColNum--) {
        if (bin_image[RowNum][ColNum] == WHITE) {//找到左线
          L_black[RowNum] = AvoidOverflow(ColNum);
          break;
        }
      }
      if (ColNum == 0) {//没找到
        //printf("从边线扫线没扫到，怎么办\r\n");
        L_black[RowNum] = 0;
      }
    } else {//此时这一行是白色,向右寻找黑点
      for (ColNum = L_black[RowNum - 1]; ColNum < COL - 1; ColNum++)
      {
        if (bin_image[RowNum][ColNum] == BLACK) {//找到左线
          L_black[RowNum] = AvoidOverflow(ColNum - 1);
          break;
        }
      }
      if (ColNum == COL - 1) {//没找到
        //printf("从边线扫线没扫到，怎么办\r\n");
        L_black[RowNum] = COL - 1;
        //记录截断
        if (find_startlineFlag == 1 && find_finishlineFlag == 0)
        {
          finish_line = AvoidOverflow(RowNum - 1);
          find_finishlineFlag = 1;
        }
      }
    }
    //右线
    if (bin_image[RowNum][R_black[RowNum - 1]] == BLACK) {//此时这一行是黑色,向右寻找白点
      for (ColNum = R_black[RowNum - 1]; ColNum < COL - 1; ColNum++) {
        if (bin_image[RowNum][ColNum] == WHITE) {//找到右线
          R_black[RowNum] = AvoidOverflow(ColNum);
          break;
        }
      }
      if (ColNum == COL - 1) {//没找到
        //printf("从边线扫线没扫到，怎么办\r\n");
        R_black[RowNum] = COL - 1;
      }
    } else {//此时这一行是白色,向左寻找黑点
      for (ColNum = R_black[RowNum - 1]; ColNum > 0; ColNum--) {
        if (bin_image[RowNum][ColNum] == BLACK) {//找到右线
          R_black[RowNum] = AvoidOverflow(ColNum + 1);
          break;
        }
      }
      if (ColNum == 0) {//没找到
        //printf("从边线扫线没扫到，怎么办\r\n");
        R_black[RowNum] = 0;
        //记录截断
        if (find_startlineFlag == 1 && find_finishlineFlag == 0)
        {
          finish_line = AvoidOverflow(RowNum - 1);
          find_finishlineFlag = 1;
        }
      }
    }
  }
  /******************中线提取与下一行处理的准备***********************/
  if (L_black[RowNum] != 0 && R_black[RowNum] != COL - 1) {//没有丢线
    if (find_startlineFlag == 0) {
      start_line = RowNum;
      find_startlineFlag = 1;//找到了起始行
    }
    if (find_leftstartlineFlag == 0) {
        find_leftstartlineFlag = 1;//找到左线起始
    }
    if (find_rightstartlineFlag == 0) {
        find_rightstartlineFlag = 1;//找到右线起始
    }
  } else if (L_black[RowNum] != 0 && R_black[RowNum] == COL - 1) {//丢失右线
    if (find_startlineFlag == 0) {
      start_line = RowNum;
      find_startlineFlag = 1;//找到起始行
    }
    if (find_leftstartlineFlag == 0) {
      find_leftstartlineFlag = 1;//找到左线起始
    }
  } else if (L_black[RowNum] == 0 && R_black[RowNum] != COL - 1) {//丢失左线
    if (find_startlineFlag == 0) {
      start_line = RowNum;
      find_startlineFlag = 1;//找到起始行
    }
    if (find_rightstartlineFlag == 0) {
      find_rightstartlineFlag = 1;//找到右线起始
    }
  } else {
     //printf("线都丢光了,赶紧调吧\r\n");
  }
  //简单拟合中线
  LCenter[RowNum] = AvoidOverflow((L_black[RowNum] + R_black[RowNum]) / 2);
  //检查中点是否白点 如果是就判定丢线
  if(bin_image[RowNum][LCenter[RowNum]] == WHITE && find_finishlineFlag == 0) {
      finish_line = AvoidOverflow_Row(RowNum - 1);
  }
  //如果这一行是第一次同时找到两边的边线 从startline到这行进行中线的修补
  if(RowNum <= finish_line) {
    if (fix_flag == 0 && find_leftstartlineFlag == 1 && find_rightstartlineFlag == 1) {
      fix_flag = 1;
      //先计算出来修补和fix_value之间的差值
      int temp_fix = 0;
      temp_fix = L_black[RowNum] - R_black[RowNum] - fixValue[RowNum];
      for (uint8_t i = start_line; i < RowNum; i++) {
        if (L_black[i] == 0) {//根据右线修补 
          LCenter[i] = AvoidOverflow(R_black[i] + (temp_fix + fixValue[i]) / 2);
        } else {//根据左线修补
          LCenter[i] = AvoidOverflow(L_black[i] - (temp_fix + fixValue[i]) / 2);
        }
      }
    }
  }
}

void imageProcess(void)//图像处理
{
  binImage();//二值化图像
  Image_filter();//图像滤波
  
  for (RowNum = 0; RowNum < 70; RowNum++) {//初始化数组
    L_black[RowNum] = 0;
    R_black[RowNum] = COL - 1;
    LCenter[RowNum] = image_center;
  }
  for(RowNum = 0; RowNum < ROW; RowNum++)//循环叠加
  {
    ImageProcess_sideline(); //图像行 处理 边线
  }
  for(RowNum = 0; RowNum < ROW; RowNum++)//循环叠加
  {
    //printf("LCenter[%d]:%d\r\n", RowNum, LCenter[RowNum]);
  }
  
  p_pid_servo->ActualValue = 573 + 200/128.0f * LCenter[25];//舵机实际值
}

