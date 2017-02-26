set(userfeedback_widgets_srcs
    feedbackconfigdialog.cpp
    feedbackconfigwidget.cpp
    notificationpopup.cpp
)

add_library(UserFeedbackWidgets SHARED ${userfeedback_widgets_srcs})
set_target_properties(UserFeedbackWidgets PROPERTIES
    SOVERSION ${USERFEEDBACK_VERSION}
    VERSION ${USERFEEDBACK_VERSION}
)
generate_export_header(UserFeedbackWidgets)
if(TARGET Qt5::Widgets)
    target_link_libraries(UserFeedbackWidgets PUBLIC Qt5::Widgets UserFeedbackCore)
else()
    target_link_libraries(UserFeedbackWidgets PUBLIC ${QT_QTGUI_LIBRARIES} UserFeedbackCore)
endif()
target_include_directories(UserFeedbackWidgets PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR};>")

install(TARGETS UserFeedbackWidgets EXPORT UserFeedbackTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    feedbackconfigdialog.h
    feedbackconfigwidget.h
    notificationpopup.h
    ${CMAKE_CURRENT_BINARY_DIR}/userfeedbackwidgets_export.h
    DESTINATION include/UserFeedbackWidgets
)

ecm_generate_pri_file(BASE_NAME UserFeedbackWidgets
                      LIB_NAME UserFeedbackWidgets
                      DEPS "UserFeedbackCore widgets"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
