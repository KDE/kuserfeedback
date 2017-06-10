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
    qtversionsource.cpp
    screeninfosource.cpp
    selectionratiosource.cpp
    startcountsource.cpp
    surveyinfo.cpp
    usagetimesource.cpp
)
if(Qt5Core_FOUND)
    list(APPEND userfeedback_core_srcs ${userfeedback_core_QM_LOADER})
endif()

add_library(KUserFeedbackCore SHARED ${userfeedback_core_srcs})
set_target_properties(KUserFeedbackCore PROPERTIES
    SOVERSION ${USERFEEDBACK_VERSION}
    VERSION ${USERFEEDBACK_VERSION}
)
generate_export_header(KUserFeedbackCore)

if(Qt5Core_FOUND)
    target_link_libraries(KUserFeedbackCore PUBLIC Qt5::Core PRIVATE Qt5::Gui Qt5::Network)
    target_compile_definitions(KUserFeedbackCore PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x040800)
else()
    target_link_libraries(KUserFeedbackCore PUBLIC ${QT_QTCORE_LIBRARIES} PRIVATE ${QT_QTGUI_LIBRARIES} ${QT_QTNETWORK_LIBRARIES})
endif()
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
        QtVersionSource
        ScreenInfoSource
        StartCountSource
        SurveyInfo
        UsageTimeSource
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
