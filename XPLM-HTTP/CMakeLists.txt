cmake_minimum_required(VERSION 3.10.0)
project(XPLM-HTTP VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)
add_library(XPLM-HTTP SHARED ${SOURCES})
target_link_libraries(XPLM-HTTP PRIVATE HTTP_SERVER)

add_compile_definitions(-DXPLM200 -DXPLM210 -DXPLM300 -DXPLM301 -DXPLM303 -DXPLM400 -DXPLM411)

# X-Plane SDK paths
set(XPSDK_DIR "${CMAKE_SOURCE_DIR}/vendors/XPSDK")
set(XPSDK_HEADERS "${XPSDK_DIR}/CHeaders")
# Include X-Plane SDK headers
target_include_directories(XPLM-HTTP PRIVATE "${XPSDK_HEADERS}")

# Link X-Plane frameworks on macOS
if(APPLE)
    add_compile_definitions(-DAPL)
    set(XPSDK_LIBRARIES "${XPSDK_DIR}/Libraries/Mac")
    target_link_libraries(XPLM-HTTP PRIVATE "${XPSDK_LIBRARIES}/XPLM.framework" "${XPSDK_LIBRARIES}/XPWidgets.framework")
    set_target_properties(XPLM-HTTP PROPERTIES
        OUTPUT_NAME "XPLM-HTTP.xpl"
        SUFFIX ""
    )
endif()
