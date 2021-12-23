set(common_src
    surveytargetexpression.cpp
    surveytargetexpressionevaluator.cpp
    surveytargetexpressionparser.cpp
)

if(HAVE_SURVEY_TARGET_EXPRESSIONS)

if (CMAKE_VERSION VERSION_LESS 3.5.0)
    set(_flex_opts COMPILE_FLAGS --nounistd --header-file=${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionscanner.h)
    set(_bison_opts COMPILE_FLAGS --defines=${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionparser_p.h)
else()
    set(_flex_opts COMPILE_FLAGS --nounistd DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionscanner.h)
    set(_bison_opts DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionparser_p.h)
endif()

flex_target(surveytargetexpressionscanner
    surveytargetexpressionlexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionlexer.cpp
    ${_flex_opts}
)
bison_target(surveytargetexpressionparser
    surveytargetexpressionparser.y
    ${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionparser_p.cpp
    ${_bison_opts}
)
add_flex_bison_dependency(surveytargetexpressionscanner surveytargetexpressionparser)
endif()

add_library(KUserFeedbackCommon STATIC
    ${common_src}
    ${BISON_surveytargetexpressionparser_OUTPUTS}
    ${FLEX_surveytargetexpressionscanner_OUTPUTS}
)
# Needed to link this static lib to shared libs
set_property(TARGET KUserFeedbackCommon PROPERTY POSITION_INDEPENDENT_CODE ON)

if (NOT BUILD_SHARED_LIBS)
    install(TARGETS KUserFeedbackCommon
            EXPORT KUserFeedbackTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
endif()

target_include_directories(KUserFeedbackCommon PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/..>")
target_link_libraries(KUserFeedbackCommon PUBLIC Qt${QT_MAJOR_VERSION}::Core)
