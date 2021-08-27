add_executable(p3_parser_tests 
    "source/TestTokenizers.cpp"
    "source/TestParser.cpp"
 )
target_link_libraries(p3_parser_tests PRIVATE p3_parser Catch2 Catch2::Catch2WithMain)

add_custom_command(
     TARGET p3_parser_tests
     COMMENT "run parser tests"
     POST_BUILD 
     COMMAND p3_parser_tests
)
