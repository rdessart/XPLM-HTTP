add_library(HTTP_SERVER STATIC
    src/HttpServer.cpp
    src/SimDispatcher.cpp
)

target_include_directories(HTTP_SERVER PUBLIC
    include
)

target_link_libraries(HTTP_SERVER PUBLIC
    httplib::httplib
    spdlog::spdlog
    nlohmann_json::nlohmann_json
)
