ecm_create_qm_loader(userfeedback_core_QM_LOADER userfeedbackprovider5_qt)

if (ECM_VERSION VERSION_GREATER "5.67.0")
ecm_qt_declare_logging_category(userfeedback_core_debug_srcs HEADER logging_p.h IDENTIFIER KUserFeedback::Log CATEGORY_NAME org.kde.UserFeedback
    DESCRIPTION "User Feedback Framework" EXPORT KUSERFEEDBACK)
else()
    set(userfeedback_core_debug_srcs
        logging_p.cpp
        logging_p.h)
endif()

set(userfeedback_core_srcs
    abstractdatasource.cpp
    applicationversionsource.cpp
    compilerinfosource.cpp
    cpuinfosource.cpp
    feedbackconfiguicontroller.cpp
    localeinfosource.cpp
    openglinfosource.cpp
    platforminfosource.cpp
    propertyratiosource.cpp
    provider.cpp
    qpainfosource.cpp
    qtversionsource.cpp
    screeninfosource.cpp
    selectionratiosource.cpp
    startcountsource.cpp
    surveyinfo.cpp
    usagetimesource.cpp
    auditloguicontroller.cpp

    abstractdatasource.h
    applicationversionsource.h
    compilerinfosource.h
    cpuinfosource.h
    feedbackconfiguicontroller.h
    localeinfosource.h
    openglinfosource.h
    platforminfosource.h
    propertyratiosource.h
    provider.h
    qpainfosource.h
    qtversionsource.h
    screeninfosource.h
    selectionratiosource.h
    startcountsource.h
    surveyinfo.h
    usagetimesource.h
    auditloguicontroller.h
    ${userfeedback_core_QM_LOADER}
    ${userfeedback_core_debug_srcs}
)



add_library(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} ${userfeedback_core_srcs})
set_target_properties(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} PROPERTIES
    SOVERSION ${KUSERFEEDBACK_SOVERSION}
    VERSION ${KUSERFEEDBACK_VERSION}
)
generate_export_header(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} BASE_NAME KUserFeedbackCore)

target_link_libraries(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} PUBLIC Qt${QT_MAJOR_VERSION}::Core PRIVATE Qt${QT_MAJOR_VERSION}::Gui Qt${QT_MAJOR_VERSION}::Network)
if(ANDROID)
    target_link_libraries(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} PRIVATE GLESv2)
endif()
target_compile_definitions(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x050300)
target_link_libraries(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} PRIVATE KUserFeedbackCommon)
target_include_directories(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KUserFeedback${KUSERFEEDBACK_EXTENSION};${KDE_INSTALL_INCLUDEDIR}>")

ecm_generate_headers(KUserFeedbackCore_HEADERS
    HEADER_NAMES
        AbstractDataSource
        ApplicationVersionSource
        CompilerInfoSource
        CpuInfoSource
        LocaleInfoSource
        OpenGLInfoSource
        PlatformInfoSource
        PropertyRatioSource
        Provider
        QPAInfoSource
        QtVersionSource
        ScreenInfoSource
        StartCountSource
        SurveyInfo
        UsageTimeSource
        FeedbackConfigUiController
    REQUIRED_HEADERS KUserFeedbackCore_HEADERS
)

install(TARGETS KUserFeedbackCore${KUSERFEEDBACK_EXTENSION} EXPORT KUserFeedback${KUSERFEEDBACK_EXTENSION}Targets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${KUserFeedbackCore_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kuserfeedbackcore_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KUserFeedback${KUSERFEEDBACK_EXTENSION}
)

ecm_generate_pri_file(BASE_NAME KUserFeedbackCore${KUSERFEEDBACK_EXTENSION}
                      LIB_NAME KUserFeedbackCore${KUSERFEEDBACK_EXTENSION}
                      DEPS "core network"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
