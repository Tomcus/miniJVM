add_executable(class_dump
    main.cpp
)

target_link_libraries(class_dump
    project_settings
    lyra
    jvmlib
)

if (CLANG_TIDY)
    set_property(TARGET class_dump PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_OPITONS})
endif()