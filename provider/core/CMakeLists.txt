set(userfeedback_core_srcs
    provider.cpp
    surveyinfo.cpp
)

add_library(UserFeedbackCore SHARED ${userfeedback_core_srcs})
set_target_properties(UserFeedbackCore PROPERTIES
    SOVERSION ${USERFEEDBACK_VERSION}
    VERSION ${USERFEEDBACK_VERSION}
)
generate_export_header(UserFeedbackCore)
target_link_libraries(UserFeedbackCore PUBLIC ${QT_QTCORE_LIBRARIES} PRIVATE ${QT_QTNETWORK_LIBRARIES})
target_include_directories(UserFeedbackCore PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR};>")

install(TARGETS UserFeedbackCore EXPORT UserFeedbackTargets ${INSTALL_TARGETS_DEFAULT_ARGS})


ecm_generate_pri_file(BASE_NAME UserFeedbackCore
                      LIB_NAME UserFeedbackCore
                      DEPS "core network"
                      FILENAME_VAR PRI_FILENAME
)

install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
