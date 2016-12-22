set(userfeedback_core_srcs
    abstractdatasource.cpp
    applicationversionsource.cpp
    platforminfosource.cpp
    provider.cpp
    qtversionsource.cpp
    screeninfosource.cpp
    surveyinfo.cpp
)

if(Qt5Core_FOUND)
    list(APPEND userfeedback_core_srcs propertyratiosource.cpp)
endif()

add_library(UserFeedbackCore SHARED ${userfeedback_core_srcs})
set_target_properties(UserFeedbackCore PROPERTIES
    SOVERSION ${USERFEEDBACK_VERSION}
    VERSION ${USERFEEDBACK_VERSION}
)
generate_export_header(UserFeedbackCore)

if(Qt5Core_FOUND)
    target_link_libraries(UserFeedbackCore PUBLIC Qt5::Core PRIVATE Qt5::Gui Qt5::Network)
else()
    target_link_libraries(UserFeedbackCore PUBLIC ${QT_QTCORE_LIBRARIES} PRIVATE ${QT_QTGUI_LIBRARIES} ${QT_QTNETWORK_LIBRARIES})
endif()
target_include_directories(UserFeedbackCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR};>")

install(TARGETS UserFeedbackCore EXPORT UserFeedbackTargets ${INSTALL_TARGETS_DEFAULT_ARGS})


ecm_generate_pri_file(BASE_NAME UserFeedbackCore
                      LIB_NAME UserFeedbackCore
                      DEPS "core network"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
