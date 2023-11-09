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

configure_file(qmldir.in ${CMAKE_CURRENT_BINARY_DIR}/qmldir)

add_library(KUserFeedbackQml SHARED ${provider_qml_srcs})

target_link_libraries(KUserFeedbackQml
    KUserFeedbackCore
    Qt6::Qml
)

install(TARGETS KUserFeedbackQml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/userfeedback)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/userfeedback)
