/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "fatfs.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hal_sdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void rtc_demo(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#include <stdio.h>

// 重写fputc
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM7_Init();
    MX_USART1_UART_Init();
    MX_RTC_Init();
    MX_FATFS_Init();
    /* USER CODE BEGIN 2 */
    // sdio_demo();
    MX_SDIO_SD_Init_Fix();

    FRESULT res = f_mount(&SDFatFS, (TCHAR const *)SDPath, 1);

    switch (res)
    {
    case FR_NO_FILESYSTEM:
        /*
            sfd：指定分区类型：
                0: FDISK格式（带有主分区表）。适用于需要多分区的设备。
                1: SFD格式（整个设备单一分区）。常用于 SD 卡、U 盘等设备。

            au：
                指定簇的大小（Allocation Unit Size），通常为 512、1024、2048 等的倍数。
                如果指定为 0，FATFS 会根据设备的大小自动选择最合适的簇大小。
        */

        res = f_mkfs(SDPath, 0, 0);
        printf("fatfs mkfs\r\n");
        if (res == FR_OK)
        {
            printf("》SD卡已成功格式化文件系统。\r\n");
            /* 格式化后，先取消挂载 */
            res = f_mount(NULL, (TCHAR const *)SDPath, 1);
            /* 重新挂载	*/
            res = f_mount(&SDFatFS, (TCHAR const *)SDPath, 1);
        }
        else
        {
            printf("《《格式化失败。》》\r\n");
            while (1)
                ;
        }
        printf("create fat32 ok mount ok \r\n");

        break;

    default:
        break;
    }

    if (res != FR_OK)
    {
        printf("fatfs mount failed\r\n");
    }
    else
    {
        printf("SDPath:%s\n", SDPath);
    }

    /*----------------------- 文件系统测试：写测试 -----------------------------*/
    /* 打开文件，如果文件不存在则创建它 */
    printf("****** 即将进行文件写入测试... ******\r\n");
    FIL file;  /* 文件对象 */
    UINT fnum; /* 文件成功读写数量 */
    BYTE WriteBuffer[] = "欢迎使用野火STM32开发板 今天是个好日子，新建文件系统测试文件\r\n";

    char tempfilepath[60];
    sprintf(tempfilepath, "%s%s", SDPath, "FatFs读写测试文件.txt"); // 拼接出带逻辑驱动器名的完整路径名

    res = f_open(&file, tempfilepath, FA_CREATE_ALWAYS | FA_WRITE);
    if (res == FR_OK)
    {
        printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
        /* 将指定存储区内容写入到文件内 */
        res = f_write(&file, WriteBuffer, sizeof(WriteBuffer), &fnum);
        if (res == FR_OK)
        {
            printf("》文件写入成功，写入字节数据：%d\r\n", fnum);
            printf("》向文件写入的数据为：\r\n%s\r\n", WriteBuffer);
        }
        else
        {
            printf("！！文件写入失败：(%d)\r\n", res);
        }
        /* 不再读写，关闭文件 */
        f_close(&file);
    }
    else
    {
        printf("！！打开/创建文件失败。\r\n");
    }

    /*------------------- 文件系统测试：读测试 ------------------------------------*/
    // printf("****** 即将进行文件读取测试... ******\r\n");
    // f_res = f_open(&file, tempfilepath, FA_OPEN_EXISTING | FA_READ);
    // if (f_res == FR_OK)
    // {
    //     printf("》打开文件成功。\r\n");
    //     f_res = f_read(&file, ReadBuffer, sizeof(ReadBuffer), &fnum);
    //     if (f_res == FR_OK)
    //     {
    //         printf("》文件读取成功,读到字节数据：%d\r\n", fnum);
    //         printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);
    //         LED_GREEN;
    //     }
    //     else
    //     {
    //         printf("！！文件读取失败：(%d)\r\n", f_res);
    //     }
    // }
    // else
    // {
    //     printf("！！打开文件失败。\r\n");
    // }
    // /* 不再读写，关闭文件 */
    // f_close(&file);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        // rtc_demo();

        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void rtc_demo(void)
{
    RTC_DateTypeDef GetData; // 获取日期结构体

    RTC_TimeTypeDef GetTime; // 获取时间结构体
    /* Get the RTC current Time */
    HAL_RTC_GetTime(&hrtc, &GetTime, RTC_FORMAT_BIN);
    /* Get the RTC current Date */
    HAL_RTC_GetDate(&hrtc, &GetData, RTC_FORMAT_BIN);

    /* Display date Format : yy/mm/dd */
    printf("%02d/%02d/%02d\r\n", 2000 + GetData.Year, GetData.Month, GetData.Date);
    /* Display time Format : hh:mm:ss */
    printf("%02d:%02d:%02d\r\n", GetTime.Hours, GetTime.Minutes, GetTime.Seconds);
    HAL_Delay(1000);
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
