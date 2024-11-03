# validate a list of qml files
function(qml_lint)
  foreach(_file ${ARGN})
    get_filename_component(_file_abs ${_file} ABSOLUTE)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}.qmllint
      COMMAND Qt6::qmllint ${_file_abs}
      COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/${_file}.qmllint
      MAIN_DEPENDENCY ${_file_abs}
    )
    add_custom_target(${_file}_qmllint ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${_file}.qmllint)
  endforeach()
endfunction()

if(TARGET KF6UserFeedbackWidgets AND NOT (CMAKE_SOURCE_DIR EQUAL CMAKE_CURRENT_SOURCE_DIR))
    add_executable(feedbackconfigwidgettest feedbackconfigwidgettest.cpp)
    target_link_libraries(feedbackconfigwidgettest KF6UserFeedbackWidgets)

    add_executable(notificationpopuptest notificationpopuptest.cpp)
    target_link_libraries(notificationpopuptest KF6UserFeedbackWidgets)

    add_executable(orwell orwell.cpp orwell.h)
    target_link_libraries(orwell KF6UserFeedbackWidgets)

    qml_lint(orwell.qml)
else()
    # stand-alone build for testing includes and cmake config
    message(STATUS "building test application stand-alone")
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    find_package(KF6UserFeedback REQUIRED)
    add_executable(orwell orwell.cpp)
    target_link_libraries(orwell KF6UserFeedbackWidgets)
endif()
