cmake_minimum_required(VERSION 3.11)
project(minijvm CXX)

include(cmake/conan.cmake)
run_conan()

include(cmake/docs.cmake)
include(cmake/options.cmake)
include(cmake/project_settings.cmake)
include(cmake/testing.cmake)

add_subdirectory(minijvm)
add_subdirectory(class_dump)
add_subdirectory(jvm)

add_subdirectory(tests)
