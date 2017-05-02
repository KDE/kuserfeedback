query_qmake(QT_INSTALL_DOCS "QT_INSTALL_DOCS")
query_qmake(QT_INSTALL_DATA "QT_INSTALL_DATA")
query_qmake(QT_INSTALL_BINS "QT_INSTALL_BINS")

find_program(QDOC_EXECUTABLE qdoc HINTS ${QT_INSTALL_BINS})

find_file(QDOC_TEMPLATE global/qt-html-templates-offline.qdocconf
  HINTS ${QT_INSTALL_DOCS}
  HINTS ${QT_INSTALL_DATA}
  HINTS ${QT_INSTALL_DATA}/doc
)
get_filename_component(QDOC_TEMPLATE_DIR ${QDOC_TEMPLATE} DIRECTORY)

if(NOT QDOC_EXECUTABLE OR NOT TARGET Qt5::qhelpgenerator OR NOT TARGET Qt5::qcollectiongenerator OR NOT QDOC_TEMPLATE_DIR)
  message(STATUS "Unable to build user manual:")
  message(STATUS "qdoc exectuable: ${QDOC_EXECUTABLE}")
  if(TARGET Qt5::qhelpgenerator)
    get_property(_path TARGET Qt5::qhelpgenerator PROPERTY LOCATION)
    message(STATUS "qhelpgenerator exectuable: ${_path}")
  else()
    message(STATUS "qhelpgenerator not found")
  endif()
  if(TARGET Qt5::qcollectiongenerator)
    get_property(_path TARGET Qt5::qcollectiongenerator PROPERTY LOCATION)
    message(STATUS "qcollectiongenerator exectuable: ${_path}")
  else()
    message(STATUS "qcollectiongenerator not found")
  endif()
  message(STATUS "qdoc template direectory: ${QDOC_TEMPLATE_DIR}")
  return()
endif()

# build .qch
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/user-feedback-manual.qdocconf.in ${CMAKE_CURRENT_BINARY_DIR}/user-feedback-manual.qdocconf)
file(GLOB_RECURSE _qdoc_srcs ${CMAKE_CURRENT_SOURCE_DIR} "*.qdoc")
set(_qdoc_output_dir ${CMAKE_CURRENT_BINARY_DIR}/manual)
add_custom_command(
    OUTPUT ${_qdoc_output_dir}/user-feedback-manual.qhp
    COMMAND ${QDOC_EXECUTABLE}
        --outputdir ${_qdoc_output_dir}
        ${CMAKE_CURRENT_BINARY_DIR}/user-feedback-manual.qdocconf
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/user-feedback-manual.qdocconf ${_qdoc_srcs}
)
add_custom_command(
    OUTPUT ${_qdoc_output_dir}/user-feedback-manual.qch
    COMMAND Qt5::qhelpgenerator ${_qdoc_output_dir}/user-feedback-manual.qhp
    DEPENDS ${_qdoc_output_dir}/user-feedback-manual.qhp
)
add_custom_target(user_feedback_manual_qch ALL DEPENDS ${_qdoc_output_dir}/user-feedback-manual.qch)
install(FILES ${_qdoc_output_dir}/user-feedback-manual.qch DESTINATION ${KDE_INSTALL_DATAROOTDIR}/KDE/UserFeedbackConsole)

# build .qhc
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/user-feedback.qhcp.in ${CMAKE_CURRENT_BINARY_DIR}/user-feedback.qhcp)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/user-feedback.qhc
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/manual/user-feedback-manual.qch ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND Qt5::qcollectiongenerator ${CMAKE_CURRENT_BINARY_DIR}/user-feedback.qhcp -o ${CMAKE_CURRENT_BINARY_DIR}/user-feedback.qhc
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/user-feedback.qhcp
        user_feedback_manual_qch
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_custom_target(user_feedback_qhc ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/user-feedback.qhc)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/user-feedback.qhc DESTINATION ${KDE_INSTALL_DATAROOTDIR}/KDE/UserFeedbackConsole)
