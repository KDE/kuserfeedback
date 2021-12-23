ecm_create_qm_loader(userfeedback_console_QM_LOADER userfeedbackconsole5_qt)

set(console_lib_srcs
    core/aggregation.cpp
    core/aggregationelement.cpp
    core/product.cpp
    core/sample.cpp
    core/schemaentry.cpp
    core/schemaentryelement.cpp
    core/schemaentrytemplates.cpp
    core/survey.cpp

    rest/restapi.cpp
    rest/restclient.cpp
    rest/serverinfo.cpp

    jobs/job.cpp
    jobs/handshakejob.cpp
    jobs/productexportjob.cpp
    jobs/productimportjob.cpp
    jobs/securityscanjob.cpp

    model/aggregateddatamodel.cpp
    model/aggregationeditormodel.cpp
    model/aggregationelementmodel.cpp
    model/aggregationelementeditmodel.cpp
    model/categoryaggregationmodel.cpp
    model/datamodel.cpp
    model/extrarowsproxymodel.cpp
    model/numericaggregationmodel.cpp
    model/productmodel.cpp
    model/ratiosetaggregationmodel.cpp
    model/rolemappingproxymodel.cpp
    model/schemamodel.cpp
    model/singlerowfilterproxymodel.cpp
    model/surveymodel.cpp
    model/timeaggregationmodel.cpp

    schematemplates/schematemplates.qrc
)

add_library(KUserFeedbackConsole STATIC ${console_lib_srcs})
target_link_libraries(KUserFeedbackConsole Qt${QT_MAJOR_VERSION}::Network)
target_include_directories(KUserFeedbackConsole PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_compile_features(KUserFeedbackConsole PRIVATE cxx_generic_lambdas)
target_compile_definitions(KUserFeedbackConsole PUBLIC QT_DISABLE_DEPRECATED_BEFORE=0x050600 QT_DEPRECATED_WARNINGS)

if(NOT TARGET KUserFeedbackWidgets)
    return()
endif()

set(console_srcs
    connectdialog.cpp
    main.cpp
    mainwindow.cpp
    helpcontroller.cpp

    analytics/aggregator.cpp
    analytics/analyticsview.cpp
    analytics/categoryaggregator.cpp
    analytics/chartexportdialog.cpp
    analytics/chartutil.cpp
    analytics/numericaggregator.cpp
    analytics/ratiosetaggregator.cpp
    analytics/totalaggregator.cpp

    schemaeditor/aggregationeditwidget.cpp
    schemaeditor/schemaeditor.cpp
    schemaeditor/schemaeditwidget.cpp
    schemaeditor/schemaentryitemeditorfactory.cpp

    surveyeditor/surveydialog.cpp
    surveyeditor/surveyeditor.cpp

    widgets/metaenumcombobox.cpp

    ${userfeedback_console_QM_LOADER}
)

add_executable(KUserFeedbackConsoleApplication ${console_srcs})
target_compile_features(KUserFeedbackConsoleApplication PRIVATE cxx_generic_lambdas)
set_target_properties(KUserFeedbackConsoleApplication PROPERTIES OUTPUT_NAME UserFeedbackConsole)
target_link_libraries(KUserFeedbackConsoleApplication
    Qt${QT_MAJOR_VERSION}::Widgets
    Qt${QT_MAJOR_VERSION}::Network
    Qt${QT_MAJOR_VERSION}::Charts
    Qt${QT_MAJOR_VERSION}::PrintSupport
    Qt${QT_MAJOR_VERSION}::Svg
    KUserFeedbackWidgets
    KUserFeedbackConsole
    KUserFeedbackCommon
)

install(TARGETS KUserFeedbackConsoleApplication ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
if(UNIX AND NOT APPLE)
    install(FILES org.kde.kuserfeedback-console.desktop DESTINATION ${KDE_INSTALL_APPDIR})
    install(FILES org.kde.kuserfeedback-console.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
endif()
