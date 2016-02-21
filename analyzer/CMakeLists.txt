set(analyzer_srcs
    aggregateddatamodel.cpp
    categoryaggregationmodel.cpp
    chart.cpp
    connectdialog.cpp
    datamodel.cpp
    main.cpp
    mainwindow.cpp
    numericaggregationmodel.cpp
    product.cpp
    productmodel.cpp
    productschemaentry.cpp
    ratioset.cpp
    ratiosetaggregationmodel.cpp
    restclient.cpp
    sample.cpp
    schemamodel.cpp
    schemaeditwidget.cpp
    schemaentryitemeditorfactory.cpp
    schemaentrytypecombobox.cpp
    serverinfo.cpp
    survey.cpp
    surveydialog.cpp
    surveymodel.cpp
    timeaggregationmodel.cpp
)

add_executable(UserFeedbackAnalyzer ${analyzer_srcs})
target_link_libraries(UserFeedbackAnalyzer Qt5::Widgets Qt5::Network Qt5::Charts UserFeedbackWidgets)

install(TARGETS UserFeedbackAnalyzer ${INSTALL_TARGETS_DEFAULT_ARGS})
