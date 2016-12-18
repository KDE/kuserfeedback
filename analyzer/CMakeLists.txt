set(analyzer_lib_srcs
    core/product.cpp
    core/ratioset.cpp
    core/sample.cpp
    core/schemaentry.cpp
    core/schemaentryelement.cpp
    core/schemaentrytemplates.cpp
    core/survey.cpp

    rest/restapi.cpp
    rest/restclient.cpp
    rest/serverinfo.cpp

    model/categoryaggregationmodel.cpp
    model/datamodel.cpp
    model/productmodel.cpp
    model/schemamodel.cpp
    model/surveymodel.cpp
    model/timeaggregationmodel.cpp

    schemaentrytemplates/schemaentrytemplates.qrc
)

add_library(UserFeedbackAnalyzer STATIC ${analyzer_lib_srcs})
target_link_libraries(UserFeedbackAnalyzer Qt5::Network)
target_include_directories(UserFeedbackAnalyzer PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};>")

set(analyzer_srcs
    aggregateddatamodel.cpp
    chart.cpp
    connectdialog.cpp
    main.cpp
    mainwindow.cpp
    numericaggregationmodel.cpp
    ratiosetaggregationmodel.cpp

    analytics/analyticsview.cpp

    schemaeditor/schemaeditwidget.cpp
    schemaeditor/schemaentryitemeditorfactory.cpp

    surveyeditor/surveydialog.cpp
    surveyeditor/surveyeditor.cpp

    widgets/metaenumcombobox.cpp
)

add_executable(UserFeedbackAnalyzerApplication ${analyzer_srcs})
set_target_properties(UserFeedbackAnalyzerApplication PROPERTIES OUTPUT_NAME UserFeedbackAnalyzer)
target_link_libraries(UserFeedbackAnalyzerApplication Qt5::Widgets Qt5::Network Qt5::Charts UserFeedbackWidgets UserFeedbackAnalyzer)

install(TARGETS UserFeedbackAnalyzerApplication ${INSTALL_TARGETS_DEFAULT_ARGS})
