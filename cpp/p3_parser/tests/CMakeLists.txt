add_executable(p3_parser_tests 
    "source/main.cpp"
    "source/TestTokenizers.cpp"
    "source/TestParser.cpp"
 )
add_test(NAME p3_parser_tests COMMAND p3_parser_tests)
target_link_libraries(p3_parser_tests PUBLIC gtest p3_parser)


add_custom_command(
     TARGET p3_parser_tests
     COMMENT "run parser tests"
     POST_BUILD 
     COMMAND p3_parser_tests
)
