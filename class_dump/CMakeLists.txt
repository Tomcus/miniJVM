add_executable(class_dump
    src/main.cpp
)

target_link_libraries(class_dump
    project_settings
    CONAN_PKG::lyra
    minijvm
)

target_add_static_analyzers(class_dump)