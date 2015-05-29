###############################################################################
#                                                                             #
#     Copyright (C) 2015 Team KODI                                            #
#     http://kodi.tv                                                          #
#                                                                             #
# This Program is free software; you can redistribute it and/or modify        #
# it under the terms of the GNU General Public License as published by        #
# the Free Software Foundation; either version 2, or (at your option)         #
# any later version.                                                          #
#                                                                             #
# This Program is distributed in the hope that it will be useful,             #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU General Public License           #
# along with KODI; see the file COPYING.  If not, see                         #
# <http://www.gnu.org/licenses/>.                                             #
#                                                                             #
###############################################################################

project(web.KODIChromiumBrowser)

cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}
)

enable_language(CXX)

find_package(kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(platform REQUIRED)

include(color-defaults)
include(KitModules)

add_definitions(-DUSE_DEMUX)

if(NOT CORE_SYSTEM_NAME)
  string(TOLOWER ${CMAKE_SYSTEM_NAME} CORE_SYSTEM_NAME)
endif()

if(DEBUG)
  add_definitions(-DDEBUG)
endif()
if(DEBUG)
  set(CMAKE_BUILD_TYPE Debug)
elseif(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# 64-bit system?
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(BITSIZE "64")
else()
  set(BITSIZE "32")
endif()

message(STATUS "${BoldWhite}==================================================================${ColourReset} --")
message(STATUS "${BoldWhite}**                                                              **${ColourReset} --")
message(STATUS "${BoldWhite}**          KODI's Chromium Embedded Framework browser          **${ColourReset} --")
message(STATUS "${BoldWhite}**                       creation started                       **${ColourReset} --")
message(STATUS "${BoldWhite}**                                                              **${ColourReset} --")
message(STATUS "${BoldWhite}==================================================================${ColourReset} --")
message(STATUS "${BoldWhite}|               |                               |                ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|               |  CMake build command options  |                ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|               *_______________________________*                ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|                                                                ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DCMAKE_INSTALL_PREFIX:PATH=<path>                            ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t< path to define install directory          ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DDEBUG=1${ColourReset}\t\t< if set debug function parts becomes       ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  enabled                                   ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DCEF_COMPLETE=1${ColourReset}\t< if set the add-on use own builded new CEF ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  binary which can be used to check newer   ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  versions and from devs to have updated    ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  zip file for download url                 ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t${Red}WARNING:  ${ColourReset}${Yellow}Complete build takes a very long time!    ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DCEF_NO_DEBUG_BUILD=1${ColourReset}                                        ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t< used if CEF_COMPLETE=1, then no debug CEF ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  becomes build                             ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DCEF_NO_RELEASE_BUILD=1                                      ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t< used if CEF_COMPLETE=1, then no release   ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  CEF becomes build                         ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|                                                                ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}| cmake <path_to_source_dir> <options...>                        ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|                                                                ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}| Note: Best way to build is to create a './build' folder inside ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}| code dir, this can be easily deleted after performed work      ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}| without a problem on it.                                       ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}|                                                                ${BoldWhite}|${ColourReset} --")
message(STATUS "${BoldWhite}==================================================================${ColourReset} --")

# Determine the platform.
if(${CORE_SYSTEM_NAME} STREQUAL windows)
  set(CEF_OS_NAME "windows")
  set(OS_WINDOWS 1)
elseif(${CORE_SYSTEM_NAME} STREQUAL linux)
  set(CEF_OS_NAME "linux")
  set(OS_LINUX 1)
  set(OS_POSIX 1)
elseif(${CORE_SYSTEM_NAME} STREQUAL darwin)
  set(CEF_OS_NAME "macosx")
  set(OS_MACOSX 1)
  set(OS_POSIX 1)
elseif(${CORE_SYSTEM_NAME} STREQUAL freebsd)
  message(FATAL_ERROR "${BoldWhite}OS ${BoldRed}${CORE_SYSTEM_NAME} ${BoldWhite}currently not supported !!!${ColourReset}")
elseif(${CORE_SYSTEM_NAME} STREQUAL ios)
  message(FATAL_ERROR "${BoldWhite}OS ${BoldRed}${CORE_SYSTEM_NAME} ${BoldWhite}currently not supported !!!${ColourReset}")
elseif(${CORE_SYSTEM_NAME} STREQUAL rbpi)
  message(FATAL_ERROR "${BoldWhite}OS ${BoldRed}${CORE_SYSTEM_NAME} ${BoldWhite}currently not supported !!!${ColourReset}")
elseif(${CORE_SYSTEM_NAME} STREQUAL android)
  message(FATAL_ERROR "${BoldWhite}OS ${BoldRed}${CORE_SYSTEM_NAME} ${BoldWhite}currently not supported !!!${ColourReset}")
endif()

if(CEF_COMPLETE)
  message(STATUS "${BoldWhite}${CEF_OS_NAME} ${BITSIZE}bit ${CMAKE_BUILD_TYPE} build${ColourReset} - ${Yellow}Complete CEF build!${ColourReset}")
  if(CEF_NO_DEBUG_BUILD)
    message(STATUS "${BoldWhite}Debug on complete CEF build disabled${ColourReset}")
  endif()
  if(CEF_NO_RELEASE_BUILD)
    message(STATUS "${BoldWhite}Release on complete CEF build disabled${ColourReset}")
  endif()
else()
  message(STATUS "${BoldWhite}${CEF_OS_NAME} ${BITSIZE}bit ${CMAKE_BUILD_TYPE} build${ColourReset} - ${Green} Fixed CEF loaded from web.${ColourReset}")
endif()

if(NOT WIN32)
  find_package(OpenGL)
  if(OPENGL_FOUND)
    set(INCLUDES ${OPENGL_INCLUDE_DIR})
    set(DEPLIBS ${OPENGL_LIBRARIES})
    add_definitions(-DHAVE_GL)
    message(STATUS "Web Browser support enabled via ${BoldGreen}OpenGL${ColourReset}")
  else()
    find_package(OpenGLES2)
    if(OPENGLES2_FOUND)
      set(KODICHROMIUM_SOURCES src/addon/EGLHelpers/VisGUIShader.cpp
                               src/addon/EGLHelpers/VisMatrixGLES.cpp
                               src/addon/EGLHelpers/VisShader.cpp)
      add_definitions(-DHAVE_GLES2)
      set(INCLUDES ${OPENGLES2_INCLUDE_DIRS})
      set(DEPLIBS ${OPENGLES2_LIBRARIES})
      message(STATUS "Web Browser support enabled via ${BoldGreen}OpenGLES${ColourReset}")
    else()
      message(FATAL_ERROR " ${BoldRed}Web Browser support not possible because OpenGL(ES) wasn't found${ColourReset}")
    endif()
  endif()
else()
  find_package(D3DX9)
  if(D3DX9_FOUND)
    set(INCLUDES ${D3DX9_INCLUDE_DIR})
    set(DEPLIBS ${D3DX9_LIBRARIES})
    add_definitions(-DHAS_DX)
    message(STATUS "Web Browser support enabled via ${BoldGreen}DirectX${ColourReset}")
  else()
    message(FATAL_ERROR " ${BoldRed}Web Browser support not possible because DirectX wasn't found${ColourReset}")
  endif()
endif()

include (${CMAKE_CURRENT_SOURCE_DIR}/versions.cmake)

add_project(cef-binary REQUIRED)
get_include_dir(cef-binary TEMP)
list(APPEND INCLUDES ${TEMP})

link_directories(${CMAKE_CURRENT_BINARY_DIR}/cef
                 ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${INCLUDES}
                    ${kodiplatform_INCLUDE_DIRS}
                    ${platform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/src/addon/
                    ${PROJECT_SOURCE_DIR}/src/addon/utils/)

list(APPEND KODICHROMIUM_SOURCES src/addon/addon.cpp
                                 src/addon/utils/Utils.cpp)

list(APPEND KODICHROMIUM_BIN_SOURCES src/app/main.cpp)

list(APPEND DEPLIBS ${platform_LIBRARIES}
                    ${kodiplatform_LIBRARIES}
                    cef
                    cef_dll_wrapper)
if(WIN32)
  list(APPEND DEPLIBS ws2_32)
endif()

add_executable(kodichromium ${KODICHROMIUM_BIN_SOURCES})
target_link_libraries(kodichromium ${DEPLIBS})

build_addon(web.KODIChromiumBrowser KODICHROMIUM DEPLIBS)

add_definitions(-DKODICHROMIUM_VERSION="${KODICHROMIUM_VERSION}")
add_dependencies(web.KODIChromiumBrowser cef-binary)

include(CPack)
