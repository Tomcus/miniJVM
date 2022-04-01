add_executable(jvm main.cpp)

target_link_libraries(jvm
    project_settings
    jvmlib
)

if (CLANG_TIDY)
    set_property(TARGET jvm PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY_OPITONS})
endif()