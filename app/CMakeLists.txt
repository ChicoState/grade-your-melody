find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(gym_app main.cpp)
target_link_libraries(gym_app PRIVATE core Qt5::Widgets)
