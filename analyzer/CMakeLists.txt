set(analyzer_lib_srcs
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

add_library(UserFeedbackAnalyzer STATIC ${analyzer_lib_srcs})
target_link_libraries(UserFeedbackAnalyzer Qt5::Network)
target_include_directories(UserFeedbackAnalyzer PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};>")
target_compile_features(UserFeedbackAnalyzer PRIVATE cxx_generic_lambdas)

set(analyzer_srcs
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
)

add_executable(UserFeedbackAnalyzerApplication ${analyzer_srcs})
set_target_properties(UserFeedbackAnalyzerApplication PROPERTIES OUTPUT_NAME UserFeedbackAnalyzer)
target_link_libraries(UserFeedbackAnalyzerApplication Qt5::Widgets Qt5::Network Qt5::Charts UserFeedbackWidgets UserFeedbackAnalyzer)

install(TARGETS UserFeedbackAnalyzerApplication ${INSTALL_TARGETS_DEFAULT_ARGS})
