add_executable(XPLM-HTTP-TEST
    src/main.cpp
    src/DummyDatarefManager.cpp
)

target_include_directories(XPLM-HTTP-TEST PRIVATE
    ../HTTP_SERVER/include
)

target_link_libraries(XPLM-HTTP-TEST PRIVATE
    HTTP_SERVER
)
