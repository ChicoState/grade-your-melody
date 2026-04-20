cmake_minimum_required(VERSION 3.16)
project(GradeYourMelody LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(core)
add_subdirectory(app)
add_subdirectory(tests)
