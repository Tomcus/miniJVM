add_library(minijvm
    types/const_pool.cpp
    types/class.cpp
)

target_include_directories(minijvm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(minijvm PUBLIC
    project_settings
    CONAN_PKG::fmt
)

target_add_unit_tests(TARGET minijvm FILES
    utils/ref.unit.cpp
    types/class/attribute.unit.cpp
)

target_add_static_analyzers(minijvm)