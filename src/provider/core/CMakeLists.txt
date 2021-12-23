ecm_create_qm_loader(userfeedback_core_QM_LOADER userfeedbackprovider5_qt)

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
    ${userfeedback_core_QM_LOADER}
)

add_library(KUserFeedbackCore ${userfeedback_core_srcs})
set_target_properties(KUserFeedbackCore PROPERTIES
    SOVERSION ${KUSERFEEDBACK_SOVERSION}
    VERSION ${KUSERFEEDBACK_VERSION}
)
generate_export_header(KUserFeedbackCore)

target_link_libraries(KUserFeedbackCore PUBLIC Qt${QT_MAJOR_VERSION}::Core PRIVATE Qt${QT_MAJOR_VERSION}::Gui Qt${QT_MAJOR_VERSION}::Network)
if(ANDROID)
    target_link_libraries(KUserFeedbackCore PRIVATE GLESv2)
endif()
target_compile_definitions(KUserFeedbackCore PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x050300)
target_link_libraries(KUserFeedbackCore PRIVATE KUserFeedbackCommon)
target_include_directories(KUserFeedbackCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(KUserFeedbackCore INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KUserFeedback;${KDE_INSTALL_INCLUDEDIR}>")

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

install(TARGETS KUserFeedbackCore EXPORT KUserFeedbackTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES
    ${KUserFeedbackCore_HEADERS}
    ${CMAKE_CURRENT_BINARY_DIR}/kuserfeedbackcore_export.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KUserFeedback
)

ecm_generate_pri_file(BASE_NAME KUserFeedbackCore
                      LIB_NAME KUserFeedbackCore
                      DEPS "core network"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
