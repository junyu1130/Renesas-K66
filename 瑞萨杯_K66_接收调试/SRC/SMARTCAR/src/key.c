#include "headfile.h"

//上拉输入 按下为0

void Key_Init(void)						//按键初始化
{
    gpio_init(Key_left_Port,GPI,2);
    gpio_init(Key_right_Port,GPI,2);
    gpio_init(Key_up_Port,GPI,2);
    gpio_init(Key_down_Port,GPI,2);
    gpio_init(Key_1_Port,GPI,2);
    gpio_init(Key_2_Port,GPI,2);
}

uint8_t Key_Check(void)				//检查按下了哪个按键，并返回
{
    //OLED_Write_Char(0,4,'o');
    if(Key_right == 0)
    {
        DelayMs(10);
        if(Key_right == 0)
        {
            while(!Key_right);
            //DelayMs(150);
            //OLED_Write_Char(0,0,1);
            return Keydown_right;
        }
    }

    if(Key_up == 0)
    {
        //OLED_Write_Char(0,2,2);
        DelayMs(10);
        if(Key_up == 0)
        {
            //OLED_Write_Char(0,4,2);
            while(!Key_up);
            //DelayMs(150);
            return Keydown_up;
        }
    }

    if(Key_left == 0)
    {
        DelayMs(10);
        if(Key_left == 0)
        {
            while(!Key_left);
            //DelayMs(150);
            return Keydown_left;
        }
    }

    if(Key_down == 0)
    {
        DelayMs(10);
        if(Key_down == 0)
        {
            while(!Key_down);
            //DelayMs(150);
            return Keydown_down;
        }
    }

    if(Key_1 == 0)
    {
        //OLED_Write_Char(0,2,3);
        DelayMs(10);
        if(Key_1 == 0)
        {
            //OLED_Write_Char(0,4,3);
            while(!Key_1);
            //DelayMs(150);
            return Keydown_ex1;
        }
    }

    if(Key_2 == 0)
    {
        //OLED_Write_Char(0,2,4);
        DelayMs(10);
        if(Key_2 == 0)
        {
            //OLED_Write_Char(0,4,4);
            while(!Key_2);
            //DelayMs(150);
            return Keydown_ex2;
        }
    }

    return Keydown_nokey;
}

