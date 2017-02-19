project(UserFeedback)

cmake_minimum_required(VERSION 2.8.12)
if(POLICY CMP0063)
    cmake_policy(SET CMP0063 NEW)
endif()
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
if(NOT DEFINED CMAKE_INSTALL_RPATH_USE_LINK_PATH)
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()
if(NOT DEFINED CMAKE_MACOSX_RPATH)
    set(CMAKE_MACOSX_RPATH TRUE)
endif()

include(ECMGeneratePriFile)
include(ECMEnableSanitizers)
include(FeatureSummary)
include(GenerateExportHeader)

enable_testing()

set(USERFEEDBACK_VERSION_MAJOR "1")
set(USERFEEDBACK_VERSION_MINOR "9")
set(USERFEEDBACK_VERSION_PATCH "84")
set(USERFEEDBACK_VERSION "${USERFEEDBACK_VERSION_MAJOR}.${USERFEEDBACK_VERSION_MINOR}.${USERFEEDBACK_VERSION_PATCH}")
set(USERFEEDBACK_VERSION_STRING "${USERFEEDBACK_VERSION}")
set(PROJECT_VERSION_STRING ${USERFEEDBACK_VERSION}) # for the ECM pri generator
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  find_package(Git)
  set_package_properties(Git PROPERTIES TYPE OPTIONAL PURPOSE "Determine exact build version.")
  if(GIT_FOUND)
    execute_process(
      COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE _git_revision
    )
    string(REGEX REPLACE "\n" "" _git_revision "${_git_revision}")
    set(USERFEEDBACK_VERSION_STRING "${USERFEEDBACK_VERSION_STRING} (revision: ${_git_revision})")
  endif()
endif()

#
# Compiler & linker settings
#
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

if(CMAKE_COMPILER_IS_GNUCXX OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -Wextra -Wall -Werror=return-type -std=c++0x -pedantic")
endif()

if(NOT ECM_ENABLE_SANITIZERS AND (CMAKE_SYSTEM_NAME MATCHES Linux OR CMAKE_SYSTEM_NAME STREQUAL GNU))
    if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined -lc ${CMAKE_SHARED_LINKER_FLAGS}")
        set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--fatal-warnings -Wl,--no-undefined -lc ${CMAKE_MODULE_LINKER_FLAGS}")
    endif()
endif()

#
# Dependencies
#

# try Qt5 first, and prefer that (if found), but only if not disabled via option
if(NOT ENFORCE_QT4_BUILD)
    find_package(Qt5Core QUIET NO_MODULE)
else()
    set(Qt5Core_FOUND FALSE)
endif()

if(Qt5Core_FOUND)
    set_package_properties(Qt5Core PROPERTIES TYPE REQUIRED)
    find_package(Qt5 NO_MODULE REQUIRED COMPONENTS Network)
    find_package(Qt5 NO_MODULE QUIET OPTIONAL_COMPONENTS Widgets Charts Test)
    find_package(KF5WidgetsAddons NO_MODULE QUIET)

    if(Qt5_POSITION_INDEPENDENT_CODE AND NOT WIN32)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    endif()

    include("cmake/ECMQt4To5Porting.cmake")

    add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x040800)
    add_definitions(-DQT_DEPRECATED_WARNINGS)

    set_package_properties(Qt5 PROPERTIES URL "http://qt-project.org/")
    set_package_properties(Qt5Widgets PROPERTIES TYPE RECOMMENDED PURPOSE "Required for analyzer and administraion tool.")
    set_package_properties(Qt5Charts PROPERTIES TYPE RECOMMENDED PURPOSE "Required for analyzer and administraion tool.")
    set_package_properties(KF5WidgetsAddons PROPERTIES TYPE RECOMMENDED PURPOSE "Required for analyzer and administraion tool.")
# Qt4
else()
    set(QT_USE_IMPORTED_TARGETS true)
    find_package(Qt4 4.8.0 REQUIRED QtCore QtNetwork QtGui)
    include(${QT_USE_FILE})
    set_package_properties(Qt4 PROPERTIES URL "http://qt-project.org/")

    # C++11/Qt5 compatibility
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FI\"${CMAKE_SOURCE_DIR}\\src\\compat\\qt4compat.h\"")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include \"${CMAKE_SOURCE_DIR}/src/compat/qt4compat.h\"")
    endif()
endif()

# debug suffixes for qmake compatibility
if(WIN32)
    set(CMAKE_DEBUG_POSTFIX "d")
elseif(APPLE)
    set(CMAKE_DEBUG_POSTFIX "_debug")
endif()

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_NO_CAST_TO_ASCII -DQT_NO_URL_CAST_FROM_STRING -DQT_NO_CAST_FROM_ASCII)

find_package(Php)
set_package_properties(Php PROPERTIES URL "http://php.net" TYPE RECOMMENDED PURPOSE "Syntax checking of PHP server code.")
find_package(PhpUnit)
set_package_properties(PhpUnit PROPERTIES URL "http://phpunit.de" TYPE RECOMMENDED PURPOSE "Unit tests for PHP server code.")

#
# Installation settings
#
set(BIN_INSTALL_DIR "bin")
set(LIB_SUFFIX "" CACHE STRING "Define suffix of directory name (32/64)")
set(LIB_INSTALL_DIR "lib${LIB_SUFFIX}")
set(INCLUDE_INSTALL_DIR "include/userfeedback")
set(CMAKECONFIG_INSTALL_DIR ${LIB_INSTALL_DIR}/cmake/UserFeedback)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${BIN_INSTALL_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${LIB_INSTALL_DIR})

# set RPATH only when installing to a non-default location and not set in cache
if(NOT CMAKE_INSTALL_RPATH)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}" _isSystemPlatformLibDir)
    list(FIND CMAKE_C_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}" _isSystemCLibDir)
    list(FIND CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}" _isSystemCxxLibDir)
    if(${_isSystemPlatformLibDir} EQUAL -1 AND ${_isSystemCLibDir} EQUAL -1 AND ${_isSystemCxxLibDir} EQUAL -1)
        set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
    endif()
endif()

set(
    INSTALL_TARGETS_DEFAULT_ARGS
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR} COMPONENT Devel
    BUNDLE DESTINATION ${BUNDLE_INSTALL_DIR}
)

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR})
configure_file(config-userfeedback-version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config-userfeedback-version.h)

#
# Actually build the stuff
#
add_subdirectory(src)
if(Qt5Charts_FOUND AND NOT CMAKE_VERSION VERSION_LESS 3.0)
    add_subdirectory(tests/manual)
endif()
if(Qt5Core_FOUND)
    add_subdirectory(tests/auto)
endif()

#
# CMake package config file generation
#
include(CMakePackageConfigHelpers)
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/UserFeedbackConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/UserFeedbackConfig.cmake
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS INCLUDE_INSTALL_DIR
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/UserFeedbackConfigVersion.cmake
    VERSION ${USERFEEDBACK_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/UserFeedbackConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/UserFeedbackConfigVersion.cmake
    DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(EXPORT UserFeedbackTargets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE UserFeedbackTarget.cmake)


feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
