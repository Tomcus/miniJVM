enable_testing()

add_executable(unit_tests ${UNIT_TEST_SOURCES})

target_link_libraries(unit_tests PUBLIC test_base_with_main ${UNIT_TEST_DEPENDENCIES})

add_test(UnitTests ${CMAKE_BINARY_DIR}/bin/unit_tests)

function(add_class_dump_test)
    add_test(NAME "Test_run_class_file_dump_${ARGV0}_class"
        COMMAND ${CMAKE_BINARY_DIR}/bin/class_dump class/${ARGV0}.class -o ref/${ARGV0}.out
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_test(NAME "Test_compare_class_file_dump_${ARGV0}_class"
        COMMAND diff ref/${ARGV0}.out out/class_dump/${ARGV0}.out
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction(add_class_dump_test)

add_class_dump_test(Add)
add_class_dump_test(HelloWorld)
add_class_dump_test(Parrot)
add_class_dump_test(TestFuncCall)
add_class_dump_test(FactorialMain)
add_class_dump_test(Factorial)