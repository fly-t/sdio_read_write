#include "hal_sdio.h"

typedef struct hal_sdio
{
    SD_HandleTypeDef *hsd;
} hal_sdio_t;

hal_sdio_t hal_sdio_obj;

void hal_sdio_init(void)
{
    hal_sdio_obj.hsd = &hsd;
}

/**
 * @brief 检查sdio执行是否有错误
 * @param  hsd              sdio handle
 * @param  res              result
 * @return * void
 */
static void _check_sdio_res(HAL_SD_CardStateTypeDef res)
{
    if (res != HAL_OK)
    {
        // error handle you need to fix
    }
    else
    {
        HAL_SD_CardStateTypeDef state;
        do
        {
            state = HAL_SD_GetCardState(hal_sdio_obj.hsd);
        } while (state != HAL_SD_CARD_TRANSFER);
    }
}

void hal_sdio_getcard_info(HAL_SD_CardInfoTypeDef *CardInfo)
{
    HAL_StatusTypeDef res = HAL_SD_GetCardInfo(hal_sdio_obj.hsd, CardInfo);
    _check_sdio_res(res);
}

void hal_erase(uint32_t startAddr, uint32_t endAddr)
{
    HAL_StatusTypeDef res = HAL_SD_Erase(hal_sdio_obj.hsd, startAddr, endAddr);
    _check_sdio_res(res);
}

void hal_sdio_read_polling(uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)
{
    HAL_StatusTypeDef res = HAL_SD_ReadBlocks(hal_sdio_obj.hsd, pData, BlockAdd, NumberOfBlocks, Timeout);
    _check_sdio_res(res);
}

void hal_sdio_wtire_polling(uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks, uint32_t Timeout)
{
    HAL_StatusTypeDef res = HAL_SD_WriteBlocks(hal_sdio_obj.hsd, pData, BlockAdd, NumberOfBlocks, Timeout);
    _check_sdio_res(res);
}

uint8_t hal_sdio_read_dma(uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
    HAL_StatusTypeDef Return_Status;
    HAL_SD_CardStateTypeDef SD_Card_Status;

    do
    {
        SD_Card_Status = HAL_SD_GetCardState(hal_sdio_obj.hsd);
    } while (SD_Card_Status != HAL_SD_CARD_TRANSFER);

    /* SDIO DMA DeInit */
    /* SDIO DeInit */
    HAL_DMA_DeInit(&hdma_sdio);
    /* SDIO DMA Init */
    /* SDIO Init */
    hdma_sdio.Instance = DMA2_Channel4;
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_NORMAL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_sdio) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_LINKDMA(hal_sdio_obj.hsd, hdmarx, hdma_sdio);

    Return_Status = HAL_SD_ReadBlocks_DMA(hal_sdio_obj.hsd, pData, BlockAdd, NumberOfBlocks);
    _check_sdio_res(Return_Status);
    return Return_Status;
}

uint8_t hal_sdio_wtire_dma(uint8_t *pData, uint32_t BlockAdd, uint32_t NumberOfBlocks)
{
    HAL_StatusTypeDef Return_Status;
    HAL_SD_CardStateTypeDef SD_Card_Status;

    do
    {
        SD_Card_Status = HAL_SD_GetCardState(hal_sdio_obj.hsd);
    } while (SD_Card_Status != HAL_SD_CARD_TRANSFER);

    /* SDIO DMA DeInit */
    /* SDIO DeInit */
    HAL_DMA_DeInit(&hdma_sdio);
    /* SDIO DMA Init */
    /* SDIO Init */
    hdma_sdio.Instance = DMA2_Channel4;
    hdma_sdio.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_NORMAL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_sdio) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_LINKDMA(hal_sdio_obj.hsd, hdmatx, hdma_sdio);

    Return_Status = HAL_SD_WriteBlocks_DMA(hal_sdio_obj.hsd, pData, BlockAdd, NumberOfBlocks);
    _check_sdio_res(Return_Status);
    return Return_Status;
}

void MX_SDIO_SD_Init_Fix(void)
{
    hsd.Instance = SDIO;
    hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
    hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
    hsd.Init.ClockDiv = 15;
    if (HAL_SD_Init(&hsd) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
    {
        Error_Handler();
    }
}

#include "stdio.h" // uart printf
#include "rtc.h"

void sdio_demo(void)
{
    MX_SDIO_SD_Init_Fix();
    hal_sdio_init();
    HAL_SD_CardInfoTypeDef CardInfo;
    hal_sdio_getcard_info(&CardInfo);

    // erase
    uint32_t start_index = 0;
    uint32_t end_index = 100;
    hal_erase(start_index, end_index);

    // sd write
    uint8_t pData[BLOCKSIZE] = "hello321\n";

    uint32_t BlockAdd = 1;
    uint32_t NumberOfBlocks = 1;
    uint32_t Timeout = 1000;

    // hal_sdio_wtire_polling(pData, BlockAdd, NumberOfBlocks, Timeout);
    hal_sdio_wtire_dma(pData, BlockAdd, NumberOfBlocks);

    uint8_t prxData[BLOCKSIZE];
    // hal_sdio_read_polling(prxData, BlockAdd, NumberOfBlocks, Timeout);
    hal_sdio_read_dma(prxData, BlockAdd, NumberOfBlocks);

   
    printf("%s\n", prxData);
}
