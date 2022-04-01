add_library(jvmlib
    types/const_pool.cpp
    types/class.cpp
)

target_include_directories(jvmlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(jvmlib PUBLIC
    project_settings
    fmt
    spdlog
)

target_add_unit_tests(TARGET jvmlib FILES
    utils/ref.unit.cpp
    types/class/attribute.unit.cpp
)

target_add_static_analyzers(jvmlib)