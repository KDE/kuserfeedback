add_executable(productapitest productapitest.cpp)
target_link_libraries(productapitest Qt5::Test UserFeedbackAnalyzer)
add_test(NAME productapitest COMMAND productapitest)

add_executable(productmodeltest productmodeltest.cpp ${CMAKE_SOURCE_DIR}/3rdparty/qt/modeltest.cpp)
target_link_libraries(productmodeltest Qt5::Test UserFeedbackAnalyzer)
add_test(NAME productmodeltest COMMAND productmodeltest)

add_executable(schemamodeltest schemamodeltest.cpp ${CMAKE_SOURCE_DIR}/3rdparty/qt/modeltest.cpp)
target_link_libraries(schemamodeltest Qt5::Test UserFeedbackAnalyzer)
add_test(NAME schemamodeltest COMMAND schemamodeltest)

add_executable(surveyapitest surveyapitest.cpp ${CMAKE_SOURCE_DIR}/3rdparty/qt/modeltest.cpp)
target_link_libraries(surveyapitest Qt5::Test UserFeedbackAnalyzer)
add_test(NAME surveyapitest COMMAND surveyapitest)

set(php_test_srcs
    utilstest.php
    producttest.php
    surveytest.php
)

php_lint(${php_test_srcs})
if(PhpUnit_FOUND)
    foreach(_php_test ${php_test_srcs})
        add_test(NAME ${_php_test} COMMAND ${PHPUNIT_EXECUTABLE} ${_php_test} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endforeach()
endif()
