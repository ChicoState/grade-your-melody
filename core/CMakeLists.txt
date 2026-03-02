add_library(core staffLineGrid.cpp presenter.cpp questionBank.cpp quizSession.cpp)
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
