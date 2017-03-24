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

    model/aggregateddatamodel.cpp
    model/aggregationeditormodel.cpp
    model/aggregationelementmodel.cpp
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

add_library(UserFeedbackConsole STATIC ${console_lib_srcs})
target_link_libraries(UserFeedbackConsole Qt5::Network)
target_include_directories(UserFeedbackConsole PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_compile_features(UserFeedbackConsole PRIVATE cxx_generic_lambdas)
target_compile_definitions(UserFeedbackConsole PUBLIC QT_DISABLE_DEPRECATED_BEFORE=0x050600 QT_DEPRECATED_WARNINGS)

if(NOT TARGET UserFeedbackWidgets)
    return()
endif()

set(console_srcs
    connectdialog.cpp
    main.cpp
    mainwindow.cpp

    analytics/aggregator.cpp
    analytics/analyticsview.cpp
    analytics/categoryaggregator.cpp
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

add_executable(UserFeedbackConsoleApplication ${console_srcs})
set_target_properties(UserFeedbackConsoleApplication PROPERTIES OUTPUT_NAME UserFeedbackConsole)
target_link_libraries(UserFeedbackConsoleApplication Qt5::Widgets Qt5::Network Qt5::Charts UserFeedbackWidgets UserFeedbackConsole UserFeedbackCommon)

install(TARGETS UserFeedbackConsoleApplication ${INSTALL_TARGETS_DEFAULT_ARGS})
