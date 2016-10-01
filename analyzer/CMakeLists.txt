set(analyzer_lib_srcs
    core/product.cpp
    core/schemaentry.cpp

    rest/restapi.cpp
    rest/restclient.cpp
    rest/serverinfo.cpp

    model/productmodel.cpp
)

add_library(UserFeedbackAnalyzer STATIC ${analyzer_lib_srcs})
target_link_libraries(UserFeedbackAnalyzer Qt5::Network)
target_include_directories(UserFeedbackAnalyzer PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};>")

set(analyzer_srcs
    aggregateddatamodel.cpp
    categoryaggregationmodel.cpp
    chart.cpp
    connectdialog.cpp
    datamodel.cpp
    main.cpp
    mainwindow.cpp
    numericaggregationmodel.cpp
    ratioset.cpp
    ratiosetaggregationmodel.cpp
    sample.cpp
    schemamodel.cpp
    schemaeditwidget.cpp
    schemaentryitemeditorfactory.cpp
    schemaentrytypecombobox.cpp
    survey.cpp
    surveydialog.cpp
    surveymodel.cpp
    timeaggregationmodel.cpp
)

add_executable(UserFeedbackAnalyzerApplication ${analyzer_srcs})
set_target_properties(UserFeedbackAnalyzerApplication PROPERTIES OUTPUT_NAME UserFeedbackAnalyzer)
target_link_libraries(UserFeedbackAnalyzerApplication Qt5::Widgets Qt5::Network Qt5::Charts UserFeedbackWidgets UserFeedbackAnalyzer)

install(TARGETS UserFeedbackAnalyzerApplication ${INSTALL_TARGETS_DEFAULT_ARGS})
