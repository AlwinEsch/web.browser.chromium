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

project(web.browser.chromium)

cmake_minimum_required(VERSION 2.8.12.2)

set(CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_SOURCE_DIR}/CMake
  ${CMAKE_MODULE_PATH}
  ${PROJECT_SOURCE_DIR}
)

# When building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# Add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

enable_language(CXX)

# ----------------------------------------------------------------------------
# Break in case of popular CMake configuration mistakes
# ----------------------------------------------------------------------------
if(NOT CMAKE_SIZEOF_VOID_P GREATER 0)
  message(FATAL_ERROR "CMake fails to deterimine the bitness of target platform.
  Please check your CMake and compiler installation. If you are crosscompiling then ensure that your CMake toolchain file correctly sets the compiler details.")
endif()

# 64-bit system?
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(BITSIZE "64")
else()
  set(BITSIZE "32")
endif()

find_package(Kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(p8-platform REQUIRED)

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

# -----------------------------------------------------------------------------
# Determine the target processor
# -----------------------------------------------------------------------------
# if CMAKE_SYSTEM_NAME is here already set, either it comes from a toolchain file
# or it was set via -DCMAKE_SYSTEM_NAME=...
# if that's the case, assume we are crosscompiling
if(CMAKE_SYSTEM_PROCESSOR)
  if(NOT DEFINED CMAKE_CROSSCOMPILING)
    set(CMAKE_CROSSCOMPILING TRUE)
  endif(NOT DEFINED CMAKE_CROSSCOMPILING)
else(CMAKE_SYSTEM_PROCESSOR)
  set(CMAKE_SYSTEM_NAME      "${CMAKE_HOST_SYSTEM_NAME}")
  set(CMAKE_SYSTEM_VERSION   "${CMAKE_HOST_SYSTEM_VERSION}")
  set(CMAKE_SYSTEM_PROCESSOR "${CMAKE_HOST_SYSTEM_PROCESSOR}")
  set(CMAKE_CROSSCOMPILING FALSE)
endif(CMAKE_SYSTEM_PROCESSOR)
string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} LOWERCASE_CMAKE_SYSTEM_PROCESSOR)
if(LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
  set(ARCH "arm")
elseif(LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "(x86_64|amd64)")
  set(ARCH "x86_64")
elseif(LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "(i[3-6]86|x86)")
  set(ARCH "x86")
elseif(LOWERCASE_CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
  set(ARCH "aarch64")
else()
  message(FATAL_ERROR "Unknown CPU '${CMAKE_SYSTEM_PROCESSOR}'")
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
# Detect OS and define macros appropriately
if(${CORE_SYSTEM_NAME} STREQUAL windows)
  add_definitions(-DTARGET_WINDOWS)
  set(OS_WINDOWS 1)
  if(WIN32)
    add_definitions(-DWINDOWS)
    if(MSVC)
      message(STATUS "${BoldWhite}| Compiling on Windows with MSVC                                     ${BoldWhite}|${ColourReset}")
    else()
      message(STATUS "${BoldWhite}| Compiling on Windows                                               ${BoldWhite}|${ColourReset}")
    endif()
  elseif(CYGWIN)
    message(STATUS "${BoldWhite}| Compiling on Cygwin                                                ${BoldWhite}|${ColourReset}")
    add_definitions(-DCYGWIN)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN/lib")
  endif()
elseif(${CORE_SYSTEM_NAME} STREQUAL linux)
  add_definitions(-DTARGET_POSIX)
  add_definitions(-DTARGET_LINUX)
  add_definitions(-D_LINUX)
  message(STATUS "${BoldWhite}| Compiling on Linux                                                 ${BoldWhite}|${ColourReset}")
  set(OS_LINUX 1)
  set(OS_POSIX 1)
  set(CMAKE_INSTALL_RPATH "\$ORIGIN")
elseif(${CORE_SYSTEM_NAME} STREQUAL darwin)
  add_definitions(-DTARGET_POSIX)
  add_definitions(-DTARGET_DARWIN)
  add_definitions(-DTARGET_DARWIN_OSX)
  add_definitions(-D_LINUX)
  message(STATUS "${BoldWhite}| Compiling on Mac OS X                                              ${BoldWhite}|${ColourReset}")
  set(OS_POSIX 1)
  set(OS_DARWIN 1)
  set(OS_DARWIN_OSX 1)
  set(CMAKE_INSTALL_RPATH "@rpath/lib")
elseif(${CORE_SYSTEM_NAME} STREQUAL freebsd)
  add_definitions(-DTARGET_POSIX)
  add_definitions(-DTARGET_FREEBSD)
  add_definitions(-D_LINUX)
  message(STATUS "${BoldWhite}| Compiling on FreeBSD                                               ${BoldWhite}|${ColourReset}")
  set(OS_POSIX 1)
  set(OS_LINUX 1)
  set(OS_FREEBSD 1)
  set(CMAKE_INSTALL_RPATH "\$ORIGIN")
elseif(${CORE_SYSTEM_NAME} STREQUAL ios)
  add_definitions(-DTARGET_POSIX)
  add_definitions(-DTARGET_DARWIN)
  add_definitions(-DTARGET_DARWIN_IOS)
  add_definitions(-D_LINUX)
  message(STATUS "${BoldWhite}| Compiling on iOS                                                   ${BoldWhite}|${ColourReset}")
  set(OS_POSIX 1)
  set(OS_DARWIN 1)
  set(OS_DARWIN_IOS 1)
  set(CMAKE_INSTALL_RPATH "@rpath/lib")
elseif(${CORE_SYSTEM_NAME} STREQUAL rbpi)
  add_definitions(-DTARGET_POSIX)
  add_definitions(-DTARGET_LINUX)
  add_definitions(-DTARGET_RASPBERRY_PI)
  add_definitions(-D_LINUX)
  add_definitions(-D_ARMEL)
  message(STATUS "${BoldWhite}| Compiling on Raspberry PI                                          ${BoldWhite}|${ColourReset}")
  set(OS_POSIX 1)
  set(OS_LINUX 1)
  set(OS_ARMEL 1)
  set(OS_RASPBERRY_PI 1)
  set(CMAKE_INSTALL_RPATH "\$ORIGIN")
elseif(${CORE_SYSTEM_NAME} STREQUAL android)
  add_definitions(-DTARGET_POSIX)
  add_definitions(-DTARGET_LINUX)
  add_definitions(-DTARGET_ANDROID)
  add_definitions(-D_LINUX)
  message(STATUS "${BoldWhite}| Compiling on Android                                               ${BoldWhite}|${ColourReset}")
  set(OS_POSIX 1)
  set(OS_LINUX 1)
  set(OS_ANDROID 1)
endif()
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

if(NOT WIN32)
  find_package(OpenGL)
  if(OPENGL_FOUND)
    add_definitions(-DHAVE_GL)
    set(KODICHROMIUM_SOURCES src/addon/WebBrowserClientOpenGL.cpp)
    set(KODICHROMIUM_HEADERS src/addon/WebBrowserClientOpenGL.h)
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
      set(KODICHROMIUM_HEADERS src/addon/WebBrowserClientOpenGLES.h
                               src/addon/EGLHelpers/VisGUIShader.h
                               src/addon/EGLHelpers/VisMatrixGLES.h
                               src/addon/EGLHelpers/VisShader.h)
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
    set(KODICHROMIUM_HEADERS src/addon/WebBrowserClientDirectX.h)
    set(INCLUDES ${D3DX9_INCLUDE_DIR})
    set(DEPLIBS ${D3DX9_LIBRARIES})
    message(STATUS "Web Browser support enabled via ${BoldGreen}DirectX${ColourReset}")
  else()
    message(FATAL_ERROR " ${BoldRed}Web Browser support not possible because DirectX wasn't found${ColourReset}")
  endif()
endif()

add_project(cef-binary REQUIRED)
get_include_dir(cef-binary TEMP)
list(APPEND INCLUDES ${TEMP})

link_directories(${CMAKE_CURRENT_BINARY_DIR}
                 ${CMAKE_CURRENT_BINARY_DIR}/lib/cef)

include_directories(${INCLUDES}
                    ${kodiplatform_INCLUDE_DIRS}
                    ${p8-platform_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}
                    ${PROJECT_SOURCE_DIR}/src/addon/
                    ${PROJECT_SOURCE_DIR}/src/addon/Dialogs/
                    ${PROJECT_SOURCE_DIR}/src/addon/settings/
                    ${PROJECT_SOURCE_DIR}/src/addon/utils/)

list(APPEND KODICHROMIUM_SOURCES src/addon/addon.cpp
                                 src/addon/DownloadHandler.cpp
                                 src/addon/DOMVisitor.cpp
                                 src/addon/GeolocationPermission.cpp
                                 src/addon/MessageIds.cpp
                                 src/addon/SystemTranslator.cpp
                                 src/addon/UploadHandler.cpp
                                 src/addon/URICheckHandler.cpp
                                 src/addon/WebBrowserClientBase.cpp
                                 src/addon/WebBrowserManager.cpp
                                 src/addon/Cookies/CookieHandler.cpp
                                 src/addon/JSInterface/Handler.cpp
                                 src/addon/JSInterface/JSDialogHandler.cpp
                                 src/addon/JSInterface/V8Handler.cpp
                                 src/addon/utils/Utils.cpp)

list(APPEND KODICHROMIUM_HEADERS src/addon/addon.h
                                 src/addon/DownloadHandler.h
                                 src/addon/DOMVisitor.h
                                 src/addon/GeolocationPermission.h
                                 src/addon/MessageIds.h
                                 src/addon/SystemTranslator.h
                                 src/addon/UploadHandler.h
                                 src/addon/URICheckHandler.h
                                 src/addon/WebBrowserClientBase.h
                                 src/addon/WebBrowserManager.h
                                 src/addon/Cookies/CookieHandler.cpp
                                 src/addon/JSInterface/Handler.h
                                 src/addon/JSInterface/JSDialogHandler.h
                                 src/addon/JSInterface/V8Handler.h
                                 src/addon/utils/Utils.h)

list(APPEND KODICHROMIUM_BIN_SOURCES src/app/main.cpp
                                     src/app/client_app.cpp
                                     src/app/client_app_other.cpp
                                     src/app/client_app_renderer.cpp
                                     src/app/client_switches.cpp)

list(APPEND DEPLIBS ${p8-platform_LIBRARIES}
                    ${kodiplatform_LIBRARIES}
                    cef_dll_wrapper
                    cef)

if(WIN32)
  list(APPEND DEPLIBS ws2_32)
endif()

add_executable(kodichromium ${KODICHROMIUM_BIN_SOURCES})
add_dependencies(kodichromium cef-binary)
target_link_libraries(kodichromium ${DEPLIBS})

set(KODICHROMIUM_CUSTOM_DATA "${CMAKE_CURRENT_BINARY_DIR}/share/cef")
set(KODICHROMIUM_ADDITIONAL_BINARY "${CMAKE_CURRENT_BINARY_DIR}/kodichromium"
                                   "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/chrome-sandbox"
                                   "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/libcef.so")
set(KODICHROMIUM_ADDITIONAL_BINARY_PARTS "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/icudtl.dat"
                                         "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/natives_blob.bin"
                                         "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/snapshot_blob.bin")

build_addon(web.browser.chromium
                KODICHROMIUM
                DEPLIBS)

if(PACKAGE_ZIP EQUAL 1)
  # needed for project installing
  list(APPEND CMAKE_ARGS -DPACKAGE_ZIP=1)
  message(STATUS "package zip specified")
elseif(PACKAGE_TGZ EQUAL 1)
  # needed for project installing
  list(APPEND CMAKE_ARGS -DPACKAGE_TGZ=1)
  message(STATUS "package tgz specified")
endif()

add_dependencies(web.browser.chromium cef-binary)
add_definitions(-DKODICHROMIUM_VERSION="${KODICHROMIUM_VERSION}")

include(CPack)
