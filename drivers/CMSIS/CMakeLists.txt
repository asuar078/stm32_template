add_library(STM32_CMSIS INTERFACE)
target_include_directories(STM32_CMSIS INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Device/ST/STM32F4xx/Include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>

        $<INSTALL_INTERFACE:Device/ST/STM32F4xx/Include>
        $<INSTALL_INTERFACE:Include>

        $<TARGET_PROPERTY:STM32_HAL,INTERFACE_INCLUDE_DIRECTORIES>
        )

target_link_libraries(STM32_CMSIS
        INTERFACE
        STM32_HAL
        )
