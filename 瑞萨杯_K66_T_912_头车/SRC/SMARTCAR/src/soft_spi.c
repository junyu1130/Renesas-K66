#include "soft_spi.h"

void soft_spi_init(void)
{
    gpio_init(SPI_SCLK_Port,GPO,0);
    gpio_init(SPI_CS_Port,GPO,0);
    gpio_init(SPI_MOSI_Port,GPO,0);
    gpio_init(SPI_MISO_Port,GPI,0);
    SPI_CS = 1;
    SPI_SCLK = 1;
    SPI_MOSI = 0;
}

//SPI读取数据一个字节
uint16_t Soft_SPI_ReadByte(void)
{
	uint16_t data = 0;
	SPI_CS = 0;
  for(uint8_t i=0; i<16; i++)
  {
    SPI_SCLK=1;
    SPI_SCLK=0;
    data <<= 1;//左移
    data = data | SPI_MISO;// 接收数据
  }
	SPI_CS = 1;
	
	return data;
}

//发送数据
void Soft_SPI_SendByte(uint8_t data)
{
	SPI_CS = 0;
	for(uint8_t i=0;i<8;i++)
	{	
		SPI_SCLK=0;				 
		if((data&0x80) == 0x80)// 处理得到一位并发送
		{
			SPI_MOSI=1;
		}			
		else
		{
			SPI_MOSI=0;
		}		
		data <<= 1;// 数据左移1位		
		SPI_SCLK=1;// 上升沿写入数据 	
	}	
	SPI_CS = 1;
}
