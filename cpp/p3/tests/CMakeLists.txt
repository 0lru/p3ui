add_executable(p3_tests "source/TestBuilder.cpp" "source/test_event_loop.cpp")
target_link_libraries(p3_tests PRIVATE p3 Catch2 Catch2::Catch2WithMain)

add_custom_command(
    TARGET p3_tests
    COMMENT "run p3 tests"
    POST_BUILD 
    COMMAND p3_tests
)
