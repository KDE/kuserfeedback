configure_file(test-config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/test-config.h)
if(TARGET UserFeedbackConsole)
add_library(UserFeedbackTestUtils STATIC
    servercontroller.cpp
    ${CMAKE_SOURCE_DIR}/3rdparty/qt/modeltest.cpp
)
target_link_libraries(UserFeedbackTestUtils LINK_PUBLIC Qt5::Test UserFeedbackConsole)
endif()

function(uf_add_test _file)
    get_filename_component(_name ${_file} NAME_WE)
    add_executable(${_name} ${_file})
    if(Qt5Core_FOUND)
        target_link_libraries(${_name} Qt5::Test ${ARGN})
    else()
        list(REMOVE_ITEM ARGN Qt5::Gui)
        target_link_libraries(${_name} ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY} ${ARGN})
    endif()
    add_test(NAME ${_name} COMMAND ${_name})
endfunction()

uf_add_test(datasourcetest UserFeedbackCore Qt5::Gui) # needs Gui for ScreenInfoSource to work
uf_add_test(providertest UserFeedbackCore Qt5::Gui)

uf_add_test(feedbackconfigtest UserFeedbackWidgets)

if(HAVE_SURVEY_TARGET_EXPRESSIONS)
    uf_add_test(surveytargetexpressiontest.cpp UserFeedbackCommon)
endif()

if(TARGET UserFeedbackConsole)
    uf_add_test(producttest UserFeedbackConsole)
    uf_add_test(schematemplatetest.cpp UserFeedbackConsole)
    uf_add_test(sampletest UserFeedbackConsole)
    uf_add_test(serverinfotest.cpp UserFeedbackConsole)

    uf_add_test(schemamodeltest UserFeedbackTestUtils)
    uf_add_test(datamodeltest UserFeedbackTestUtils)
    uf_add_test(timeaggregationmodeltest UserFeedbackTestUtils)
    uf_add_test(categoryaggregationmodeltest UserFeedbackTestUtils)
    uf_add_test(numericaggregationmodeltest UserFeedbackTestUtils)
    uf_add_test(ratiosetaggregationmodeltest UserFeedbackTestUtils)

if(Php_FOUND)
    uf_add_test(productapitest UserFeedbackTestUtils)
    uf_add_test(productmodeltest UserFeedbackTestUtils)
    uf_add_test(surveyapitest UserFeedbackTestUtils)
    uf_add_test(submittest UserFeedbackTestUtils UserFeedbackCore Qt5::Gui)
endif()
endif()

set(php_test_srcs
    utilstest.php
    schemaentrytest.php
    schemaentryelementtest.php
    producttest.php
    sampletest.php
    surveytest.php
    aggregationtest.php
)

php_lint(abstractdatastoretest.php)
php_lint(${php_test_srcs})
if(PhpUnit_FOUND)
    foreach(_php_test ${php_test_srcs})
        add_test(NAME ${_php_test} COMMAND ${PHPUNIT_EXECUTABLE} ${_php_test} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endforeach()
endif()
