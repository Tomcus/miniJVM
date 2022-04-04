add_executable(class_dump
    main.cpp
)

target_link_libraries(class_dump
    project_settings
    lyra
    minijvm
)

target_add_static_analyzers(class_dump)