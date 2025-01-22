# Board

野火指南者: STM32F103VET6

![野火指南者](https://doc.embedfire.com/mcu/stm32/f103zhinanzhe/std/zh/latest/_static/images/f103zhinanzhe.png)

# 使用SDIO读写SD卡

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

![](https://raw.githubusercontent.com/fly-t/images/main/blog/README-2025-01-21-17-06-54.png)

![](https://raw.githubusercontent.com/fly-t/images/main/blog/README-2025-01-21-17-20-17.png)


bypass 旁路时钟分频器, 最大时钟36Mhz直接作为sdio的时钟


### 1.1 生成代码

## 修改

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
  hsd.Init.ClockDiv = 5; // 
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



