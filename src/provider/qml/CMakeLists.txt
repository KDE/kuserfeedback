set(provider_qml_srcs
    qmlabstractdatasource.cpp
    qmldatasources.cpp
    qmlplugin.cpp
    qmlpropertyratiosource.cpp
    qmlproviderextension.cpp
    qmlpropertysource.cpp

    qmlabstractdatasource.h
    qmldatasources.h
    qmlplugin.h
    qmlpropertyratiosource.h
    qmlproviderextension.h
    qmlpropertysource.h
)

add_library(KUserFeedbackQml SHARED ${provider_qml_srcs})

target_link_libraries(KUserFeedbackQml
    KUserFeedbackCore${KUSERFEEDBACK_EXTENSION}
    Qt${QT_MAJOR_VERSION}::Qml
)

install(TARGETS KUserFeedbackQml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/userfeedback)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/userfeedback)
