cmake_minimum_required(VERSION 3.20)
project(XPLM-HTTP VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find dependencies
find_package(httplib CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Add subdirectories
add_subdirectory(HTTP_SERVER)
add_subdirectory(XPLM-HTTP)
add_subdirectory(XPLM-HTTP-TEST)
