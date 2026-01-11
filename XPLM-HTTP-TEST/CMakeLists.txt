cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
project("XPLM-HTTP-TEST" VERSION 1.0.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES
    src/*.hpp
    src/*.cpp
    src/API/*.hpp
    src/API/*.cpp
)

add_executable(XPLM-HTTP-TEST ${SOURCES})

target_include_directories(XPLM-HTTP-TEST PRIVATE
    ../HTTP_SERVER/include
)

target_link_libraries(XPLM-HTTP-TEST PRIVATE
    HTTP_SERVER
)
