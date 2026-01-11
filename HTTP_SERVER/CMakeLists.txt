cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE debug)
project("HTTP_SERVER" VERSION 1.0.0 LANGUAGES C CXX)

file(GLOB_RECURSE SOURCES
    src/Server/*.hpp
    src/Server/*.cpp
    include/API/*.hpp
    include/API/*.cpp
    include/Server/*.hpp
    include/Server/*.cpp
)

add_library(HTTP_SERVER STATIC ${SOURCES})

target_include_directories(HTTP_SERVER PUBLIC
    include
)
if(APPLE)
    find_library(CARBON_LIBRARY Carbon)
    find_library(FOUNDATION_LIBRARY Foundation)
    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
endif()

target_link_libraries(HTTP_SERVER PUBLIC
    ${XPLM_FRAMEWORK}
    ${CARBON_LIBRARY}
    ${FOUNDATION_LIBRARY}
    ${COREFOUNDATION_LIBRARY}
    httplib::httplib
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)
