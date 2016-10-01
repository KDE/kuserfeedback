add_executable(productapitest productapitest.cpp)
target_link_libraries(productapitest Qt5::Test UserFeedbackAnalyzer)
add_test(NAME productapitest COMMAND productapitest)

add_executable(productmodeltest productmodeltest.cpp ${CMAKE_SOURCE_DIR}/3rdparty/qt/modeltest.cpp)
target_link_libraries(productmodeltest Qt5::Test UserFeedbackAnalyzer)
add_test(NAME productmodeltest COMMAND productmodeltest)
