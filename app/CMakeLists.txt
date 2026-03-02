set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Svg)

add_executable(gym_app main.cpp MainWindow.cpp AspectRatioWidget.cpp StaffAreaWidget.cpp StaffWidget.cpp QuizWidget.cpp QuizSelectionWidget.cpp ClefWidget.cpp resources.qrc)
target_link_libraries(gym_app PRIVATE core Qt5::Widgets Qt5::Svg)
target_include_directories(gym_app PRIVATE ${CMAKE_SOURCE_DIR}/core)
