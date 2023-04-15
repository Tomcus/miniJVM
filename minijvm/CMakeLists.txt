add_library(minijvm
    src/execution/instructions.cpp
    src/types/const_pool.cpp
    src/types/class.cpp
    src/types/class/method.cpp
    src/types/basic.cpp
)

target_include_directories(minijvm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

target_link_libraries(minijvm PUBLIC
    project_settings
    CONAN_PKG::fmt
    CONAN_PKG::fmtlog
    CONAN_PKG::expected-lite
)

target_glob_tests(minijvm)
