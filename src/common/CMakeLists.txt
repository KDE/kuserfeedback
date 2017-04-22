set(common_src
    surveytargetexpression.cpp
    surveytargetexpressionevaluator.cpp
    surveytargetexpressionparser.cpp
)

if(HAVE_SURVEY_TARGET_EXPRESSIONS)
flex_target(surveytargetexpressionscanner
    surveytargetexpressionlexer.l
    ${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionlexer.cpp
    COMPILE_FLAGS --header-file=${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionscanner.h
)
bison_target(surveytargetexpressionparser
    surveytargetexpressionparser.y
    ${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionparser_p.cpp
    COMPILE_FLAGS --defines=${CMAKE_CURRENT_BINARY_DIR}/surveytargetexpressionparser_p.h
)
add_flex_bison_dependency(surveytargetexpressionscanner surveytargetexpressionparser)
endif()

add_library(UserFeedbackCommon STATIC
    ${common_src}
    ${BISON_surveytargetexpressionparser_OUTPUTS}
    ${FLEX_surveytargetexpressionscanner_OUTPUTS}
)

target_include_directories(UserFeedbackCommon PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR};${CMAKE_CURRENT_SOURCE_DIR}/..>")
if(Qt5Core_FOUND)
    target_link_libraries(UserFeedbackCommon LINK_PRIVATE Qt5::Core)
else()
    target_link_libraries(UserFeedbackCommon LINK_PRIVATE ${QT_QTCORE_LIBS})
    set_target_properties(UserFeedbackCommon PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()
