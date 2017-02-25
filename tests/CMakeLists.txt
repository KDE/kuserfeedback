include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR}/provider/core)

if(TARGET UserFeedbackWidgets)
    add_executable(feedbackconfigwidgettest feedbackconfigwidgettest.cpp)
    target_link_libraries(feedbackconfigwidgettest UserFeedbackWidgets)

    add_executable(notificationpopuptest notificationpopuptest.cpp)
    target_link_libraries(notificationpopuptest UserFeedbackWidgets)

    add_executable(orwell orwell.cpp)
    target_link_libraries(orwell UserFeedbackWidgets ${QT_QTGUI_LIBRARIES})
endif()
