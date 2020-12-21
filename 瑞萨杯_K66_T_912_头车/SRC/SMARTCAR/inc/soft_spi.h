#ifndef _SOFT_SPI_H_
#define _SOFT_SPI_H_

#include "headfile.h"

#define SPI_SCLK_Port		B9
#define SPI_SCLK				PBin(9)

#define SPI_CS_Port		  C16
#define SPI_CS				  PCin(16)

#define SPI_MOSI_Port   A29
#define SPI_MOSI				PAin(29)

#define SPI_MISO_Port		D1
#define SPI_MISO				PDout(1)

void soft_spi_init(void);			  //spi≥ı ºªØ
uint16_t Soft_SPI_ReadByte(void);
void Soft_SPI_SendByte(uint8_t data);

#endif
