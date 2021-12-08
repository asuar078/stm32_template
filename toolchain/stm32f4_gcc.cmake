# Set cross compilation information
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_CROSSCOMPILING 1)

# prevent from running linker during compiler test
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

message(STATUS "Using STM32 GCC Toolchain")

# GCC Toolchain prefix
if (NOT STM32_TOOLCHAIN_PATH)
    # if no Toolchain is specified use this one
    message("setting toolchain to default value")
    set(STM32_TOOLCHAIN_PATH "$ENV{HOME}/Applications/gcc-arm-none-eabi-10.3-2021.07")
endif ()

message("Toolchain path: ${STM32_TOOLCHAIN_PATH}")

# target triplet used
set(STM32_TARGET_TRIPLET "arm-none-eabi")

# define Toolchain paths
set(TOOLCHAIN_SYSROOT "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}")
set(TOOLCHAIN_BIN_PATH "${STM32_TOOLCHAIN_PATH}/bin")
set(TOOLCHAIN_INC_PATH "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}/include")
set(TOOLCHAIN_LIB_PATH "${STM32_TOOLCHAIN_PATH}/${STM32_TARGET_TRIPLET}/lib")

# change to cross compiling Toolchain
set(CMAKE_C_COMPILER ${TOOLCHAIN_BIN_PATH}/${STM32_TARGET_TRIPLET}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_BIN_PATH}/${STM32_TARGET_TRIPLET}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_BIN_PATH}/${STM32_TARGET_TRIPLET}-gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_BIN_PATH}/${STM32_TARGET_TRIPLET}-objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_BIN_PATH}/${STM32_TARGET_TRIPLET}-objdump)
set(CMAKE_SIZE ${TOOLCHAIN_BIN_PATH}/${STM32_TARGET_TRIPLET}-size)
set(CMAKE_DEBUGGER ${TOOLCHAIN_BIN_PATH}/${STM32_TARGET_TRIPLET}-gdb)
set(CMAKE_CPPFILT ${TOOLCHAIN_BIN_PATH}/${STM32_TARGET_TRIPLET}-c++filt)

message(STATUS "Using C compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER}")

# search for programs in the build host directories
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# avoid checking if compiler works
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

# set link module flags to Toolchain
set(CMAKE_MODULE_LINKER_FLAGS "-L${TOOLCHAIN_LIB_PATH}" CACHE INTERNAL "module link flags")
set(CMAKE_SHARED_LINKER_FLAGS "-L${TOOLCHAIN_LIB_PATH}" CACHE INTERNAL "shared link flags")

# set exe suffix
set(CMAKE_EXECUTABLE_SUFFIX_C .elf)
set(CMAKE_EXECUTABLE_SUFFIX_CXX .elf)
set(CMAKE_EXECUTABLE_SUFFIX_ASM .elf)

# helper function to print exe size
function(stm32_print_size_of_target TARGET)
    add_custom_target(${TARGET}_always_display_size
            ALL COMMAND ${CMAKE_SIZE} ${TARGET}${CMAKE_EXECUTABLE_SUFFIX_C}
            COMMENT "Target Sizes: "
            DEPENDS ${TARGET}
            )
endfunction()

# helper function to generate binary file after build
function(stm32_generate_binary_file TARGET)
    add_custom_command(
            TARGET ${TARGET}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O binary ${TARGET}${CMAKE_EXECUTABLE_SUFFIX_C} ${TARGET}.bin
            BYPRODUCTS ${TARGET}.bin
            COMMENT "Generating binary file ${CMAKE_PROJECT_NAME}.bin"
    )
endfunction()

# helper function to generate Motorola S-Record file after build
function(stm32_generate_srec_file TARGET)
    add_custom_command(
            TARGET ${TARGET}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O srec ${TARGET}${CMAKE_EXECUTABLE_SUFFIX_C} ${TARGET}.srec
            BYPRODUCTS ${TARGET}.srec

            COMMENT "Generating srec file ${CMAKE_PROJECT_NAME}.srec"
    )
endfunction()

# helper function to generate hex file after build
function(stm32_generate_hex_file TARGET)
    add_custom_command(
            TARGET ${TARGET}
            POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -O ihex ${TARGET}${CMAKE_EXECUTABLE_SUFFIX_C} ${TARGET}.hex
            BYPRODUCTS ${TARGET}.hex
            COMMENT "Generating hex file ${CMAKE_PROJECT_NAME}.hex"
    )
endfunction()

# set some compilation definitions to be used in entire project
add_compile_definitions(
        STM32F4
        STM32F446xx
        USE_HAL_DRIVER
)

# Processor architecture flags
set(ARCH_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb --sysroot=${TOOLCHAIN_SYSROOT} ")

# set ASM flags
set(CMAKE_ASM_FLAGS "${ARCH_FLAGS} -x assembler-with-cpp -fdata-sections -ffunction-sections -ffreestanding "
        CACHE INTERNAL "ASM Compiler options")

# set flags used by both C/C++
set(C_AND_CXX_FLAGS "-ffunction-sections -fdata-sections \
    -ffreestanding -fno-builtin -fomit-frame-pointer -mabi=aapcs \
    -fstack-usage "
        )

# set C flags
set(CMAKE_C_FLAGS "${ARCH_FLAGS} ${C_AND_CXX_FLAGS} "
        CACHE INTERNAL "C Compiler options")

# set C++ flags
set(CMAKE_CXX_FLAGS "${ARCH_FLAGS} ${C_AND_CXX_FLAGS} -fno-exceptions -fno-non-call-exceptions -fno-rtti \
    -fno-unwind-tables -fno-threadsafe-statics -fno-asynchronous-unwind-tables -fno-use-cxa-atexit "
        CACHE INTERNAL "C++ Compiler options")

# set linker flags
set(CMAKE_EXE_LINKER_FLAGS
        "${ARCH_FLAGS} \
    --specs=nosys.specs --specs=nano.specs \
    -T ${CMAKE_CURRENT_LIST_DIR}/STM32F446RETX_FLASH.ld \
    -Wl,-Map=stm32f4.map \
    -Wl,--print-memory-usage \
    -Wl,--gc-sections -static \
    -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group \
    "
        CACHE INTERNAL "Linker options")
