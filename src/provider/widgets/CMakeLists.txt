set(userfeedback_widgets_srcs
    feedbackconfigdialog.cpp
    feedbackconfigwidget.cpp
    notificationpopup.cpp
    notificationwidget.cpp
)

add_library(UserFeedbackWidgets SHARED ${userfeedback_widgets_srcs})
set_target_properties(UserFeedbackWidgets PROPERTIES
    SOVERSION ${USERFEEDBACK_VERSION}
    VERSION ${USERFEEDBACK_VERSION}
)
generate_export_header(UserFeedbackWidgets)
target_link_libraries(UserFeedbackWidgets PUBLIC KF5::WidgetsAddons UserFeedbackCore)
target_include_directories(UserFeedbackWidgets PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR};>")

install(TARGETS UserFeedbackWidgets EXPORT UserFeedbackTargets ${INSTALL_TARGETS_DEFAULT_ARGS})


ecm_generate_pri_file(BASE_NAME UserFeedbackWidgets
                      LIB_NAME UserFeedbackWidgets
                      DEPS "UserFeedbackCore KF5WidgetsAddons"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
