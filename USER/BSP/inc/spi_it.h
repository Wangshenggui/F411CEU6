#ifndef _SPI_IT_H
#define _SPI_IT_H

#include "system.h"

#define SPI_DEV_MAX   2

typedef struct
{
    SPI_HandleTypeDef *hspi;
    volatile uint8_t tx_complete;   /* 1=空闲, 0=发送中 */
} spi_dma_t;

/* 注册设备 */
void SPI_Register(spi_dma_t *dev);

/* 启动 DMA 发送 */
void SPI_WriteDMA(spi_dma_t *dev, uint8_t *buf, uint32_t len);


#endif
