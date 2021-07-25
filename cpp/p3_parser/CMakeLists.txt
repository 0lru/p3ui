add_library(p3_parser STATIC "source/p3/Parser.cpp")
target_include_directories(p3_parser PUBLIC source)
add_subdirectory(tests)