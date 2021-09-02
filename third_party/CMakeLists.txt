
add_library(etl INTERFACE)
target_include_directories(etl SYSTEM INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/etl/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/etl_profile>
        $<INSTALL_INTERFACE:etl/include>
        )
target_link_libraries(etl INTERFACE)

add_subdirectory(FreeRTOS)

add_library(NamedType INTERFACE)
target_include_directories(NamedType
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/NamedType/include>
        $<INSTALL_INTERFACE:NamedType/include>)
