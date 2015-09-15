###############################################################################
#                                                                             #
#     Copyright (C) 2015 Team KODI                                            #
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
include(web-addon-helpers)

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

message(STATUS "${BoldWhite}======================================================================${ColourReset}")
message(STATUS "${BoldWhite}**                                                                  **${ColourReset}")
message(STATUS "${BoldWhite}**            KODI's Chromium Embedded Framework browser            **${ColourReset}")
message(STATUS "${BoldWhite}**                         creation started                         **${ColourReset}")
message(STATUS "${BoldWhite}**                                                                  **${ColourReset}")
message(STATUS "${BoldWhite}======================================================================${ColourReset}")
message(STATUS "${BoldWhite}|                 |                               |                  ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|                 |  CMake build command options  |                  ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|                 *_______________________________*                  ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|                                                                    ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DCMAKE_INSTALL_PREFIX:PATH=<path>  \t\t\t\t${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t< path to define install directory              ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DPACKAGE_ZIP=1${ColourReset}\t< if set install create a zip package \t\t${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  instead of normal install                     ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DPACKAGE_TGZ=1${ColourReset}\t< if set install create a tgz package \t\t${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  instead of normal install                     ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset}\t\t   ${Yellow}NOTE:  It can only use ZIP or TGZ, not both,\t\t${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset}\t\t   ${Yellow}       use 'make addon-package' to create.  \t\t${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DDEBUG=1${ColourReset}\t\t< if set debug function parts becomes \t\t${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  enabled                                       ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset} ${Green} -DOVERRIDE_PATHS=1${ColourReset}\t\t\t\t\t\t${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t< Set this if you want to disable path check    ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|${ColourReset}\t\t\t  with kodi                                     ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|                                                                    ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}| cmake <path_to_source_dir> <options...>                            ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|                                                                    ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}| Note: Best way to build is to create a './build' folder inside     ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}| codedir, this can be easily deleted after performed work without a ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}| problem on it.                                                     ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}|                                                                    ${BoldWhite}|${ColourReset}")
message(STATUS "${BoldWhite}======================================================================${ColourReset}")

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

if(NOT WIN32)
  find_package(OpenGL)
  if(OPENGL_FOUND)
    add_definitions(-DHAVE_GL)
    set(KODICHROMIUM_SOURCES src/addon/WebBrowserClientOpenGL.cpp)
    set(INCLUDES ${OPENGL_INCLUDE_DIR})
    set(DEPLIBS ${OPENGL_LIBRARIES})
    message(STATUS "Web Browser support enabled via ${BoldGreen}OpenGL${ColourReset}")
  else()
    find_package(OpenGLES2)
    if(OPENGLES2_FOUND)
      add_definitions(-DHAVE_GLES2)
      set(KODICHROMIUM_SOURCES src/addon/WebBrowserClientOpenGLES.cpp
                               src/addon/EGLHelpers/VisGUIShader.cpp
                               src/addon/EGLHelpers/VisMatrixGLES.cpp
                               src/addon/EGLHelpers/VisShader.cpp)
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
    add_definitions(-DHAS_DX)
    set(KODICHROMIUM_SOURCES src/addon/WebBrowserClientDirectX.cpp)
    set(INCLUDES ${D3DX9_INCLUDE_DIR})
    set(DEPLIBS ${D3DX9_LIBRARIES})
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
                    ${PROJECT_SOURCE_DIR}/src/addon/Dialogs/
                    ${PROJECT_SOURCE_DIR}/src/addon/settings/
                    ${PROJECT_SOURCE_DIR}/src/addon/utils/)

list(APPEND KODICHROMIUM_SOURCES src/addon/addon.cpp
                                 src/addon/URICheckHandler.cpp
                                 src/addon/WebBrowserClientBase.cpp
                                 src/addon/WebBrowserManager.cpp
                                 src/addon/Dialogs/WebGUIDialogBrowser.cpp
                                 src/addon/Dialogs/WebGUIDialogContextMenu.cpp
                                 src/addon/settings/SettingsMain.cpp
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

add_custom_command(TARGET kodichromium POST_BUILD
  # Move binary to cef folder
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/kodichromium${CMAKE_EXECUTABLE_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/cef
  COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_CURRENT_BINARY_DIR}/kodichromium${CMAKE_EXECUTABLE_SUFFIX}
)

build_web_addon(web.KODIChromiumBrowser KODICHROMIUM DEPLIBS ${CMAKE_CURRENT_BINARY_DIR}/cef)

if(PACKAGE_ZIP EQUAL 1)
  # needed for project installing
  list(APPEND CMAKE_ARGS -DPACKAGE_ZIP=1)
  message(STATUS "package zip specified")
elseif(PACKAGE_TGZ EQUAL 1)
  # needed for project installing
  list(APPEND CMAKE_ARGS -DPACKAGE_TGZ=1)
  message(STATUS "package tgz specified")
endif()

add_dependencies(web.KODIChromiumBrowser cef-binary)
add_definitions(-DKODICHROMIUM_VERSION="${KODICHROMIUM_VERSION}")

include(CPack)
