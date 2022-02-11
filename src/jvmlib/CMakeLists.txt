add_library(jvmlib
    types/class.cpp
)

target_include_directories(jvmlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jvmlib PUBLIC
    project_settings
    fmt
    spdlog
)