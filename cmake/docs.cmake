find_package(Doxygen REQUIRED dot)

doxygen_add_docs(
    docs
    ${PROJECT_SOURCE_DIR}/src
    COMMENT "Generate project documentation"
)