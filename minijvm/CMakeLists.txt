add_library(minijvm
    src/types/const_pool.cpp
    src/types/class.cpp
)

target_include_directories(minijvm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(minijvm PUBLIC
    project_settings
    CONAN_PKG::fmt
)

target_add_unit_tests(TARGET minijvm FILES
    src/utils/ref.unit.cpp
    src/types/class/attribute.unit.cpp
)

# target_add_static_analyzers(minijvm)
