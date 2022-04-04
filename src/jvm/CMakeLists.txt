add_executable(jvm main.cpp)

target_link_libraries(jvm
    project_settings
    minijvm
)

target_add_static_analyzers(jvm)