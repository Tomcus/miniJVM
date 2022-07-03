find_package(Doxygen QUIET COMPONENTS dot)

if (${Doxygen_FOUND})
    doxygen_add_docs(
        docs
        ${PROJECT_SOURCE_DIR}/src
        COMMENT "Generate project documentation"
    )
endif()