cmake_minimum_required(VERSION 3.10.0)
project(HTTP_SERVER VERSION 0.1.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp include/*.hpp include/*.h)
add_library(HTTP_SERVER ${SOURCES})


find_package(httplib CONFIG REQUIRED)
target_link_libraries(HTTP_SERVER PUBLIC httplib::httplib)
target_include_directories(HTTP_SERVER PUBLIC include/)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(HTTP_SERVER PUBLIC nlohmann_json::nlohmann_json)

 find_package(spdlog CONFIG REQUIRED)
target_link_libraries(HTTP_SERVER PRIVATE spdlog::spdlog)

if(APPLE)
    target_link_libraries(HTTP_SERVER PUBLIC "-framework CoreFoundation" "-framework CFNetwork")
endif()