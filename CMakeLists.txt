cmake_minimum_required(VERSION 3.0)
project(KUserFeedback VERSION 0.9.84)

set(POLICY CMP0048 NEW)
if(POLICY CMP0053)
    cmake_policy(SET CMP0053 NEW)
endif()
if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()
find_package(ECM 5.26 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH})

set(ENABLE_SURVEY_TARGET_EXPRESSIONS ON CACHE BOOL "Survey targeting expressions support is enabled by default.")
set(ENABLE_PHP                       ON CACHE BOOL "Syntax checking of PHP server code.")
set(ENABLE_PHP_UNIT                  ON CACHE BOOL "Unit tests for PHP server code.")
set(ENABLE_TESTING                   ON CACHE BOOL "Build tests.")
set(ENABLE_DOCS                      ON CACHE BOOL "Generate documentation.")
set(ENABLE_CONSOLE                   ON CACHE BOOL "Application console.")
set(ENABLE_CLI                       ON CACHE BOOL "Command line interface support.")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)

include(ECMGenerateHeaders)
include(ECMPoQmTools)
include(ECMSetupVersion)
include(ECMQueryQmake)
include(ECMGeneratePriFile)
include(FeatureSummary)
include(GenerateExportHeader)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

if (ENABLE_TESTING)
    enable_testing()
endif()

ecm_setup_version(PROJECT
    VARIABLE_PREFIX KUSERFEEDBACK
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kuserfeedback_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KUserFeedbackConfigVersion.cmake"
)
#
# Dependencies
#

find_package(Qt5 5.4 NO_MODULE REQUIRED COMPONENTS Core Network)
find_package(Qt5 NO_MODULE QUIET OPTIONAL_COMPONENTS Widgets Charts Test Qml Svg PrintSupport)
find_package(QmlLint)
if (NOT CMAKE_CROSSCOMPILING)
    find_package(Qt5 NO_MODULE QUIET OPTIONAL_COMPONENTS Help)
endif()

set_package_properties(Qt5 PROPERTIES URL "http://qt-project.org/")
set_package_properties(Qt5Widgets PROPERTIES TYPE RECOMMENDED PURPOSE "Required for feedback configuration and notification widgets.")
set_package_properties(Qt5Charts PROPERTIES TYPE RECOMMENDED PURPOSE "Required for UserFeedbackConsole.")

# debug suffixes for qmake compatibility
if(WIN32)
    set(CMAKE_DEBUG_POSTFIX "d")
elseif(APPLE)
    set(CMAKE_DEBUG_POSTFIX "_debug")
endif()

add_definitions(-DQT_USE_QSTRINGBUILDER -DQT_USE_FAST_OPERATOR_PLUS -DQT_NO_CAST_TO_ASCII -DQT_NO_URL_CAST_FROM_STRING -DQT_NO_CAST_FROM_ASCII)

if (ENABLE_SURVEY_TARGET_EXPRESSIONS)
    find_package(FLEX)
    set_package_properties(FLEX PROPERTIES TYPE RECOMMENDED PURPOSE "Survey target expression parser.")

    find_package(BISON)
    set_package_properties(BISON PROPERTIES TYPE RECOMMENDED PURPOSE "Survey target expression parser.")

    if(FLEX_FOUND AND BISON_FOUND)
        set(HAVE_SURVEY_TARGET_EXPRESSIONS 1)
    endif()
endif()
add_feature_info("Survey targeting expressions support" HAVE_SURVEY_TARGET_EXPRESSIONS "Requires flex and bison parser generators.")

if (ENABLE_PHP)
    find_package(Php)
    set_package_properties(Php PROPERTIES URL "http://php.net" TYPE RECOMMENDED PURPOSE "Syntax checking of PHP server code.")
endif()

if (ENABLE_PHP_UNIT)
    find_package(PhpUnit)
    set_package_properties(PhpUnit PROPERTIES URL "http://phpunit.de" TYPE RECOMMENDED PURPOSE "Unit tests for PHP server code.")
endif()

#
# Actually build the stuff
#
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
configure_file(config-userfeedback.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-userfeedback.h)

add_subdirectory(src)

if (ENABLE_TESTING)
    if (BUILD_TESTING)
        add_subdirectory(autotests)
    endif()

    add_subdirectory(tests)
endif()

if (ENABLE_DOCS)
    add_subdirectory(docs)
endif()

#
# CMake package config file generation
#
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/KUserFeedbackConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/KUserFeedbackConfig.cmake
    INSTALL_DESTINATION ${KDE_INSTALL_CMAKEPACKAGEDIR}/KUserFeedback
    PATH_VARS KDE_INSTALL_INCLUDEDIR
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/KUserFeedbackConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/KUserFeedbackConfigVersion.cmake
    DESTINATION ${KDE_INSTALL_CMAKEPACKAGEDIR}/KUserFeedback
)

install(EXPORT KUserFeedbackTargets DESTINATION ${KDE_INSTALL_CMAKEPACKAGEDIR}/KUserFeedback FILE KUserFeedbackTarget.cmake)
install(FILES org_kde_UserFeedback.categories DESTINATION ${KDE_INSTALL_CONFDIR})

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
