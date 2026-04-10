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

set_source_files_properties(app/App.qml PROPERTIES QT_RESOURCE_ALIAS App.qml)
set_source_files_properties(app/Screen01.qml PROPERTIES QT_RESOURCE_ALIAS Screen01.qml)
set_source_files_properties(app/NoteSlot.qml PROPERTIES QT_RESOURCE_ALIAS NoteSlot.qml)

set_source_files_properties("app/images/staff lines 2.png" PROPERTIES QT_RESOURCE_ALIAS "images/staff lines 2.png")
set_source_files_properties("app/images/title.png" PROPERTIES QT_RESOURCE_ALIAS "images/title.png")
set_source_files_properties("app/images/note transparent cut.png" PROPERTIES QT_RESOURCE_ALIAS "images/note transparent cut.png")
set_source_files_properties("app/images/quarternote.png" PROPERTIES QT_RESOURCE_ALIAS "images/quarternote.png")
set_source_files_properties("app/images/sharpnote.png" PROPERTIES QT_RESOURCE_ALIAS "images/sharpnote.png")
set_source_files_properties("app/images/notes.png" PROPERTIES QT_RESOURCE_ALIAS "images/notes.png")
set_source_files_properties("app/images/sheet music.png" PROPERTIES QT_RESOURCE_ALIAS "images/sheet music.png")
set_source_files_properties("app/images/staff lines with notes.png" PROPERTIES QT_RESOURCE_ALIAS "images/staff lines with notes.png")
set_source_files_properties("app/images/flatbutton.png" PROPERTIES QT_RESOURCE_ALIAS "images/flatbutton.png")
set_source_files_properties("app/images/naturalbutton.png" PROPERTIES QT_RESOURCE_ALIAS "images/naturalbutton.png")
set_source_files_properties("app/images/sharpbutton.png" PROPERTIES QT_RESOURCE_ALIAS "images/sharpbutton.png")
set_source_files_properties("app/images/gradebutton.png" PROPERTIES QT_RESOURCE_ALIAS "images/gradebutton.png")
set_source_files_properties("app/images/flatnote.png" PROPERTIES QT_RESOURCE_ALIAS "images/flatnote.png")
set_source_files_properties("app/images/eighthnote.png" PROPERTIES QT_RESOURCE_ALIAS "images/eighthnote.png")
set_source_files_properties("app/images/quarterbutton.png" PROPERTIES QT_RESOURCE_ALIAS "images/quarterbutton.png")
set_source_files_properties("app/images/eighthbutton.png" PROPERTIES QT_RESOURCE_ALIAS "images/eighthbutton.png")

qt_add_qml_module(GradeYourMelody
    URI GradeYourMelodyUI
    VERSION 1.0
    RESOURCE_PREFIX /qt/qml
    QML_FILES
        app/App.qml
        app/Screen01.qml
        app/NoteSlot.qml
    RESOURCES
        "app/images/staff lines 2.png"
        app/images/title.png
        "app/images/note transparent cut.png"
        app/images/quarternote.png
        app/images/sharpnote.png
        app/images/notes.png
        "app/images/sheet music.png"
        "app/images/staff lines with notes.png"
        app/images/flatbutton.png
        app/images/naturalbutton.png
        app/images/sharpbutton.png
        app/images/gradebutton.png
        app/images/flatnote.png
        app/images/eighthnote.png
        app/images/quarterbutton.png
        app/images/eighthbutton.png
)

add_custom_command(TARGET GradeYourMelody POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/core/questions.csv
        $<TARGET_FILE_DIR:GradeYourMelody>/questions.csv
)

target_link_libraries(GradeYourMelody
    PRIVATE Qt6::Core Qt6::Gui Qt6::Qml Qt6::Quick
)
