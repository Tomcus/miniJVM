add_library(utility INTERFACE)

target_include_directories(utility INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(utility INTERFACE
    project_settings
)