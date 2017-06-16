set(provider_qml_srcs
    qmlabstractdatasource.cpp
    qmldatasources.cpp
    qmlplugin.cpp
    qmlpropertyratiosource.cpp
    qmlproviderextension.cpp
    qmlpropertysource.cpp
)

add_library(KUserFeedbackQml SHARED ${provider_qml_srcs})

target_link_libraries(KUserFeedbackQml
    KUserFeedbackCore
    Qt5::Qml
)

install(TARGETS KUserFeedbackQml DESTINATION ${QML_INSTALL_DIR}/org/kde/userfeedback)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/userfeedback)
