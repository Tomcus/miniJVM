add_library(test_base_with_main INTERFACE)

target_link_libraries(test_base_with_main INTERFACE project_settings CONAN_PKG::catch2)

set(UNIT_TEST_SOURCES "" CACHE INTERNAL "UNIT_TEST_SOURCES")
set(UNIT_TEST_DEPENDENCIES "" CACHE INTERNAL "UNIT_TEST_DEPENDENCIES")

set(VALID_TEST_NAMES_SUFFIX "unit;test")
set(VALID_TEST_FILES_SUFFIX "h;hpp;c;cc;cpp")

function(target_add_unit_tests)
    cmake_parse_arguments(ADD_UNIT_TEST "" "TARGET" "FILES" ${ARGN})

	string(REPLACE "${CMAKE_SOURCE_DIR}" "" PATH_PREFIX ${CMAKE_CURRENT_SOURCE_DIR})
	set(PATH_PREFIX "..${PATH_PREFIX}")
	foreach(FILE ${ADD_UNIT_TEST_FILES})
        MESSAGE(STATUS "Adding test file for ${ADD_UNIT_TEST_TARGET}: ${FILE}")
		if ("${UNIT_TEST_SOURCES}" STREQUAL "")
			set(UNIT_TEST_SOURCES ${PATH_PREFIX}/${FILE} CACHE INTERNAL "UNIT_TEST_SOURCES")
		else()
			set(UNIT_TEST_SOURCES ${UNIT_TEST_SOURCES} ${PATH_PREFIX}/${FILE} CACHE INTERNAL "UNIT_TEST_SOURCES")
		endif()
	endforeach()
    set(UNIT_TEST_DEPENDENCIES ${UNIT_TEST_DEPENDENCIES} ${ADD_UNIT_TEST_TARGET} CACHE INTERNAL "UNIT_TEST_DEPENDENCIES")
endfunction()

function(target_glob_tests target)
    foreach(TEST_NAME ${VALID_TEST_NAMES_SUFFIX})
        foreach(TEST_SUFFIX ${VALID_TEST_FILES_SUFFIX})
            file(GLOB_RECURSE TEST_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.${TEST_NAME}.${TEST_SUFFIX}")
            target_add_unit_tests(TARGET ${target} FILES ${TEST_SRC})
        endforeach()
    endforeach()
endfunction()
