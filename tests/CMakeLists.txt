enable_testing()

add_executable(core_tests test_melody.cpp)
target_link_libraries(core_tests PRIVATE core)

add_test(NAME core_tests COMMAND core_tests)
