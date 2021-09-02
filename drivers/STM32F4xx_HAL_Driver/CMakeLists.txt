add_library(STM32_HAL STATIC

        Src/stm32f4xx_hal.c
        Src/stm32f4xx_hal.c
        Src/stm32f4xx_hal_dma.c
        Src/stm32f4xx_hal_exti.c
        Src/stm32f4xx_hal_flash_ex.c
        Src/stm32f4xx_hal_gpio.c
        Src/stm32f4xx_hal_pwr_ex.c
        Src/stm32f4xx_hal_rcc_ex.c
        Src/stm32f4xx_hal_tim_ex.c
        Src/stm32f4xx_hal_cortex.c
        Src/stm32f4xx_hal_dma_ex.c
        Src/stm32f4xx_hal_flash.c
        Src/stm32f4xx_hal_flash_ramfunc.c
        Src/stm32f4xx_hal_pwr.c
        Src/stm32f4xx_hal_rcc.c
        Src/stm32f4xx_hal_tim.c
        Src/stm32f4xx_hal_uart.c
        )

target_include_directories(STM32_HAL
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Inc>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Inc/Legacy>

        $<INSTALL_INTERFACE:Inc>
        $<INSTALL_INTERFACE:Inc/Legacy>

        PRIVATE
        $<TARGET_PROPERTY:STM32_CMSIS,INTERFACE_INCLUDE_DIRECTORIES>
        )

target_link_libraries(STM32_HAL
        PUBLIC
        STM32_CMSIS
        )
