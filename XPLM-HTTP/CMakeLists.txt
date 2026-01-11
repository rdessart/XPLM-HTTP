cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
project("XPLM-HTTP" VERSION 1.0.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES
    src/API/*.hpp
    src/API/*.cpp
    src/Datarefs/*.hpp
    src/Datarefs/*.cpp
    src/*.hpp
    src/*.cpp
)

add_library(XPLM-HTTP MODULE ${SOURCES})

add_compile_definitions(APL XPLM200 XPLM210 XPLM300 XPLM301 XPLM303 XPLM400 XPLM411)

target_include_directories(XPLM-HTTP PRIVATE
    ../HTTP_SERVER/include
    ../vendors/XPSDK/CHeaders
)

find_library(XPLM_FRAMEWORK XPLM PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/XPSDK/Libraries/Mac)

if(APPLE)
    find_library(CARBON_LIBRARY Carbon)
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
endif()

target_link_libraries(XPLM-HTTP PRIVATE
    HTTP_SERVER
    ${XPLM_FRAMEWORK}
    ${CARBON_LIBRARY}
    ${FOUNDATION_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
)

# Set output name to .xpl
set_target_properties(XPLM-HTTP PROPERTIES
    PREFIX ""
    SUFFIX ".xpl"
)
