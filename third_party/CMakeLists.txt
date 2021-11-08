
add_library(etl INTERFACE)
target_include_directories(etl SYSTEM INTERFACE
        etl/include
        etl_profile
        )
target_link_libraries(etl INTERFACE)

add_subdirectory(FreeRTOS)

add_library(NamedType INTERFACE)
target_include_directories(NamedType
    SYSTEM INTERFACE
        NamedType/include
    )
