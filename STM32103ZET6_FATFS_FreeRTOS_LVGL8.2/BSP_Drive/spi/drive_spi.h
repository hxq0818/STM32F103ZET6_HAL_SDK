#ifndef _spi_H
#define _spi_H


#include "system.h"



void SPI2_Init(void);
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler);
u8 SPI2_ReadWriteByte(u8 TxData);


#endif
