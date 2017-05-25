set(provider_qml_srcs
    qmlplugin.cpp
)

add_library(UserFeedbackQml SHARED ${provider_qml_srcs})

target_link_libraries(UserFeedbackQml
    UserFeedbackCore
    Qt5::Qml
)

install(TARGETS UserFeedbackQml DESTINATION ${QML_INSTALL_DIR}/org/kde/userfeedback)
install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/userfeedback)
