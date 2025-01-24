/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file   fatfs.c
 * @brief  Code for fatfs applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include "rtc.h"
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retSD;  /* Return value for SD */
char SDPath[4]; /* SD logical drive path */
FATFS SDFatFS;  /* File system object for SD logical drive */
FIL SDFile;     /* File object for SD */

/* USER CODE BEGIN Variables */
typedef union
{
    struct
    {
        uint32_t Seconds : 5; // 秒 (0-29)，FATFS 秒数是以2秒为单位
        uint32_t Minutes : 6; // 分钟 (0-59)
        uint32_t Hours : 5;   // 小时 (0-23)
        uint32_t Day : 5;     // 日期 (1-31)
        uint32_t Month : 4;   // 月份 (1-12)
        uint32_t Year : 7;    // 年份 (从1980年起偏移值)
    } Fields;                 // 位域字段
    uint32_t Raw;             // 用于组合访问的DWORD表示
} FATDateTime_t;
/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
    /*## FatFS: Link the SD driver ###########################*/
    retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

    /* USER CODE BEGIN Init */
    /* additional user code for init */
    /* USER CODE END Init */
}

/**
 * @brief  Gets Time from RTC
 * @param  None
 * @retval Time in DWORD
 */
DWORD get_fattime(void)
{
    /* USER CODE BEGIN get_fattime */
    RTC_DateTypeDef GetData; // 获取日期结构体

    RTC_TimeTypeDef GetTime; // 获取时间结构体
    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);

    /* Display date Format : yy/mm/dd */
    // printf("%02d/%02d/%02d\r\n", 2000 + GetData.Year, GetData.Month, GetData.Date);
    /* Display time Format : hh:mm:ss */
    // printf("%02d:%02d:%02d\r\n", GetTime.Hours, GetTime.Minutes, GetTime.Seconds);

    return ((DWORD)((GetData.Year+2000)-1980) << 25) /* Year     */
           | ((DWORD)GetData.Month << 21)       /* Month     */
           | ((DWORD)GetData.Date << 16)        /* Day_m  */
           | ((DWORD)GetTime.Hours << 11)       /* Hour        */
           | ((DWORD)GetTime.Minutes << 5)      /* Min      */
           | ((DWORD)GetTime.Seconds >> 1);     /* Sec        */
    return 0;
    /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
