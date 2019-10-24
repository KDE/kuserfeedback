configure_file(test-config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/test-config.h)
if(TARGET KUserFeedbackConsole)
add_library(KUserFeedbackTestUtils STATIC
    servercontroller.cpp
)
target_link_libraries(KUserFeedbackTestUtils LINK_PUBLIC Qt5::Test KUserFeedbackConsole)
endif()

function(uf_add_test _file)
    get_filename_component(_name ${_file} NAME_WE)
    add_executable(${_name} ${_file})
    target_link_libraries(${_name} Qt5::Test ${ARGN})
    add_test(NAME ${_name} COMMAND ${_name})
endfunction()

uf_add_test(datasourcetest KUserFeedbackCore Qt5::Gui) # needs Gui for ScreenInfoSource to work
uf_add_test(openglinfosourcetest.cpp KUserFeedbackCore Qt5::Gui)
uf_add_test(providertest KUserFeedbackCore Qt5::Gui)
uf_add_test(selectionratiosourcetest.cpp KUserFeedbackCore Qt5::Gui)
uf_add_test(surveyprovidertest.cpp KUserFeedbackCore)

uf_add_test(feedbackconfigtest KUserFeedbackWidgets)

if(HAVE_SURVEY_TARGET_EXPRESSIONS)
    uf_add_test(surveytargetexpressiontest.cpp KUserFeedbackCommon)
endif()

if(TARGET KUserFeedbackConsole)
    uf_add_test(producttest KUserFeedbackConsole)
    uf_add_test(schematemplatetest.cpp KUserFeedbackConsole)
    uf_add_test(sampletest KUserFeedbackConsole)
    uf_add_test(serverinfotest.cpp KUserFeedbackConsole)

    uf_add_test(schemamodeltest KUserFeedbackTestUtils)
    uf_add_test(datamodeltest KUserFeedbackTestUtils)
    uf_add_test(timeaggregationmodeltest KUserFeedbackTestUtils)
    uf_add_test(categoryaggregationmodeltest KUserFeedbackTestUtils)
    uf_add_test(numericaggregationmodeltest KUserFeedbackTestUtils)
    uf_add_test(ratiosetaggregationmodeltest KUserFeedbackTestUtils)
endif()

if(ENABLE_PHP)
    list(FIND PHP_MODULES "PDO" _has_pdo)
    if(TARGET KUserFeedbackConsole AND _has_pdo GREATER -1)
        uf_add_test(productapitest KUserFeedbackTestUtils)
        uf_add_test(productmodeltest KUserFeedbackTestUtils)
        uf_add_test(surveyapitest KUserFeedbackTestUtils)
        uf_add_test(submittest KUserFeedbackTestUtils KUserFeedbackCore Qt5::Gui)
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
endif()
