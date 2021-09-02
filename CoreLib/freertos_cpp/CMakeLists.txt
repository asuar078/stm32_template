add_library(freertos_cpp STATIC
        Critical.hpp
        EventGroup.hpp
        EventGroup.cpp
        Mutex.hpp
        Mutex.cpp
        Semaphore.cpp
        Semaphore.hpp
        StreamBuffer.cpp
        StreamBuffer.hpp
        ReadWriteLock.cpp
        ReadWriteLock.hpp
        Queue.cpp
        Queue.hpp
        Task.hpp
        Task.cpp
        Tick.hpp
        TickHook.hpp
        TickHook.cpp
        Timer.hpp
        Timer.cpp
        )


target_link_libraries(freertos_cpp
        PRIVATE
        freertos
        )

# include file directory
target_include_directories(freertos_cpp
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

# compilation flags and other options
target_compile_options(freertos_cpp PRIVATE
        ${FINAL_COMPILE_OPTIONS}
        $<$<COMPILE_LANGUAGE:CXX>:${FINAL_COMPILE_OPTIONS_CXX}>
        )