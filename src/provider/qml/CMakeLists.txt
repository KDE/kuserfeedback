set(provider_qml_srcs
    qmlabstractdatasource.cpp
    qmldatasources.cpp
    qmlplugin.cpp
    qmlpropertyratiosource.cpp
    qmlproviderextension.cpp
    qmlpropertysource.cpp
)

add_library(KUserFeedbackQml${KUSERFEEDBACK_EXTENSION} SHARED ${provider_qml_srcs})

target_link_libraries(KUserFeedbackQml${KUSERFEEDBACK_EXTENSION}
    KUserFeedbackCore${KUSERFEEDBACK_EXTENSION}
    Qt${QT_MAJOR_VERSION}::Qml
)

install(TARGETS KUserFeedbackQml${KUSERFEEDBACK_EXTENSION} DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/userfeedback)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/userfeedback)
