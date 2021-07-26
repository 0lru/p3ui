add_executable(p3_tests 

    source/main.cpp
    "source/TestBuilder.cpp"

)

add_test(NAME p3_tests COMMAND p3_tests)
target_link_libraries(p3_tests PUBLIC gtest p3)

add_custom_command(
    TARGET p3_tests
    COMMENT "run p3 tests"
    POST_BUILD 
    COMMAND p3_tests
)
