set(userfeedback_widgets_srcs
    feedbackconfigdialog.cpp
    feedbackconfigwidget.cpp
    notificationpopup.cpp
    styleinfosource.cpp
    auditlogbrowserdialog.cpp
)

add_library(KUserFeedbackWidgets ${userfeedback_widgets_srcs})
set_target_properties(KUserFeedbackWidgets PROPERTIES
    SOVERSION ${KUSERFEEDBACK_SOVERSION}
    VERSION ${KUSERFEEDBACK_VERSION}
)
generate_export_header(KUserFeedbackWidgets)
if(TARGET Qt${QT_MAJOR_VERSION}::Widgets)
    target_link_libraries(KUserFeedbackWidgets PUBLIC Qt${QT_MAJOR_VERSION}::Widgets KUserFeedbackCore)
else()
    target_link_libraries(KUserFeedbackWidgets PUBLIC ${QT_QTGUI_LIBRARIES} KUserFeedbackCore)
endif()
target_include_directories(KUserFeedbackWidgets PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(KUserFeedbackWidgets INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KUserFeedback;${KDE_INSTALL_INCLUDEDIR}>")

ecm_generate_headers(KUserFeedbackWidgets_HEADERS
    HEADER_NAMES
        FeedbackConfigDialog
        FeedbackConfigWidget
        NotificationPopup
        StyleInfoSource
    REQUIRED_HEADERS KUserFeedbackWidgets_HEADERS
)

install(TARGETS KUserFeedbackWidgets EXPORT KUserFeedbackTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${KUserFeedbackWidgets_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kuserfeedbackwidgets_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KUserFeedback
)

ecm_generate_pri_file(BASE_NAME KUserFeedbackWidgets
                      LIB_NAME KUserFeedbackWidgets
                      DEPS "KUserFeedbackCore widgets"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
