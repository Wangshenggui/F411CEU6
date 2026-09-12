#include "spi_it.h"


static spi_dma_t *spi_devs[SPI_DEV_MAX];
static uint8_t spi_dev_cnt = 0;

/*SPI注册*/
void SPI_Register(spi_dma_t *dev)
{
    if (spi_dev_cnt < SPI_DEV_MAX)
	{
        dev->tx_complete = 1;
        spi_devs[spi_dev_cnt++] = dev;
    }
}

/* 启动 DMA 发送 */
void SPI_WriteDMA(spi_dma_t *dev, uint8_t *buf, uint32_t len)
{
    while (dev->tx_complete == 0);   /* 等上次发完 */
    dev->tx_complete = 0;
    if (HAL_SPI_Transmit_DMA(dev->hspi, buf, len) != HAL_OK)
	{
        dev->tx_complete = 1;        /* 启动失败，立刻放行 */
    }
}

/*查找绑定的SPI设备*/
static spi_dma_t *spi_find(SPI_HandleTypeDef *hspi)
{
    for (uint8_t i = 0; i < spi_dev_cnt; i++)
	{
        if (spi_devs[i]->hspi == hspi)
		{
            return spi_devs[i];
		}
	}
    return NULL;
}

/*发送完成回调*/
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    spi_dma_t *dev = spi_find(hspi);
    if (dev)
	{
		dev->tx_complete = 1;
	}
}

/*错误回调*/
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    spi_dma_t *dev = spi_find(hspi);
    if (dev)
	{
		dev->tx_complete = 1;
	}
}



