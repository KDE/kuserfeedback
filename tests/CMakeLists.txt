if(TARGET UserFeedbackWidgets AND NOT (CMAKE_SOURCE_DIR EQUAL CMAKE_CURRENT_SOURCE_DIR))
    add_executable(feedbackconfigwidgettest feedbackconfigwidgettest.cpp)
    target_link_libraries(feedbackconfigwidgettest UserFeedbackWidgets)

    add_executable(notificationpopuptest notificationpopuptest.cpp)
    target_link_libraries(notificationpopuptest UserFeedbackWidgets)

    add_executable(orwell orwell.cpp)
    target_link_libraries(orwell UserFeedbackWidgets ${QT_QTGUI_LIBRARIES})
else()
    # stand-alone build for testing includes and cmake config
    message(STATUS "building test application stand-alone")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    find_package(UserFeedback REQUIRED)
    add_executable(orwell orwell.cpp)
    target_link_libraries(orwell UserFeedbackWidgets)
endif()
