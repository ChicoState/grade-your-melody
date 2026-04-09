enable_testing()

add_executable(
    hello_test
    hello_test.cc
)

target_link_libraries(
    hello_test
    GTest::gtest_main
)

add_executable(
    question_handler_test
    test_questionhandler.cpp
    ${CMAKE_SOURCE_DIR}/core/questionHandler.cpp
)

target_include_directories(
    question_handler_test
    PRIVATE ${CMAKE_SOURCE_DIR}/core
)

target_link_libraries(
    question_handler_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(question_handler_test)