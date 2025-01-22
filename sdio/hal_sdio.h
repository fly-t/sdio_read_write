#ifndef __HAL_SDIO_H__
#define __HAL_SDIO_H__
// Auther: Tengfei
// Date: 2025-01-21
// Description:
#include "sdio.h"

extern DMA_HandleTypeDef hdma_sdio;

void hal_sdio_init(void);

void hal_sdio_getcard_info(HAL_SD_CardInfoTypeDef *CardInfo);
void hal_erase(uint32_t BlockAdd, uint32_t NumberOfBlocks);

void hal_sdio_read_polling(uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout);
void hal_sdio_wtire_polling(uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout);

uint8_t hal_sdio_read_dma(uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);

uint8_t hal_sdio_wtire_dma(uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks);
#endif // __HAL_SDIO_H__
