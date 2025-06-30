# Source code checks
add_custom_target(format
    COMMAND clang-format -i -Werror ${SOURCES} ${HEADERS}
    COMMENT "Formatting source files with clang-format"
)

add_custom_target(format-check
    COMMAND clang-format --dry-run -Werror ${SOURCES} ${HEADERS}
    COMMENT "Checking source files with clang-format"
)

add_custom_target(tidy
    COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} ${SOURCES} ${HEADERS}
            --format-style=file
    COMMENT "Running clang-tidy without fixing"
)

add_custom_target(tidy-fix
    COMMAND clang-tidy -p=${CMAKE_BINARY_DIR} ${SOURCES} ${HEADERS}
            --fix --format-style=file
    COMMENT "Running clang-tidy with fixes"
)
