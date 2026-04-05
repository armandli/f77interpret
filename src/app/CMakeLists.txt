add_executable(f77interpret main.cpp)
target_link_libraries(f77interpret PRIVATE f77interpret_lib Boost::program_options)
