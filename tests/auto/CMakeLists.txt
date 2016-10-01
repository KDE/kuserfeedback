add_executable(productapitest productapitest.cpp)
target_link_libraries(productapitest Qt5::Test UserFeedbackAnalyzer)
add_test(NAME productapitest COMMAND productapitest)
