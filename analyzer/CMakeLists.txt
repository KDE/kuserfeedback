set(analyzer_srcs
    chart.cpp
    connectdialog.cpp
    datamodel.cpp
    main.cpp
    mainwindow.cpp
    product.cpp
    productmodel.cpp
    restclient.cpp
    sample.cpp
    serverinfo.cpp
    survey.cpp
    surveydialog.cpp
    surveymodel.cpp
    timeaggregationmodel.cpp
)

add_executable(UserFeedbackAnalyzer ${analyzer_srcs})
target_link_libraries(UserFeedbackAnalyzer Qt5::Widgets Qt5::Network Qt5::Charts)

install(TARGETS UserFeedbackAnalyzer ${INSTALL_TARGETS_DEFAULT_ARGS})

