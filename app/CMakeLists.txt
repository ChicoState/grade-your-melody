cmake_minimum_required(VERSION 3.16)
project(gym_app)

set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt")
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Qml Quick)

qt_add_executable(gym_app
    main.cpp
    ../core/GridController.cpp
    ../core/staffLineGrid.cpp
)

target_include_directories(gym_app PRIVATE ../core)

qt_add_qml_module(gym_app
    URI GradeYourMelodyUI
    VERSION 1.0
    RESOURCE_PREFIX /qt/qml
    QML_FILES
        App.qml
        NoteSlot.qml
        Screen01.qml
        GradeYourMelodyUI/Constants.qml
        GradeYourMelodyUI/EventListModel.qml
        GradeYourMelodyUI/EventListSimulator.qml
    RESOURCES
        images/quarternote.png
        "images/note transparent cut.png"
        "images/staff lines 2.png"
        images/title.png
        "images/sheet music.png"
        images/sharpnote.png
        images/notes.png
        "images/staff lines with notes.png"
        GradeYourMelodyUI/qmldir
)

target_link_libraries(gym_app PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Qml Qt6::Quick
)
