set(userfeedback_core_srcs
    abstractdatasource.cpp
    applicationversionsource.cpp
    platforminfosource.cpp
    propertyratiosource.cpp
    provider.cpp
    qtversionsource.cpp
    screeninfosource.cpp
    startcountsource.cpp
    surveyinfo.cpp
    usagetimesource.cpp
)

add_library(UserFeedbackCore SHARED ${userfeedback_core_srcs})
set_target_properties(UserFeedbackCore PROPERTIES
    SOVERSION ${USERFEEDBACK_VERSION}
    VERSION ${USERFEEDBACK_VERSION}
)
generate_export_header(UserFeedbackCore)

if(Qt5Core_FOUND)
    target_link_libraries(UserFeedbackCore PUBLIC Qt5::Core PRIVATE Qt5::Gui Qt5::Network)
    target_compile_definitions(UserFeedbackCore PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x040800)
else()
    target_link_libraries(UserFeedbackCore PUBLIC ${QT_QTCORE_LIBRARIES} PRIVATE ${QT_QTGUI_LIBRARIES} ${QT_QTNETWORK_LIBRARIES})
endif()
target_include_directories(UserFeedbackCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src;${CMAKE_CURRENT_BINARY_DIR};>")

install(TARGETS UserFeedbackCore EXPORT UserFeedbackTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

install(FILES
    abstractdatasource.h
    applicationversionsource.h
    platforminfosource.h
    provider.h
    qtversionsource.h
    screeninfosource.h
    startcountsource.h
    surveyinfo.h
    usagetimesource.h
    ${CMAKE_CURRENT_BINARY_DIR}/userfeedbackcore_export.h
    DESTINATION include/UserFeedbackCore
)

ecm_generate_pri_file(BASE_NAME UserFeedbackCore
                      LIB_NAME UserFeedbackCore
                      DEPS "core network"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
