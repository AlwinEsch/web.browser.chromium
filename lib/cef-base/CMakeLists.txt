cmake_minimum_required(VERSION 3.5)
project(cef-lib-creator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

option(CEF_NO_ARCHIVE "Prevent archive creation?" ON)

include(CheckSymbolExists)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "32")
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CEF_IS_64BIT TRUE)
  set(CEF_BIT_STR "64")
else()
  message(FATAL_ERROR "Unknown current CPU bit size: ${CMAKE_SIZEOF_VOID_P}")
endif()

set(CEF_IS_ARM FALSE)
set(CEF_IS_ARM64 FALSE)

if(NOT CORE_SYSTEM_NAME)
  if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CORE_SYSTEM_NAME "osx")
  else()
    string(TOLOWER ${CMAKE_SYSTEM_NAME} CORE_SYSTEM_NAME)
  endif()
endif()

if(CEF_IS_64BIT)
  set(CEF_BUILD_TYPE "--x64-build")
  set(CEF_BUILD_CPU "x64")
elseif(CEF_IS_ARM)
  set(CEF_BUILD_TYPE "--arm-build")
  set(CEF_BUILD_CPU "arm")
elseif(CEF_IS_ARM64)
  set(CEF_BUILD_TYPE "--arm64-build")
  set(CEF_BUILD_CPU "arm64")
else()
  set(CEF_BUILD_CPU "x86")
endif()

if(CORE_SYSTEM_NAME STREQUAL "linux")
  check_symbol_exists(major "sys/mkdev.h" MAJOR_IN_MKDEV)
  if(NOT MAJOR_IN_MKDEV)
    set(MAJOR_IN_MKDEV false)
  endif()
  check_symbol_exists(major "sys/sysmacros.h" MAJOR_IN_SYSMACROS)
endif()

set(USED_CEF_BRANCH 3729-kodi)

set(TARGET_BUILD_STEPS
#             cef_load
#             cef_configuration
#             cef_creation_debug
#             cef_creation_release
#             cef_package
#             cef_final
)

set(ENV{PATH} "${PROJECT_BINARY_DIR}/depot_tools:$ENV{PATH}")
set(GN_DEFINES "${GN_DEFINES} use_jumbo_build=true")
set(GN_DEFINES "${GN_DEFINES} use_mkdev=${MAJOR_IN_MKDEV}")
set(GN_DEFINES "${GN_DEFINES} use_cups=false")
set(GN_DEFINES "${GN_DEFINES} treat_warnings_as_errors=false")
set(GN_DEFINES "${GN_DEFINES} enable_widevine=true")
set(GN_DEFINES "${GN_DEFINES} use_alsa=false")
set(GN_DEFINES "${GN_DEFINES} use_pulseaudio=false")
set(GN_DEFINES "${GN_DEFINES} rtc_use_h264=true")
set(GN_DEFINES "${GN_DEFINES} use_libpci=true")
set(GN_DEFINES "${GN_DEFINES} is_official_build=true")
set(GN_DEFINES "${GN_DEFINES} use_allocator=none")
set(GN_DEFINES "${GN_DEFINES} proprietary_codecs=true")
set(GN_DEFINES "${GN_DEFINES} ffmpeg_branding=Chrome")

add_custom_command(OUTPUT cef_load
                   COMMAND python ${CMAKE_CURRENT_LIST_DIR}/automate-git.py
                              --download-dir=${PROJECT_BINARY_DIR}/chromium_git
                              --depot-tools-dir=${PROJECT_BINARY_DIR}/depot_tools
                              --no-distrib
                              --no-build
#                               --force-clean
                              --branch=${USED_CEF_BRANCH}
                   COMMENT "Download CEF chromium"
                   VERBATIM)

add_custom_command(OUTPUT cef_configuration
                   COMMAND ${CMAKE_COMMAND} -E env "GN_DEFINES=${GN_DEFINES}" "PATH=$ENV{PATH}" python tools/gclient_hook.py
                   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/chromium_git/chromium/src/cef
                   COMMENT "Creating CEF chromium"
                   VERBATIM)

add_custom_command(OUTPUT cef_creation_debug
                   COMMAND PATH=$ENV{PATH} ninja -C out/Debug_GN_${CEF_BUILD_CPU} cef chrome_sandbox
                   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/chromium_git/chromium/src
                   COMMENT "Creating debug CEF chromium"
                   VERBATIM)

add_custom_command(OUTPUT cef_creation_release
                   COMMAND PATH=$ENV{PATH} ninja -C out/Release_GN_${CEF_BUILD_CPU} cef chrome_sandbox
                   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/chromium_git/chromium/src
                   COMMENT "Creating release CEF chromium"
                   VERBATIM)

add_custom_command(OUTPUT cef_package
                   COMMAND PATH=$ENV{PATH} python make_distrib.py --ninja-build --output-dir ${PROJECT_BINARY_DIR}/chromium_package --package $<$<BOOL:${CEF_NO_ARCHIVE}>:--no-archive> ${CEF_BUILD_TYPE}
                   COMMAND PATH=$ENV{PATH} python cef_version.py current > ${PROJECT_BINARY_DIR}/cef_version.txt
                   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/chromium_git/chromium/src/cef/tools
                   COMMENT "Creating CEF chromium package"
                   VERBATIM)

add_custom_command(OUTPUT cef_final
                   COMMAND ${CMAKE_COMMAND} -DPROJECT_BINARY_DIR=${PROJECT_BINARY_DIR}
                                            -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}
                                            -DCORE_SYSTEM_NAME=${CORE_SYSTEM_NAME}
                                            -DCEF_BIT_STR=${CEF_BIT_STR}
                                            -P ${CMAKE_CURRENT_SOURCE_DIR}/lib/cef-base/copy-cef.cmake
                   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/chromium_package
                   COMMENT "Move CEF to addon source"
                   VERBATIM)

add_custom_target(cef-lib-creator ALL DEPENDS ${TARGET_BUILD_STEPS})
