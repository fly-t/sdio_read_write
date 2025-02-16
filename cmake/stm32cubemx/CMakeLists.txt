cmake_minimum_required(VERSION 3.22)

project(stm32cubemx)
add_library(stm32cubemx INTERFACE)

# Enable CMake support for ASM and C languages
enable_language(C ASM)

target_compile_definitions(stm32cubemx INTERFACE 
	USE_HAL_DRIVER 
	STM32F103xE
    $<$<CONFIG:Debug>:DEBUG>
)

target_include_directories(stm32cubemx INTERFACE
    ../../Core/Inc
    ../../FATFS/Target
    ../../FATFS/App
    ../../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy
    ../../Drivers/STM32F1xx_HAL_Driver/Inc
    ../../Middlewares/Third_Party/FatFs/src
    ../../Middlewares/Third_Party/FatFs/src/drivers
    ../../Drivers/CMSIS/Device/ST/STM32F1xx/Include
    ../../Drivers/CMSIS/Include
)

target_sources(stm32cubemx INTERFACE
    ../../Core/Src/main.c
    ../../Core/Src/gpio.c
    ../../Core/Src/dma.c
    ../../Core/Src/rtc.c
    ../../Core/Src/sdio.c
    ../../Core/Src/tim.c
    ../../Core/Src/usart.c
    ../../Core/Src/stm32f1xx_it.c
    ../../Core/Src/stm32f1xx_hal_msp.c
    ../../FATFS/Target/bsp_driver_sd.c
    ../../FATFS/App/fatfs.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rtc_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_sdmmc.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_sd.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_mmc.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c
    ../../Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
    ../../Core/Src/system_stm32f1xx.c
    ../../Middlewares/Third_Party/FatFs/src/diskio.c
    ../../Middlewares/Third_Party/FatFs/src/ff.c
    ../../Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
    ../../Middlewares/Third_Party/FatFs/src/option/syscall.c
    ../../Middlewares/Third_Party/FatFs/src/option/ccsbcs.c
    ../../Middlewares/Third_Party/FatFs/src/drivers/sd_diskio.c
    ../../Core/Src/sysmem.c
    ../../Core/Src/syscalls.c
    ../../startup_stm32f103xe.s
)

target_link_directories(stm32cubemx INTERFACE
)

target_link_libraries(stm32cubemx INTERFACE
)

# Validate that STM32CubeMX code is compatible with C standard
if(CMAKE_C_STANDARD LESS 11)
    message(ERROR "Generated code requires C11 or higher")
endif()


