/*****************************************************
               图像处理部分
*****************************************************/
#include "image_process.h"

uint8_t bin_image[ROW][COL];//二值图

static uint8_t BinArray[256];
uint8_t Ostu_Threshold;
uint8_t threshold=87;//84

//大津法取阈值
static void Ostu(void)
{
    float histogram[256];
    int size = (ROW-0) * COL;
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
    uint8_t *p_image = &image[0][0];
    for (i = 0; i < ROW; ++i)
    {
        for (int j = 0; j < COL; ++j)
        {
            ++histogram[*p_image];
            p_image++;
        }
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
        {   //找到使灰度差最大的值
            maxVariance = variance;
            Ostu_Threshold = i;            //那个值就是阈值
        }
    }
    //printf("Ostu_Threshold = %d\r\n" , Ostu_Threshold);
    //Ostu_Threshold = threshold;
    for (i = Ostu_Threshold; i < 256; i++)
    {   //生成阈值数组，用于二值化加速
        *(BinArray + i) = 1;
    }
    for (i = 0; i < Ostu_Threshold; i++)
    {   // 生成阈值数组，用于二值化加速
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
    for(H = 0; H < ROW; H++)
    {
        for(W = 0; W < COL; W++)
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
    int RowNum, ColNum;
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

void imageProcess(void)//图像处理
{
    binImage();//二值化图像
    Image_filter();//图像滤波
}

