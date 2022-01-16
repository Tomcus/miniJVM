add_subdirectory(utility)
add_subdirectory(jvmlib)

add_executable(jvm main.cpp)

target_link_libraries(jvm
    project_settings
    utility
    jvmlib
)