cmake_minimum_required(VERSION 3.11)
project(minijvm CXX)

include(cmake/docs.cmake)
include(cmake/options.cmake)
include(cmake/project_settings.cmake)
include(cmake/testing.cmake)
include(cmake/fmt.cmake)
include(cmake/spdlog.cmake)
include(cmake/lyra.cmake)

add_subdirectory(src)

add_subdirectory(tests)
