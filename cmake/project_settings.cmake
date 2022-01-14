set(CMAKE_CXX_STANDARD 20)

add_library(project_settings INTERFACE)

set_target_properties(project_settings PROPERTIES CXX_STANDARD 20)

target_compile_options(project_settings INTERFACE
    -Wall
    -Wextra
    # -Wpedantic - can't use TRY and MUST
    -Wunused
    -Wmaybe-uninitialized
    -Wshadow
    -Wno-comment
    -Wnon-virtual-dtor
)

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    message(STATUS "Setting up DEBUG build")

    target_compile_definitions(project_settings INTERFACE
        DEBUG
    )

    # target_link_libraries(project_settings INTERFACE
    #     asan
    # )

    # target_compile_options(project_settings INTERFACE
    #     -fno-omit-frame-pointer
    #     -fsanitize=address
    # )

    # target_link_options(project_settings INTERFACE
    #     -fno-omit-frame-pointer
    #     -fsanitize=address
    # )
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "Setting up RELEASE build")

    target_compile_options(project_settings INTERFACE
        -Werror
    )
endif()