cmake_minimum_required(VERSION 3.5)
project(cef-lib-creator)

# Chromium/CEF source code URL's
set(USED_CEF_REPO https://github.com/kodi-web/cef.git)
set(USED_CEF_BRANCH kodi)

# Custom target command steps
set(TARGET_BUILD_STEPS
  cef_load
  cef_configuration
  cef_creation_release
  cef_package
  cef_final
)

# CMake options
option(CEF_NO_ARCHIVE "Prevent archive creation?" ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(CheckSymbolExists)
include(ArchSetup)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif()

if(ARCH STREQUAL x86_64)
  set(CEF_BUILD_TYPE "--x64-build")
  set(CEF_BUILD_CPU "x64")
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "64")
  set(GN_DEFINES "${GN_DEFINES} target_cpu=x64")
elseif(ARCH STREQUAL x86)
  set(CEF_BUILD_CPU "x86")
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "32")
  set(GN_DEFINES "${GN_DEFINES} target_cpu=x86")
elseif(ARCH STREQUAL arm)
  set(CEF_BUILD_TYPE "--arm-build")
  set(CEF_BUILD_CPU "arm")
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "32")
  set(GN_DEFINES "${GN_DEFINES} target_cpu=arm")
  set(GN_DEFINES "${GN_DEFINES} arm_use_neon=false")
  set(GN_DEFINES "${GN_DEFINES} arm_float_abi=soft arm_use_thumb=false")
elseif(ARCH STREQUAL aarch64)
  set(CEF_BUILD_TYPE "--arm64-build")
  set(CEF_BUILD_CPU "arm64")
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "32")
  set(GN_DEFINES "${GN_DEFINES} target_cpu=arm64")
else()
  message(SEND_ERROR "System '${CORE_SYSTEM_NAME}' Unknown CPU: ${CPU}")
endif()

if(CORE_SYSTEM_NAME STREQUAL android)
  set(GN_DEFINES "${GN_DEFINES} target_os=android")
elseif(CORE_SYSTEM_NAME STREQUAL freebsd)
elseif(CORE_SYSTEM_NAME STREQUAL ios)
  set(GN_DEFINES "${GN_DEFINES} target_os=ios")
elseif(CORE_SYSTEM_NAME STREQUAL osx)
  set(GN_DEFINES "${GN_DEFINES} target_os=mac")
elseif(CORE_SYSTEM_NAME STREQUAL windows)
  set(GN_DEFINES "${GN_DEFINES} target_os=win")
elseif(CORE_SYSTEM_NAME STREQUAL linux)
  set(GN_DEFINES "${GN_DEFINES} target_os=linux")
endif()

set(ENV{PATH} "${PROJECT_BINARY_DIR}/depot_tools:$ENV{PATH}")
set(GN_DEFINES "${GN_DEFINES} use_jumbo_build=true")
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
set(GN_DEFINES "${GN_DEFINES} fatal_linker_warnings=false")
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set(GN_DEFINES "${GN_DEFINES} is_debug=false")
  set(GN_DEFINES "${GN_DEFINES} remove_webcore_debug_symbols=true")
  set(GN_DEFINES "${GN_DEFINES} symbol_level=1")
endif()

add_custom_command(OUTPUT cef_load
                   COMMAND python ${CMAKE_CURRENT_LIST_DIR}/automate-git.py
                              --url=${USED_CEF_REPO}
                              --checkout=${USED_CEF_BRANCH}
                              --download-dir=${PROJECT_BINARY_DIR}/chromium_git
                              --depot-tools-archive=${USED_DEPOT_TOOLS}
                              --depot-tools-dir=${PROJECT_BINARY_DIR}/depot_tools
                              --no-distrib
                              --no-build
#                               --force-clean
                   COMMENT "Download CEF chromium"
                   VERBATIM)

add_custom_command(OUTPUT cef_configuration
                   COMMAND ${CMAKE_COMMAND} -E env "GN_DEFINES=${GN_DEFINES}" "PATH=$ENV{PATH}" python tools/gclient_hook.py
                   WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/chromium_git/chromium/src/cef
                   COMMENT "Creating CEF chromium"
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
