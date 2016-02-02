set(analyzer_srcs
    connectdialog.cpp
    datamodel.cpp
    main.cpp
    mainwindow.cpp
    product.cpp
    productmodel.cpp
    restclient.cpp
    serverinfo.cpp
    survey.cpp
    surveydialog.cpp
    surveymodel.cpp
    timeaggregationmodel.cpp
)

add_executable(UserFeedbackAnalyzer ${analyzer_srcs})
target_link_libraries(UserFeedbackAnalyzer Qt5::Widgets Qt5::Network)

install(TARGETS UserFeedbackAnalyzer ${INSTALL_TARGETS_DEFAULT_ARGS})

