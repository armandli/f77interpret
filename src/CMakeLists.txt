add_library(f77interpret_lib STATIC
    f77interpret.cpp
    lexer/lexer.cpp
    lexer/source_file.cpp
    lexer/interactive_source_file.cpp
)
target_include_directories(f77interpret_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/util
    ${CMAKE_SOURCE_DIR}/src/lexer
)

add_subdirectory(app)
