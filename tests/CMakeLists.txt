if(TARGET KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} AND NOT (CMAKE_SOURCE_DIR EQUAL CMAKE_CURRENT_SOURCE_DIR))
    add_executable(feedbackconfigwidgettest feedbackconfigwidgettest.cpp)
    target_link_libraries(feedbackconfigwidgettest KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION})

    add_executable(notificationpopuptest notificationpopuptest.cpp)
    target_link_libraries(notificationpopuptest KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION})

    add_executable(orwell orwell.cpp)
    target_link_libraries(orwell KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION})

    qml_lint(orwell.qml)
else()
    # stand-alone build for testing includes and cmake config
    message(STATUS "building test application stand-alone")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    find_package(KUserFeedback${KUSERFEEDBACK_EXTENSION} REQUIRED)
    add_executable(orwell orwell.cpp)
    target_link_libraries(orwell KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION})
endif()
