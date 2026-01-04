cmake_minimum_required(VERSION 3.10.0)
project(XPLM-HTTP-TEST VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp src/*.hpp)

add_executable(XPLM-HTTP-TEST ${SOURCES})

target_include_directories(XPLM-HTTP-TEST PRIVATE
    ../HTTP_SERVER/include
)

target_link_libraries(XPLM-HTTP-TEST PRIVATE
    HTTP_SERVER
)