# STM32 Template

Another template project for the STM32 Line of microcontroller. This project is based
on the output from the STMCubeMx for the STM32F446RET. 

The only configuration that needs to be set is the STM32_TOOLCHAIN_PATH variable. Either 
set in the CMakeList.txt file before including the toolchain file or passed in
from the command line. 

## Features

- CMake project
- C++ support
- FreeRTOS with C++ wrapper
- Uses the [embedded template library ETL](https://github.com/ETLCPP/etl.git) for embedded safe STL types
- Uses [basic boost outcomes](https://github.com/ned14/outcome) for errors handling
- Uses [Named Typed](https://github.com/joboccara/NamedType) library for better interfaces


## Making Named Types Smaller

The __STDC_HOSTED__ flag doesn't always work so to not include iostream
from the standard in the "named_type_impl.hpp" change the following starting 
on line 17. 

```c++
// comment out
//#if defined(__STDC_HOSTED__)
//#    define FLUENT_HOSTED 1
//#else
//#    define FLUENT_HOSTED 0
//#endif

// redefine
#define FLUENT_HOSTED 0
```
