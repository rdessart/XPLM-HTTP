add_library(XPLM-HTTP MODULE
    src/entrypoint.cpp
)

add_compile_definitions(APL XPLM200 XPLM210 XPLM300 XPLM301 XPLM303 XPLM400 XPLM411)

target_include_directories(XPLM-HTTP PRIVATE
    ../HTTP_SERVER/include
    ../vendors/XPSDK/CHeaders
)

target_link_libraries(XPLM-HTTP PRIVATE
    HTTP_SERVER
    # Assuming XPSDK provides XPLM
    # On macOS, might need to link against XPLM framework
)

# Set output name to .xpl
set_target_properties(XPLM-HTTP PROPERTIES
    PREFIX ""
    SUFFIX ".xpl"
)
