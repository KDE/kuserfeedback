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

set(php_test_srcs
    utilstest.php
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
