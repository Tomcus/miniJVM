add_library(jvmlib
    loader.cpp
)

target_include_directories(jvmlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jvmlib PUBLIC
    project_settings
    utility
    fmt
    spdlog
)