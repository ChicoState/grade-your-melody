cmake_minimum_required(VERSION 3.22)
project(GradeYourMelody LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_subdirectory(tests)

qt_add_executable(GradeYourMelody
    app/main.cpp
    core/GridController.cpp
    core/staffLineGrid.h
    core/staffLineGrid.cpp
    core/questionHandler.cpp
)

configure_file(${CMAKE_SOURCE_DIR}/core/questions.csv
               ${CMAKE_BINARY_DIR}/questions.csv
               COPYONLY)


target_include_directories(GradeYourMelody PRIVATE core)

qt_add_resources(GradeYourMelody "app_resources"
    PREFIX "/"
    FILES
        app/Screen01.qml
        app/NoteSlot.qml
        app/App.qml
        app/resources.qrc
)
qt_add_qml_module(GradeYourMelody
    URI GradeYourMelodyUI
    VERSION 1.0
    QML_FILES
        app/App.qml
        app/Screen01.qml
        app/NoteSlot.qml
    RESOURCES
        app/images/staff\ lines\ 2.png
        app/images/title.png
        app/images/note\ transparent\ cut.png
        app/images/note\ transparent\ cut.png
        app/images/quarternote.png
)

target_link_libraries(GradeYourMelody
    PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick
)
