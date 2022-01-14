add_subdirectory(utility)

add_executable(jvm main.cpp)

target_link_libraries(jvm
    project_settings
    utility
)