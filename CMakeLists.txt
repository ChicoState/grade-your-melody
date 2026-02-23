cmake_minimum_required(VERSION 3.16)

project(LedgerTrainer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure automoc/autouic are enabled (they usually are by default)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find Qt6 (Widgets + Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

# Create executable from main.cpp
add_executable(LedgerTrainer
    main.cpp
)

# Link Qt libs
target_link_libraries(LedgerTrainer
    PRIVATE
        Qt6::Widgets
        Qt6::Multimedia
)
