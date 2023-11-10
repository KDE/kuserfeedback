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

add_library(KF6UserFeedbackQml SHARED ${provider_qml_srcs})
add_library(KF6::UserFeedbackQml ALIAS KF6UserFeedbackQml)

target_link_libraries(KF6UserFeedbackQml
    KF6UserFeedbackCore
    Qt6::Qml
)

install(TARGETS KF6UserFeedbackQml DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/userfeedback)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/kde/userfeedback)
