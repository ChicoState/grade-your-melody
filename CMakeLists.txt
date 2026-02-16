cmake_minimum_required(VERSION 3.16)
project(GradeYourMelody LANGUAGES CXX)

list(APPEND CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(core)
add_subdirectory(app)
add_subdirectory(tests)
