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
    grid_test
    test_staffLineGrid.cpp
    ${CMAKE_SOURCE_DIR}/core/staffLineGrid.cpp
)

target_include_directories(
    grid_test
    PRIVATE ${CMAKE_SOURCE_DIR}/core
)

target_link_libraries(
    grid_test
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
    test_gridcontroller.cpp
    test_gridcontroller_basic.cpp
    ${CMAKE_SOURCE_DIR}/core/GridController.cpp
    ${CMAKE_SOURCE_DIR}/core/staffLineGrid.cpp
    ${CMAKE_SOURCE_DIR}/core/questionHandler.cpp
    ${CMAKE_SOURCE_DIR}/core/AudioEngine.cpp
)

set_target_properties(grid_controller_test PROPERTIES AUTOMOC ON)

target_include_directories(
    grid_controller_test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/core
        ${CMAKE_SOURCE_DIR}/third_party/tsf
)

target_link_libraries(
    grid_controller_test
    GTest::gtest_main
    Qt6::Core
    Qt6::Multimedia
)

include(GoogleTest)
gtest_discover_tests(hello_test)
gtest_discover_tests(question_handler_test)
gtest_discover_tests(grid_test)
gtest_discover_tests(question_handler_test)
