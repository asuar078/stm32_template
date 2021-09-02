cmake_minimum_required(VERSION 3.16)

# Create project
project(stm32_template
        VERSION 0.0.1
        DESCRIPTION "Template project for STM32F4"
        LANGUAGES C CXX ASM)

# Set standards used
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

# set optional variables
option(EXTRA_WARNING_FLAGS "Add extra warning and error flags" ON)

# Turn off shared libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "do not build shared library" FORCE)
set(BUILD_SHARED OFF CACHE BOOL "do not build shared library" FORCE)
set(BUILD_STATIC ON CACHE BOOL "build static library" FORCE)

# Include toolchain
include(toolchain/stm32f4_gcc.cmake)

#---------------------------------------------------------------------------------------
# Set debug/release build configuration Options
#---------------------------------------------------------------------------------------

# Options for DEBUG build
# -Og   Enables optimizations that do not interfere with debugging.
# -g    Produce debugging information in the operating system’s native format.
set(CMAKE_C_FLAGS_DEBUG "-Og -g ${ARCH_DEBUG_FLAGS}" CACHE INTERNAL "C Compiler options for debug build type")
set(CMAKE_CXX_FLAGS_DEBUG "-Og -g ${ARCH_DEBUG_FLAGS}" CACHE INTERNAL "C++ Compiler options for debug build type")
set(CMAKE_ASM_FLAGS_DEBUG "-g" CACHE INTERNAL "ASM Compiler options for debug build type")
#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "Linker options for debug build type")

# Options for RELEASE build
# -Os   Optimize for size. -Os enables all -O2 optimizations.
# -flto Runs the standard link-time optimizer.
set(CMAKE_C_FLAGS_RELEASE "-Os" CACHE INTERNAL "C Compiler options for release build type")
set(CMAKE_CXX_FLAGS_RELEASE "-Os -flto" CACHE INTERNAL "C++ Compiler options for release build type")
set(CMAKE_ASM_FLAGS_RELEASE "-Os" CACHE INTERNAL "ASM Compiler options for release build type")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto" CACHE INTERNAL "Linker options for release build type")

# compiler compatible flags
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,Clang,GNU>")

# Basic warning flags
list(APPEND WARNING_ERROR_FLAGS_BASIC -Wall -Wextra -pedantic -fstack-protector-all -pipe -Wno-deprecated-declarations
        -Wfatal-errors)

# Additional warning flags
list(APPEND WARNING_ERROR_FLAGS_EXTRA
        -fno-common -finline-small-functions -Wmissing-include-dirs
        -findirect-inlining -finline-limit=150 -feliminate-unused-debug-types
        -Wstrict-aliasing -fmax-errors=5 -Wunreachable-code -Wcast-align -Wcast-qual
        -Wdisabled-optimization -Wformat=2 -Wformat-truncation -Winit-self -Wlogical-op
        -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-overflow=5 -Wswitch-default
        -Wundef -Wno-unused -Wno-variadic-macros -Wno-parentheses -fdiagnostics-show-option
        -Wdouble-promotion)

list(APPEND WARNING_ERROR_FLAGS_EXTRA_CXX
        -Wctor-dtor-privacy -Wnoexcept -Woverloaded-virtual -Wsign-promo -Wstrict-null-sentinel)

# final list of all compilation options
if (${EXTRA_WARNING_FLAGS})
    list(APPEND FINAL_COMPILE_OPTIONS ${WARNING_ERROR_FLAGS_BASIC} ${WARNING_ERROR_FLAGS_EXTRA})
    list(APPEND FINAL_COMPILE_OPTIONS_CXX ${WARNING_ERROR_FLAGS_EXTRA_CXX})
else()
    list(APPEND FINAL_COMPILE_OPTIONS ${WARNING_ERROR_FLAGS_BASIC})
endif ()


# need to globally add header files in this directory
include_directories(core/inc)

add_compile_definitions(
        OUTCOME_DISABLE_EXECINFO
)

# Add CMSIS, HAL and other CubeMx generated libraries
add_subdirectory(drivers/CMSIS)
add_subdirectory(drivers/STM32F4xx_HAL_Driver)
add_subdirectory(third_party)

# Add project libraries
add_subdirectory(core_lib/freertos_cpp)

# Base project sources
set(PROJECT_SOURCES
        core/src/main.cpp
        core/src/freertos.c
        core/src/stm32f4xx_it.c
        core/src/stm32f4xx_hal_msp.c
        core/src/stm32f4xx_hal_timebase_tim.c
        core/src/system_stm32f4xx.c
        core/src/syscalls.c
        core/src/sysmem.c
        core/startup/startup_stm32f446retx.s
        )

# create exe with project name
add_executable(${CMAKE_PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC
        Core/Inc
        )

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC
        STM32_CMSIS
        STM32_HAL
        freertos
        freertos_cpp
        etl
        NamedType
        )

# compilation flags and other options
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
        ${FINAL_COMPILE_OPTIONS}
        $<$<COMPILE_LANGUAGE:CXX>:${FINAL_COMPILE_OPTIONS_CXX}>
        )

stm32_print_size_of_target(${CMAKE_PROJECT_NAME})
stm32_generate_binary_file(${CMAKE_PROJECT_NAME})

# Check of flags and build types
MESSAGE(STATUS "Build type: " ${CMAKE_BUILD_TYPE})
MESSAGE(STATUS "Library Type: " ${LIB_TYPE})
MESSAGE(STATUS "Compiler flags:" ${CMAKE_CXX_COMPILE_FLAGS})
MESSAGE(STATUS "Compiler cxx debug flags:" ${CMAKE_CXX_FLAGS_DEBUG})
MESSAGE(STATUS "Compiler cxx release flags:" ${CMAKE_CXX_FLAGS_RELEASE})
MESSAGE(STATUS "Compiler cxx min size flags:" ${CMAKE_CXX_FLAGS_MINSIZEREL})
MESSAGE(STATUS "Compiler cxx flags:" ${CMAKE_CXX_FLAGS})

get_target_property(MAIN_CFLAGS ${CMAKE_PROJECT_NAME} COMPILE_OPTIONS)
# also see: COMPILE_DEFINITIONS INCLUDE_DIRECTORIES
message("-- Target compiler flags are: ${MAIN_CFLAGS}")
