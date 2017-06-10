set(userfeedback_widgets_srcs
    feedbackconfigdialog.cpp
    feedbackconfigwidget.cpp
    notificationpopup.cpp
    styleinfosource.cpp
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
target_include_directories(UserFeedbackWidgets PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(UserFeedbackWidgets INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KUserFeedback;${KDE_INSTALL_INCLUDEDIR}>")

ecm_generate_headers(UserFeedbackWidgets_HEADERS
    HEADER_NAMES
        FeedbackConfigDialog
        FeedbackConfigWidget
        NotificationPopup
        StyleInfoSource
    REQUIRED_HEADERS UserFeedbackWidgets_HEADERS
)

install(TARGETS UserFeedbackWidgets EXPORT UserFeedbackTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${UserFeedbackWidgets_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/userfeedbackwidgets_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KUserFeedback
)

ecm_generate_pri_file(BASE_NAME UserFeedbackWidgets
                      LIB_NAME UserFeedbackWidgets
                      DEPS "UserFeedbackCore widgets"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
