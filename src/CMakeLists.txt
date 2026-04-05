add_library(f77interpret_lib STATIC f77interpret.cpp)
target_include_directories(f77interpret_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/util
)

add_subdirectory(app)
