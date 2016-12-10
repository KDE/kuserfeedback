configure_file(test-config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/test-config.h)
add_library(UserFeedbackTestUtils STATIC
    servercontroller.cpp
    ${CMAKE_SOURCE_DIR}/3rdparty/qt/modeltest.cpp
)
target_link_libraries(UserFeedbackTestUtils LINK_PUBLIC Qt5::Test UserFeedbackAnalyzer)

add_executable(productapitest productapitest.cpp)
target_link_libraries(productapitest UserFeedbackTestUtils)
add_test(NAME productapitest COMMAND productapitest)

add_executable(productmodeltest productmodeltest.cpp)
target_link_libraries(productmodeltest UserFeedbackTestUtils)
add_test(NAME productmodeltest COMMAND productmodeltest)

add_executable(schemamodeltest schemamodeltest.cpp)
target_link_libraries(schemamodeltest UserFeedbackTestUtils)
add_test(NAME schemamodeltest COMMAND schemamodeltest)

add_executable(surveyapitest surveyapitest.cpp)
target_link_libraries(surveyapitest UserFeedbackTestUtils)
add_test(NAME surveyapitest COMMAND surveyapitest)

add_executable(datasourcetest datasourcetest.cpp)
target_link_libraries(datasourcetest UserFeedbackCore Qt5::Gui Qt5::Test) # needs Gui for ScreenInfoSource
add_test(NAME datasourcetest COMMAND datasourcetest)

add_executable(providertest providertest.cpp)
target_link_libraries(providertest UserFeedbackTestUtils UserFeedbackCore Qt5::Gui)
add_test(NAME providertest COMMAND providertest)

add_executable(sampletest sampletest.cpp)
target_link_libraries(sampletest UserFeedbackAnalyzer Qt5::Test)
add_test(NAME sampletest COMMAND sampletest)

add_executable(datamodeltest datamodeltest.cpp)
target_link_libraries(datamodeltest  UserFeedbackTestUtils Qt5::Test)
add_test(NAME datamodeltest COMMAND datamodeltest)

set(php_test_srcs
    utilstest.php
    schemaentrytest.php
    schemaentryelementtest.php
    producttest.php
    sampletest.php
    surveytest.php
)

php_lint(${php_test_srcs})
if(PhpUnit_FOUND)
    foreach(_php_test ${php_test_srcs})
        add_test(NAME ${_php_test} COMMAND ${PHPUNIT_EXECUTABLE} ${_php_test} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endforeach()
endif()
