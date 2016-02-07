include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/provider/core)

add_executable(orwell orwell.cpp)
target_link_libraries(orwell UserFeedbackWidgets ${QT_QTGUI_LIBRARIES})
