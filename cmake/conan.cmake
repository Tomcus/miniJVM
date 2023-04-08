# Run Conan for dependency management
macro(run_conan)
  # Download automatically, you can also just copy the conan.cmake file
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(
      DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
      "${CMAKE_BINARY_DIR}/conan.cmake"
      TLS_VERIFY ON)
  endif()

  set(ENV{CONAN_REVISIONS_ENABLED} 1)
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
  list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

  include(${CMAKE_BINARY_DIR}/conan.cmake)

  # Add (or remove) remotes as needed
  # conan_add_remote(NAME conan-center URL https://conan.bintray.com)
  conan_add_remote(
    NAME
    cci
    URL
    https://center.conan.io
    INDEX
    0)
  conan_add_remote(
    NAME
    bincrafters
    URL
    https://bincrafters.jfrog.io/artifactory/api/conan/public-conan)

  conan_cmake_run(
    CONANFILE conanfile.txt
    BASIC_SETUP CMAKE_TARGETS
    BUILD missing)

endmacro()
