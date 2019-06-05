###############################################################################
#                                                                             #
#     Copyright (C) 2019 Team KODI                                            #
#     http://kodi.tv                                                          #
#                                                                             #
#  This program is free software: you can redistribute it and/or modify       #
#  it under the terms of the GNU General Public License as published by       #
#  the Free Software Foundation, either version 3 of the License, or          #
#  (at your option) any later version.                                        #
#                                                                             #
#  This program is distributed in the hope that it will be useful,            #
#  but WITHOUT ANY WARRANTY; without even the implied warranty of             #
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              #
#  GNU General Public License for more details.                               #
#                                                                             #
#  You should have received a copy of the GNU General Public License          #
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.      #
#                                                                             #
###############################################################################
#
# This CMake script generate the Chromium Embedded Framework library by the use
# of his python scripts and Chromium's GN and ninja ways.
#
# Due to special ways used on Python scripts is it not direct possible to build
# by CMake direct and done by ExternalProject here. This are separated to allow
# select of wanted build part.
#
# This is part of Kodi's web.browser.chromium addon, but constructed to use
# also independent for somewhere else.
#
# To use the CMake ExternalProject way instead of add_custom_target is to have
# more control over steps. With add_custom_target is the complete call done
# all the time, with ExternalProject is it done only one time unless,
# --clean-first is used to force.
#
#-------------------------------------------------------------------------------
#
# Example:
#   mkdir build
#   cd build
#   cmake .. -DTHIRD_PARTY_PATH=D:/chromium
#   cmake --build . --config "Release"
#
# Example for step target:
#   cmake --build . --config "Release" --target configure_chromium
#
# If on the calls --clean-first becomes added is the build forced to do again.
#
# Following step targets are present:
#  ____________________________________________________________________________
# | Flow Pos. | Name                 | Description                             |
# |___________|______________________|_________________________________________|
# |     1     | depot_tools          | To download for Chromium build required |
# |           |                      | build tools, this are then set to the   |
# |           |                      | PATH.                                   |
# |___________|______________________|_________________________________________|
# |     2     | load_chromium        | Project to make download of Chromium    |
# |           |                      | source code                             |
# |___________|______________________|_________________________________________|
# |     3     | configure_chromium   | To configure chromium by call of Python |
# |           |                      | gclient_hook.py script inside CEF       |
# |___________|______________________|_________________________________________|
# |     4     | build_chromium       | To build Chromium itself                |
# |___________|______________________|_________________________________________|
# |     5     | cef_package          | Generates the required DevKit package   |
# |           |                      |                                         |
# |___________|______________________|_________________________________________|
#
#-------------------------------------------------------------------------------
#
#                                 !!! WARNING !!!
#
# If you use this script on Windows make sure your drive C: have more as
# 40 GByte free space, otherwise define with "-DTHIRD_PARTY_PATH=YOUR_PATH"
# place to somewhere else.
#
# Windows is limited to a path length of 256 characters what makes it
# impossible to build over Kodi's addon build folder and define separate to
# C:/chromium where it becomes created!
#
# Also if windows registry and git commands are changes is this not fixed.
# Chromium's build way use a own git where not can be edited!
###############################################################################

cmake_minimum_required(VERSION 3.5)
project(cef-lib-creator)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/CMake)

# Chromium/CEF source code URL's
set(USED_CEF_REPO https://github.com/kodi-web/cef.git)
set(USED_CEF_BRANCH kodi)
set(USED_CHROMIUM_REPO https://chromium.googlesource.com/chromium/src.git)

include(ExternalProject)
include(CheckSymbolExists)
include(ArchSetup)

# CMake options
option(CEF_NO_ARCHIVE "Prevent archive creation?" ON)
if(CORE_SYSTEM_NAME STREQUAL windows)
  # Set C:\chromium as default if no -DTHIRD_PARTY_PATH=... is defined
  # On Windows brings it problems by use of cmake build folders where path
  # length becomes longer as 256 characters, git used by Chromium does not
  # work with longer (no matter what in Windows registry or somewhere is tried).
  set(THIRD_PARTY_PATH "C:/chromium" CACHE STRING "Chromium build directory")
else()
  set(THIRD_PARTY_PATH ${PROJECT_BINARY_DIR}/chromium CACHE STRING "Chromium build directory")
endif()
set(DEPOT_TOOLS_DIR ${THIRD_PARTY_PATH}/depot_tools CACHE STRING "Depot_tools directory")

# Check x86 is tried on Mac (not supported)
if(CORE_SYSTEM_NAME STREQUAL osx AND ARCH STREQUAL x86)
  message(FATAL_ERROR "32-bit Mac OS X builds are no longer supported by CEF. Only 64bit possible.")
endif()

#===============================================================================
# Setup general build defines
#------------------------------------------------------------------------------.
#                                                                              |

# if CMAKE_BUILD_TYPE is not set fallback to Release as default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Set the used Python or
# force on windows to use python.bat from depot_tools
if(NOT CORE_SYSTEM_NAME STREQUAL windows)
  find_program(PYTHON "python")
else()
  set(PYTHON "${DEPOT_TOOLS_DIR}/python.bat")
 endif()

if(ARCH STREQUAL x86_64)
  set(CEF_BUILD_TYPE "--x64-build")
  set(CEF_BUILD_CPU "x64")
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "64")
elseif(ARCH STREQUAL x86)
  set(CEF_BUILD_CPU "x86")
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "32")
elseif(ARCH STREQUAL arm)
  set(CEF_BUILD_TYPE "--arm-build")
  set(CEF_BUILD_CPU "arm")
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "32")
elseif(ARCH STREQUAL aarch64)
  set(CEF_BUILD_TYPE "--arm64-build")
  set(CEF_BUILD_CPU "arm64")
  set(CEF_IS_64BIT FALSE)
  set(CEF_BIT_STR "32")
else()
  message(SEND_ERROR "System '${CORE_SYSTEM_NAME}' Unknown CPU: ${CPU}")
endif()

#                                                                              |
#------------------------------------------------------------------------------*


#===============================================================================
# Setup GN_DEFINES
#------------------------------------------------------------------------------.
#                                                                              |

# Setup OS independent ninja GN defines
set(GN_DEFINES               "use_jumbo_build=true")
set(GN_DEFINES "${GN_DEFINES} treat_warnings_as_errors=false")  # To prevent build stop by possible present warnings
set(GN_DEFINES "${GN_DEFINES} enable_widevine=true")            # To allow DRM media streams
set(GN_DEFINES "${GN_DEFINES} rtc_use_h264=true")
set(GN_DEFINES "${GN_DEFINES} is_official_build=true")
set(GN_DEFINES "${GN_DEFINES} proprietary_codecs=true")
set(GN_DEFINES "${GN_DEFINES} ffmpeg_branding=Chrome")
set(GN_DEFINES "${GN_DEFINES} fatal_linker_warnings=false")     # To prevent build stop by possible present warnings
set(GN_DEFINES "${GN_DEFINES} enable_ac3_eac3_audio_demuxing=true")
set(GN_DEFINES "${GN_DEFINES} enable_basic_print_dialog=false")
set(GN_DEFINES "${GN_DEFINES} enable_desktop_in_product_help=false")
set(GN_DEFINES "${GN_DEFINES} enable_dolby_vision_demuxing=true")
set(GN_DEFINES "${GN_DEFINES} enable_hevc_demuxing=true")
set(GN_DEFINES "${GN_DEFINES} enable_hls_sample_aes=true")
set(GN_DEFINES "${GN_DEFINES} enable_libaom_decoder=true")
set(GN_DEFINES "${GN_DEFINES} enable_mpeg_h_audio_demuxing=true")
set(GN_DEFINES "${GN_DEFINES} enable_mse_mpeg2ts_stream_parser=true")
set(GN_DEFINES "${GN_DEFINES} rtc_build_examples=false")
if(CMAKE_BUILD_TYPE STREQUAL Release)
  set(GN_DEFINES "${GN_DEFINES} is_debug=false")
  set(GN_DEFINES "${GN_DEFINES} remove_webcore_debug_symbols=true")
  set(GN_DEFINES "${GN_DEFINES} symbol_level=1")
endif()

# Set chromium OS dependent ninja GN defines
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
  set(GN_DEFINES "${GN_DEFINES} use_cups=false")                # To prevent: "error: 'ppdFindChoice' is deprecated"
  set(GN_DEFINES "${GN_DEFINES} use_allocator=none")            # To prevent: "Check failed: base::allocator::IsAllocatorInitialized()."
  set(GN_DEFINES "${GN_DEFINES} use_alsa=false")                # To prevent not wanted audio, becomes done by Kodi itself
  set(GN_DEFINES "${GN_DEFINES} use_pulseaudio=false")          # To prevent not wanted audio, becomes done by Kodi itself
  set(GN_DEFINES "${GN_DEFINES} use_gnome_keyring=false")
  set(GN_DEFINES "${GN_DEFINES} target_os=linux")
endif()

# Setup CPU dependent ninja GN defines
if(ARCH STREQUAL x86_64)
  set(GN_DEFINES "${GN_DEFINES} target_cpu=x64")
elseif(ARCH STREQUAL x86)
  set(GN_DEFINES "${GN_DEFINES} target_cpu=x86")
elseif(ARCH STREQUAL arm)
  set(GN_DEFINES "${GN_DEFINES} target_cpu=arm")
  set(GN_DEFINES "${GN_DEFINES} arm_use_neon=false")
  set(GN_DEFINES "${GN_DEFINES} arm_float_abi=soft arm_use_thumb=false")
elseif(ARCH STREQUAL aarch64)
  set(GN_DEFINES "${GN_DEFINES} target_cpu=arm64")
endif()

# Setup GN_ARGUMENTS
if(CORE_SYSTEM_NAME STREQUAL windows)
  set(GN_ARGUMENTS                 "--ide=vs2017")
  set(GN_ARGUMENTS "${GN_ARGUMENTS} --sln=cef")
  set(GN_ARGUMENTS "${GN_ARGUMENTS} --filters=//cef/*")
endif()

if(CORE_SYSTEM_NAME STREQUAL osx)
  set(AUTOMATE_EXTRA_FLAGS --x64-build)
endif()

#                                                                              |
#------------------------------------------------------------------------------*


#===============================================================================
# Load for chromium build required depot tools
# Example direct call:
#   cmake --build . --config "Release" --target depot_tools --clean-first
#------------------------------------------------------------------------------.
#                                                                              |
ExternalProject_Add(
  depot_tools

  GIT_REPOSITORY      https://chromium.googlesource.com/chromium/tools/depot_tools.git
  GIT_TAG             "master"

  CONFIGURE_COMMAND   ""
  PATCH_COMMAND       ""
  SOURCE_DIR          "${DEPOT_TOOLS_DIR}"

  TEST_COMMAND        ""
  BUILD_COMMAND       ""
  INSTALL_COMMAND     ""
)

# For Windows call the update to have Python, Git and SVN
if(CORE_SYSTEM_NAME STREQUAL windows)
  ExternalProject_Add_Step(
    depot_tools
    update_depot_tools

    COMMAND ${CMAKE_COMMAND} -E env ${DEPOT_TOOLS_DIR}/update_depot_tools.bat
    DEPENDEES update
  )
endif()

# Add for Python scripts the binary path to Chromium depot tools
if(CORE_SYSTEM_NAME STREQUAL windows)
  set(CMAKE_MSVCIDE_RUN_PATH "${DEPOT_TOOLS_DIR}")
else()
  set(ENV{PATH} "${DEPOT_TOOLS_DIR}:$ENV{PATH}")
endif()

message(STATUS "Chromium depot tools directory: ${DEPOT_TOOLS_DIR}")

#                                                                              |
#------------------------------------------------------------------------------*


#===============================================================================
# Load Chromium itself
# Example direct call:
#   cmake --build . --config "Release" --target load_chromium --clean-first
#------------------------------------------------------------------------------.
#                                                                              |

ExternalProject_Add(
  load_chromium

  # Load required automate-git.py
  DOWNLOAD_COMMAND    ${CMAKE_COMMAND} -DAUTOMATE_GIT_BRANCH=${USED_CEF_BRANCH}
                                       -DAUTOMATE_GIT_DEST=${PROJECT_BINARY_DIR}
                                       -P ${CMAKE_SOURCE_DIR}/CMake/LoadAutomateGit.cmake

  # Undef all ExternalProject commands, Chromium need special ways by steps
  UPDATE_COMMAND      ""
  PATCH_COMMAND       ""
  CONFIGURE_COMMAND   ""
  SOURCE_DIR          ${THIRD_PARTY_PATH}/chromium_git
  TEST_COMMAND        ""
  BUILD_COMMAND       ""
  INSTALL_COMMAND     ""
  BUILD_IN_SOURCE     TRUE
  UPDATE_DISCONNECTED TRUE
  DEPENDS             depot_tools
)

ExternalProject_Add_Step(
  load_chromium
  download_chromium

  # Start automate-git.py to download chromium from web and add CEF changes on end
  COMMAND             ${PYTHON} ${PROJECT_BINARY_DIR}/automate-git.py
                              --url=${USED_CEF_REPO}
                              --checkout=${USED_CEF_BRANCH}
                              --download-dir=${THIRD_PARTY_PATH}/chromium_git
                              --depot-tools-dir=${DEPOT_TOOLS_DIR}
                              --no-distrib
                              --no-build
                              --no-depot-tools-update
                              ${AUTOMATE_EXTRA_FLAGS}
                              --force-clean
  COMMENT             "Performing download of Chromium from Web by automate-git.py from CEF"
  WORKING_DIRECTORY   ${THIRD_PARTY_PATH}/chromium_git
  DEPENDEES           download
)

#                                                                              |
#------------------------------------------------------------------------------*


#===============================================================================
# Configure Chromium itself
# Example direct call:
#   cmake --build . --config "Release" --target configure_chromium --clean-first
#------------------------------------------------------------------------------.
#                                                                              |

ExternalProject_Add(
  configure_chromium

  # Undef all ExternalProject commands, Chromium need special ways by steps
  DOWNLOAD_COMMAND    ""
  UPDATE_COMMAND      ""
  PATCH_COMMAND       ""
  CONFIGURE_COMMAND   ""
  SOURCE_DIR          ${THIRD_PARTY_PATH}/chromium_git
  TEST_COMMAND        ""
  BUILD_COMMAND       ""
  INSTALL_COMMAND     ""
  BUILD_IN_SOURCE     TRUE
  UPDATE_DISCONNECTED TRUE
  DEPENDS             load_chromium
)

# Step to configure chromium for build
if(CORE_SYSTEM_NAME STREQUAL windows)
  # Windows need different way as other OS, PATH is set there by
  # CMAKE_MSVCIDE_RUN_PATH and need also GN_ARGUMENTS
  ExternalProject_Add_Step(
    configure_chromium
    configuration

    COMMAND             ${CMAKE_COMMAND} -E env GN_DEFINES=${GN_DEFINES}
                                                GN_ARGUMENTS=${GN_ARGUMENTS}
                                                ${PYTHON} tools/gclient_hook.py
    COMMENT             "Run GN ninja configure by call of CEF's gclient_hook.py"
    WORKING_DIRECTORY   ${THIRD_PARTY_PATH}/chromium_git/chromium/src/cef
    DEPENDEES           configure
  )
else()
  # For other OS's are the binary path defined by PATH=$ENV{PATH}
  ExternalProject_Add_Step(
    configure_chromium
    configuration

    COMMAND             ${CMAKE_COMMAND} -E env GN_DEFINES=${GN_DEFINES}
                                                PATH=$ENV{PATH}
                                                ${PYTHON} tools/gclient_hook.py
    COMMENT             "Run GN ninja configure by call of CEF's gclient_hook.py"
    WORKING_DIRECTORY   ${THIRD_PARTY_PATH}/chromium_git/chromium/src/cef
    DEPENDEES           configure
  )
endif()

#                                                                              |
#------------------------------------------------------------------------------*


#===============================================================================
# Create Chromium itself
# Example direct call:
#   cmake --build . --config "Release" --target build_chromium
#
# Direct call is nice to test code changes on Chromium and to prevent call of
# automate-git.py from CEF where his patch call can be in conflict with changes.
# Warning: Do not add "--clean-first" otherwise the step before wants to add
# patches.
#------------------------------------------------------------------------------.
#                                                                              |

# Build libcef as mandatory part
set(NINJA_BUILD_TYPE ${CMAKE_BUILD_TYPE}_GN_${CEF_BUILD_CPU})
set(NINJA_BUILD_PARTS "cef")

# If linux add also sandbox build
if(CORE_SYSTEM_NAME STREQUAL linux)
  list(APPEND NINJA_BUILD_PARTS "chrome_sandbox")
endif()

ExternalProject_Add(
  build_chromium

  # Undef all ExternalProject commands, Chromium need special ways by steps
  DOWNLOAD_COMMAND    ""
  UPDATE_COMMAND      ""
  PATCH_COMMAND       ""
  CONFIGURE_COMMAND   ""
  SOURCE_DIR          ${THIRD_PARTY_PATH}/chromium_git
  TEST_COMMAND        ""
  BUILD_COMMAND       ""
  INSTALL_COMMAND     ""
  BUILD_IN_SOURCE     TRUE
  UPDATE_DISCONNECTED TRUE
  DEPENDS             configure_chromium
)

if(CORE_SYSTEM_NAME STREQUAL windows)
  ExternalProject_Add_Step(
    build_chromium
    builder

    COMMAND             ${CMAKE_COMMAND} -E env ninja -C out/${NINJA_BUILD_TYPE} ${NINJA_BUILD_PARTS}
    COMMENT             "Run GN ninja build process"
    WORKING_DIRECTORY   ${THIRD_PARTY_PATH}/chromium_git/chromium/src
    DEPENDEES           build
  )
else()
  ExternalProject_Add_Step(
    build_chromium
    builder

    COMMAND             ${CMAKE_COMMAND} -E env GN_DEFINES=${GN_DEFINES}
                                                PATH=$ENV{PATH}
                                                ninja -C out/${NINJA_BUILD_TYPE} ${NINJA_BUILD_PARTS}
    COMMENT             "Run GN ninja build process"
    WORKING_DIRECTORY   ${THIRD_PARTY_PATH}/chromium_git/chromium/src
    DEPENDEES           build
  )
endif()

#                                                                              |
#------------------------------------------------------------------------------*


#===============================================================================
# Create for addon required CEF dev kit package
# Example direct call:
#   cmake --build . --config "Release" --target cef_package
#------------------------------------------------------------------------------.
#                                                                              |

ExternalProject_Add(
  cef_package

  # Undef all ExternalProject commands, Chromium need special ways by steps
  DOWNLOAD_COMMAND    ""
  UPDATE_COMMAND      ""
  PATCH_COMMAND       ""
  CONFIGURE_COMMAND   ""
  SOURCE_DIR          ${THIRD_PARTY_PATH}/chromium_git
  TEST_COMMAND        ""
  BUILD_COMMAND       ""
  INSTALL_COMMAND     ""
  BUILD_IN_SOURCE     TRUE
  UPDATE_DISCONNECTED TRUE
  DEPENDS             build_chromium
)

# Step to create the CEF package folder
ExternalProject_Add_Step(
  cef_package
  create_package

  COMMAND             ${CMAKE_COMMAND} -E env ${PYTHON} make_distrib.py --ninja-build
                                                --output-dir ${THIRD_PARTY_PATH}/chromium_package
                                                --allow-partial
                                                $<$<BOOL:${CEF_NO_ARCHIVE}>:--no-archive>
                                                ${CEF_BUILD_TYPE}
  COMMAND             ${PYTHON} cef_version.py current > ${THIRD_PARTY_PATH}/cef_version.txt
  COMMENT             "Creating CEF chromium package"
  WORKING_DIRECTORY   ${THIRD_PARTY_PATH}/chromium_git/chromium/src/cef/tools
  DEPENDEES           build
)

# Step to copy for addon needed CEF package to right place
#
# CEF create a folder with a bigger name e.g.
# "cef_binary_74.0.0-kodi.1973+ga51c714+chromium-74.0.3729.0_windows32_package"
# where it becomes harder for a Kodi addon to find it before it is created.
# With this becomes it copied by separated CMake script to:
# ${PROJECT_BINARY_DIR}/cef where it is easier found.
#
ExternalProject_Add_Step(
  cef_package
  install_build_package

  COMMAND             ${CMAKE_COMMAND} -DPROJECT_BINARY_DIR=${THIRD_PARTY_PATH}
                                       -DSOURCE_DIR=${CMAKE_INSTALL_PREFIX}/src/cef
                                       -DCORE_SYSTEM_NAME=${CORE_SYSTEM_NAME}
                                       -DCEF_BIT_STR=${CEF_BIT_STR}
                                       -P ${CMAKE_SOURCE_DIR}/CMake/copy-cef.cmake
  COMMENT             "Move CEF to addon source"
  WORKING_DIRECTORY   ${THIRD_PARTY_PATH}/chromium_package
  DEPENDEES           create_package
)

#                                                                              |
#------------------------------------------------------------------------------*
