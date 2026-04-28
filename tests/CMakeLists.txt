enable_testing()

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Core)

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

add_executable(
    grid_controller_test
    test_GridController.cpp
    ${CMAKE_SOURCE_DIR}/core/GridController.cpp
    ${CMAKE_SOURCE_DIR}/core/questionHandler.cpp
    ${CMAKE_SOURCE_DIR}/core/staffLineGrid.cpp
)

target_include_directories(
    grid_controller_test
    PRIVATE ${CMAKE_SOURCE_DIR}/core
)

target_link_libraries(
    grid_controller_test
    PRIVATE
    Qt6::Core
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(question_handler_test)
gtest_discover_tests(grid_controller_test)