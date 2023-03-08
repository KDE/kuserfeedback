set(userfeedback_widgets_srcs
    feedbackconfigdialog.cpp
    feedbackconfigwidget.cpp
    notificationpopup.cpp
    styleinfosource.cpp
    auditlogbrowserdialog.cpp

    feedbackconfigdialog.h
    feedbackconfigwidget.h
    notificationpopup.h
    styleinfosource.h
    auditlogbrowserdialog.h
)

add_library(KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} ${userfeedback_widgets_srcs})
set_target_properties(KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} PROPERTIES
    SOVERSION ${KUSERFEEDBACK_SOVERSION}
    VERSION ${KUSERFEEDBACK_VERSION}
)
generate_export_header(KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} BASE_NAME KUserFeedbackWidgets)
if(TARGET Qt${QT_MAJOR_VERSION}::Widgets)
    target_link_libraries(KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} PUBLIC Qt${QT_MAJOR_VERSION}::Widgets KUserFeedbackCore${KUSERFEEDBACK_EXTENSION})
else()
    target_link_libraries(KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} PUBLIC ${QT_QTGUI_LIBRARIES} KUserFeedbackCore${KUSERFEEDBACK_EXTENSION})
endif()
target_include_directories(KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KUserFeedback${KUSERFEEDBACK_EXTENSION};${KDE_INSTALL_INCLUDEDIR}>")

ecm_generate_headers(KUserFeedbackWidgets_HEADERS
    HEADER_NAMES
        FeedbackConfigDialog
        FeedbackConfigWidget
        NotificationPopup
        StyleInfoSource
    REQUIRED_HEADERS KUserFeedbackWidgets_HEADERS
)

install(TARGETS KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION} EXPORT KUserFeedback${KUSERFEEDBACK_EXTENSION}Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${KUserFeedbackWidgets_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kuserfeedbackwidgets_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KUserFeedback${KUSERFEEDBACK_EXTENSION}
)
if (QT_MAJOR_VERSION STREQUAL "6")
install(FILES
    ${KUserFeedbackWidgets_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kuserfeedbackwidgets_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KUserFeedback
)
endif()


ecm_generate_pri_file(BASE_NAME KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION}
                      LIB_NAME KUserFeedbackWidgets${KUSERFEEDBACK_EXTENSION}
                      DEPS "KUserFeedbackCore widgets"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
