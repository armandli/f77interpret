add_library(f77interpret_lib STATIC f77interpret.cpp)
target_include_directories(f77interpret_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(app)
