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

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(ECMGenerateHeaders)
include(ECMPoQmTools)
include(ECMSetupVersion)
include(FeatureSummary)
include(GenerateExportHeader)
include(KDECompilerSettings)

enable_testing()

ecm_setup_version(PROJECT
    VARIABLE_PREFIX KUSERFEEDBACK
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kuserfeedback_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KUserFeedbackConfigVersion.cmake"
)

#
# Compiler & linker settings
#
if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall -std=c++0x -pedantic")
endif()


#
# Dependencies
#

# try Qt5 first, and prefer that (if found), but only if not disabled via option
if(NOT ENFORCE_QT4_BUILD)
    find_package(Qt5Core QUIET NO_MODULE)
    find_package(QmlLint)
     # do not include in the Qt4 path, these calls find_package(Qt5)
    include(ECMQueryQmake)
    include(ECMGeneratePriFile)
    include(KDEInstallDirs)
    include(KDECMakeSettings)
else()
    set(Qt5Core_FOUND FALSE)
endif()

if(Qt5Core_FOUND)
    set_package_properties(Qt5Core PROPERTIES TYPE REQUIRED)
    find_package(Qt5 NO_MODULE REQUIRED COMPONENTS Network)
    find_package(Qt5 NO_MODULE QUIET OPTIONAL_COMPONENTS Widgets Charts Test Qml Svg PrintSupport)
    if (NOT CMAKE_CROSSCOMPILING)
        find_package(Qt5 NO_MODULE QUIET OPTIONAL_COMPONENTS Help)
    endif()

    set_package_properties(Qt5 PROPERTIES URL "http://qt-project.org/")
    set_package_properties(Qt5Widgets PROPERTIES TYPE RECOMMENDED PURPOSE "Required for feedback configuration and notification widgets.")
    set_package_properties(Qt5Charts PROPERTIES TYPE RECOMMENDED PURPOSE "Required for UserFeedbackConsole.")
# Qt4
else()
    set(QT_USE_IMPORTED_TARGETS true)
    find_package(Qt4 4.8.0 REQUIRED QtCore QtNetwork QtGui QtTest)
    include(${QT_USE_FILE})
    set_package_properties(Qt4 PROPERTIES URL "http://qt-project.org/")

    # C++11/Qt5 compatibility
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI\"${CMAKE_SOURCE_DIR}\\src\\compat\\qt4compat.h\"")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include \"${CMAKE_SOURCE_DIR}/src/compat/qt4compat.h\"")
    endif()
    include(Qt4ECMStub)

    # Qt4 moc limitation workaround (Qt4 moc cannot evaluate QT_VERSION preprocessor conditionals
    add_definitions(-DQT4_MOC_WORKAROUND)
endif()

# debug suffixes for qmake compatibility
if(WIN32)
    set(CMAKE_DEBUG_POSTFIX "d")
elseif(APPLE)
    set(CMAKE_DEBUG_POSTFIX "_debug")
endif()

add_definitions(-DQT_USE_QSTRINGBUILDER -DQT_USE_FAST_OPERATOR_PLUS -DQT_NO_CAST_TO_ASCII -DQT_NO_URL_CAST_FROM_STRING -DQT_NO_CAST_FROM_ASCII)

find_package(FLEX)
set_package_properties(FLEX PROPERTIES TYPE RECOMMENDED PURPOSE "Survey target expression parser.")
find_package(BISON)
set_package_properties(BISON PROPERTIES TYPE RECOMMENDED PURPOSE "Survey target expression parser.")
if(FLEX_FOUND AND BISON_FOUND)
    set(HAVE_SURVEY_TARGET_EXPRESSIONS 1)
endif()
add_feature_info("Survey targeting expressions support" HAVE_SURVEY_TARGET_EXPRESSIONS "Requires flex and bison parser generators.")

find_package(Php)
set_package_properties(Php PROPERTIES URL "http://php.net" TYPE RECOMMENDED PURPOSE "Syntax checking of PHP server code.")
find_package(PhpUnit)
set_package_properties(PhpUnit PROPERTIES URL "http://phpunit.de" TYPE RECOMMENDED PURPOSE "Unit tests for PHP server code.")

#
# Actually build the stuff
#
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
configure_file(config-userfeedback.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-userfeedback.h)

add_subdirectory(src)
if (BUILD_TESTING)
    add_subdirectory(autotests)
endif()
if(Qt5Core_FOUND)
    add_subdirectory(tests)
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
