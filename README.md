# branch

1. sd_raw(sd卡原始读写)
2. sd_fatfs(文件系统)

# Board

野火指南者: STM32F103VET6

![野火指南者](https://doc.embedfire.com/mcu/stm32/f103zhinanzhe/std/zh/latest/_static/images/f103zhinanzhe.png)

# 使用SDIO配合fatfs读写SD卡

## 原理图

![](https://raw.githubusercontent.com/fly-t/images/main/blog/README-2025-01-21-17-01-49.png)

## 参数摘要

| 配置     | 参数            |
| -------- | --------------- |
| 系统时钟 | 72Mhz           |
| SDIO     | SD卡读写        |
| uart1    | 调试输出 115200 |

## 1.CUBEMX

修改:

1. 这时钟分频器, 默认配置时钟是36Mhz
2. 4bits模式
3. 栈大小调大(SDIO使用了512个byte我给了0x1000(4K))
4. 配置fatfs语言为USA, simplechines 的cc936太占用空间了
5. 是否开启长文件名

![](https://raw.githubusercontent.com/fly-t/images/main/blog/README-2025-01-21-17-06-54.png)

![](https://raw.githubusercontent.com/fly-t/images/main/blog/README-2025-01-21-17-20-17.png)


![](https://raw.githubusercontent.com/fly-t/images/main/blog/README-2025-01-23-15-31-55.png)

bypass 旁路时钟分频器, 最大时钟36Mhz直接作为sdio的时钟


### 1.1 生成代码

## 修改

1. 修改sdio的时钟,注意polling读写时钟要低
2. dma可以高一些
3. 栈大小调大

``` c
void MX_SDIO_SD_Init(void)
{
  hsd.Instance = SDIO;
  hsd.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
  hsd.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
  hsd.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
// -------修改这里(将4B修改为1B,cubemx更新可能会修复)------
//   hsd.Init.BusWide = SDIO_BUS_WIDE_4B; 
  hsd.Init.BusWide = SDIO_BUS_WIDE_1B;
   
  hsd.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd.Init.ClockDiv = 15; // polling read nedd lower clock
  if (HAL_SD_Init(&hsd) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_SD_ConfigWideBusOperation(&hsd, SDIO_BUS_WIDE_4B) != HAL_OK)
  {
    Error_Handler();
  }
}
```

防止cubemx重新覆盖

``` c
    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM7_Init();
    MX_USART1_UART_Init();
    MX_SDIO_SD_Init();
    /* USER CODE BEGIN 2 */
    MX_SDIO_SD_Init_Fix(); //<---------------修复sd_init函数---------------
```

``` c
void MX_SDIO_SD_Init_Fix(void)
{

    /* USER CODE BEGIN SDIO_Init 0 */
    // HAL_SD_DeInit(&hsd);
    /* USER CODE END SDIO_Init 0 */

    /* USER CODE BEGIN SDIO_Init 1 */

    /* USER CODE END SDIO_Init 1 */
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
    /* USER CODE BEGIN SDIO_Init 2 */

    /* USER CODE END SDIO_Init 2 */
}
```

## F103只有一个dma

所以读写需要修改dma的方向. 外设到内存(读) 内存到外设(写)


## FatFs 修改

1. 可根据读写修改只读或者只写..


`sd_diskio.c`

``` c
const Diskio_drvTypeDef  SD_Driver =
{
  SD_initialize,
  SD_status,
  SD_read, 
#if  _USE_WRITE == 1    // <-------------修改为0表示关闭写
  SD_write,
#endif /* _USE_WRITE == 1 */
  
#if  _USE_IOCTL == 1    // <-------------修改为0表示关闭io控制
  SD_ioctl,
#endif /* _USE_IOCTL == 1 */
};
```

## 读写

1. 挂载
