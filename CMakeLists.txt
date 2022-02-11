cmake_minimum_required(VERSION 3.0)
project(KUserFeedback VERSION 1.2.0)

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
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH})

set(ENABLE_SURVEY_TARGET_EXPRESSIONS ON CACHE BOOL "Survey targeting expressions support is enabled by default.")
set(ENABLE_PHP                       ON CACHE BOOL "Syntax checking of PHP server code.")
set(ENABLE_PHP_UNIT                  ON CACHE BOOL "Unit tests for PHP server code.")
set(ENABLE_DOCS                      ON CACHE BOOL "Generate documentation.")
set(ENABLE_CONSOLE                   ON CACHE BOOL "Application console.")
set(ENABLE_CLI                       ON CACHE BOOL "Command line interface support.")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMGenerateHeaders)
include(ECMPoQmTools)
include(ECMSetupVersion)
include(ECMQueryQmake)
include(ECMGeneratePriFile)
include(FeatureSummary)
include(GenerateExportHeader)

ecm_setup_version(PROJECT
    VARIABLE_PREFIX KUSERFEEDBACK
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kuserfeedback_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KUserFeedbackConfigVersion.cmake"
)
#
# Dependencies
#

find_package(Qt${QT_MAJOR_VERSION} 5.4 NO_MODULE REQUIRED COMPONENTS Core Network)
find_package(Qt${QT_MAJOR_VERSION} NO_MODULE QUIET OPTIONAL_COMPONENTS Widgets Charts Test Qml Svg PrintSupport)
find_package(QmlLint)
if (NOT CMAKE_CROSSCOMPILING)
    find_package(Qt${QT_MAJOR_VERSION} NO_MODULE QUIET OPTIONAL_COMPONENTS Help)
endif()

if (ENABLE_DOCS)
    if(QT_MAJOR_VERSION STREQUAL "5")
        find_package(Qt5 CONFIG OPTIONAL_COMPONENTS DocTools)
    else()
        find_package(Qt6 CONFIG OPTIONAL_COMPONENTS ToolsTools)
    endif()
endif()

set_package_properties(Qt${QT_MAJOR_VERSION} PROPERTIES URL "http://qt-project.org/")
set_package_properties(Qt${QT_MAJOR_VERSION}Widgets PROPERTIES TYPE RECOMMENDED PURPOSE "Required for feedback configuration and notification widgets.")
set_package_properties(Qt${QT_MAJOR_VERSION}Charts PROPERTIES TYPE RECOMMENDED PURPOSE "Required for UserFeedbackConsole.")

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

if (BUILD_TESTING)
    add_subdirectory(autotests)

    add_subdirectory(tests)
endif()

if (ENABLE_DOCS)
    add_subdirectory(docs)
endif()

#
# CMake package config file generation
#
include(CMakePackageConfigHelpers)

if (${QT_MAJOR_VERSION} GREATER_EQUAL 6)
    set(_CONFIG_CMAKE_IN "KUserFeedbackConfig-Qt6.cmake.in")
else()
    set(_CONFIG_CMAKE_IN "KUserFeedbackConfig-Qt5.cmake.in")
endif()

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${_CONFIG_CMAKE_IN}
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

if (ECM_VERSION VERSION_GREATER "5.58.0")
    install(FILES org_kde_UserFeedback.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
else()
    install(FILES org_kde_UserFeedback.categories DESTINATION ${KDE_INSTALL_CONFDIR})
endif()

ecm_install_po_files_as_qm(poqm)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
