
add_library(freertos STATIC

        Source/croutine.c
        Source/event_groups.c
        Source/list.c
        Source/tasks.c
        Source/queue.c
        Source/stream_buffer.c
        Source/timers.c
        Source/portable/GCC/ARM_CM4F/port.c

        # include the specified heap allocator
        Source/portable/MemMang/heap_4.c

        Source/CMSIS_RTOS_V2/cmsis_os2.c
        )

# Set include path
target_include_directories (freertos 
        PUBLIC

        Source/include
        Source/portable/GCC/ARM_CM4F
        Source/portable/MemMang
        Source/CMSIS_RTOS_V2

        PRIVATE
        $<TARGET_PROPERTY:STM32_CMSIS,INTERFACE_INCLUDE_DIRECTORIES>
        )

target_link_libraries(freertos
        PUBLIC
        STM32_CMSIS
        )