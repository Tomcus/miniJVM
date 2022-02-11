set(CMAKE_CXX_STANDARD 20)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(project_settings INTERFACE)

set_target_properties(project_settings PROPERTIES CXX_STANDARD 20)

target_compile_options(project_settings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wunused
    -Wmaybe-uninitialized
    -Wshadow
    -Wno-comment
    -Wnon-virtual-dtor
)

message(STATUS "Configuring ${CMAKE_BUILD_TYPE} build type.")

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" OR ${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_link_libraries(project_settings INTERFACE
            asan
        )

        target_compile_options(project_settings INTERFACE
            -fno-omit-frame-pointer
            -fsanitize=address
        )
    endif()

    target_compile_definitions(project_settings INTERFACE
        DEBUG
    )

endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release" OR ${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    target_compile_options(project_settings INTERFACE
        -Werror
    )
endif()

find_program(CLANG_TIDY NAMES clang-tidy)
if (CLANG_TIDY)
    message(STATUS "Adding clang-tidy")
    set_property(TARGET project_settings PROPERTY CXX_CLANG_TIDY "clang-tidy;-checks=-*,performance-*,modernize-*,clang-analyzer-*,concurrency-*,readability-*")
    # set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=-*,performance-*,modernize-*,clang-analyzer-*,concurrency-*,readability-*,-modernize-use-trailing-return-type")
endif()

find_program(CPP_CHECK NAMES cppcheck)
if (CPP_CHECK)
    message(STATUS "Adding cppcheck")
    set_property(TARGET project_settings PROPERTY CXX_CPPCHECK cppcheck)
endif()

find_program(IWYU NAMES include-what-you-use)
if (IWYU)
    message(STATUS "Adding include-what-you-use")
    set_property(TARGET project_settings PROPERTY CXX_INCLUDE_WHAT_YOU_USE include-what-you-use)
endif()