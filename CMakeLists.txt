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

cmake_minimum_required(VERSION 3.5)
project(web.browser.chromium)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake
                      ${CMAKE_MODULE_PATH}
                      ${PROJECT_SOURCE_DIR})

find_package(Kodi REQUIRED)
find_package(kodiplatform REQUIRED)
find_package(p8-platform REQUIRED)
find_package(glm REQUIRED)

include(ExternalProject)
include(KitModules)

# Chromium creator
include(lib/cef-base/CMakeLists.txt)

if(APP_RENDER_SYSTEM STREQUAL "gl" OR NOT APP_RENDER_SYSTEM)
  find_package(OpenGl REQUIRED)
  set(DEPLIBS ${OPENGL_LIBRARIES})
  set(INCLUDES ${OPENGL_INCLUDE_DIR})
  add_definitions(${OPENGL_DEFINITIONS})
elseif(APP_RENDER_SYSTEM STREQUAL "gles")
  find_package(OpenGLES REQUIRED)
  set(DEPLIBS ${OPENGLES_LIBRARIES})
  set(INCLUDES ${OPENGLES_INCLUDE_DIR})
  add_definitions(${OPENGLES_DEFINITIONS})
endif()

if(ARCH STREQUAL x86_64)
  add_definitions(-DWIDEVINE_CPU="x64")
elseif(ARCH STREQUAL x86)
  add_definitions(-DWIDEVINE_CPU="ia32")
elseif(ARCH STREQUAL arm)
  add_definitions(-DWIDEVINE_CPU="arm")
elseif(ARCH STREQUAL aarch64)
  add_definitions(-DWIDEVINE_CPU="aarch64")
else()
  message(SEND_ERROR "System '${CORE_SYSTEM_NAME}' Unknown CPU: ${CPU}")
endif()

if(WIN32)
  add_definitions(-DWIDEVINE_OS="win")
  add_definitions(-DWIDEVINE_LIB="widevinecdm.dll")
elseif(TARGET_DARWIN)
  add_definitions(-DWIDEVINE_OS="mac")
  add_definitions(-DWIDEVINE_LIB="libwidevinecdm.dylib")
else()
  add_definitions(-DWIDEVINE_OS="linux")
  add_definitions(-DWIDEVINE_LIB="libwidevinecdm.so")
endif()

#-------------------------------------------------------------------------------

# Don't allow binary install for CEF (does not support empty INSTALL_COMMAND in system)
set(cef-binary_NO_INSTALL 1)

add_external_project(cef-binary SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/cef)
ExternalProject_Get_Property(cef-binary SOURCE_DIR)
ExternalProject_Get_Property(cef-binary BINARY_DIR)
ExternalProject_Add_StepDependencies(cef-binary download cef-lib-creator)

set_include_dir(cef-binary ${SOURCE_DIR})
set_libraries(cef-binary cef)

add_custom_command(TARGET cef-binary
  # Static libs which becomes added
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libcef_dll_wrapper/${CMAKE_STATIC_LIBRARY_PREFIX}cef_dll_wrapper${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}

  # Shared libs and parts which need present on add-on
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/cef/locales
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR}/Resources/locales      ${CMAKE_CURRENT_BINARY_DIR}/share/cef/locales
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef.pak                ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_100_percent.pak    ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_200_percent.pak    ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_extensions.pak     ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/devtools_resources.pak ${CMAKE_CURRENT_BINARY_DIR}/share/cef

  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/${CMAKE_SHARED_LIBRARY_PREFIX}cef${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/chrome-sandbox           ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/natives_blob.bin         ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/snapshot_blob.bin        ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Release/v8_context_snapshot.bin  ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/icudtl.dat             ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR}/Release                ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
)

#-------------------------------------------------------------------------------

list(APPEND DEPENDS cef-lib-creator cef-binary)

link_directories(${CMAKE_CURRENT_BINARY_DIR}
                 ${CMAKE_CURRENT_BINARY_DIR}/lib/cef)

include_directories(${INCLUDES}
                    ${kodiplatform_INCLUDE_DIRS}
                    ${p8-platform_INCLUDE_DIRS}
                    ${GLM_INCLUDE_DIR}
                    ${KODI_INCLUDE_DIR}/..
                    ${PROJECT_SOURCE_DIR}/lib/cef
                    ${PROJECT_SOURCE_DIR}/src/
                    ${PROJECT_SOURCE_DIR}/src/addon/
                    ${PROJECT_SOURCE_DIR}/src/addon/Dialogs/
                    ${PROJECT_SOURCE_DIR}/src/addon/settings/
                    ${PROJECT_SOURCE_DIR}/src/addon/utils/)

list(APPEND DEPLIBS ${p8-platform_LIBRARIES}
                    ${kodiplatform_LIBRARIES}
                    cef_dll_wrapper)

if(WIN32)
  list(APPEND DEPLIBS ws2_32)
endif()

#-------------------------------------------------------------------------------

list(APPEND KODICHROMIUM_BIN_SOURCES src/app/Main.cpp
                                     src/app/AppOther.cpp
                                     src/app/renderer/AppRenderer.cpp
                                     src/app/renderer/DOMVisitor.cpp
                                     src/app/renderer/V8Handler.cpp
                                     src/MessageIds.cpp)

add_executable(kodichromium ${KODICHROMIUM_BIN_SOURCES})
add_dependencies(kodichromium cef-binary)
target_link_libraries(kodichromium ${DEPLIBS} cef dl)

#-------------------------------------------------------------------------------

set(KODICHROMIUM_CUSTOM_DATA "${CMAKE_CURRENT_BINARY_DIR}/share/cef")
set(KODICHROMIUM_ADDITIONAL_BINARY "${CMAKE_CURRENT_BINARY_DIR}/kodichromium"
                                   "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/chrome-sandbox"
                                   "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/libcef.so")
set(KODICHROMIUM_ADDITIONAL_BINARY_PARTS "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/icudtl.dat"
                                         "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/natives_blob.bin"
                                         "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/snapshot_blob.bin"
                                         "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/v8_context_snapshot.bin")
set(KODICHROMIUM_ADDITIONAL_BINARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/swiftshader")

set(KODICHROMIUM_SOURCES src/addon/renderer/RendererGL.cpp)
set(KODICHROMIUM_HEADERS src/addon/renderer/RendererGL.h)

list(APPEND KODICHROMIUM_SOURCES src/addon/addon.cpp
                                 src/addon/AppBrowser.cpp
                                 src/addon/RequestContextHandler.cpp
                                 src/addon/SandboxControl.cpp
                                 src/addon/SchemeKodi.cpp
                                 src/addon/URICheckHandler.cpp
                                 src/addon/WebBrowserClient.cpp
                                 src/addon/WidevineControl.cpp
                                 src/addon/audio/AudioHandler.cpp
                                 src/addon/gui/DialogBrowserContextMenu.cpp
                                 src/addon/gui/DialogCookie.cpp
                                 src/addon/gui/DialogDownload.cpp
                                 src/addon/gui/DialogFile.cpp
                                 src/addon/gui/DialogKeyboard.cpp
                                 src/addon/gui/GUIManager.cpp
                                 src/addon/interface/Handler.cpp
                                 src/addon/interface/JSDialogHandler.cpp
                                 src/addon/interface/v8/v8-kodi.cpp
                                 src/addon/renderer/IRenderer.cpp
                                 src/addon/renderer/Renderer.cpp
                                 src/addon/utils/StringUtils.cpp
                                 src/addon/utils/SystemTranslator.cpp
                                 src/addon/utils/Utils.cpp
                                 src/MessageIds.cpp)

list(APPEND KODICHROMIUM_HEADERS src/addon/addon.h
                                 src/addon/AppBrowser.h
                                 src/addon/RequestContextHandler.h
                                 src/addon/SandboxControl.h
                                 src/addon/SchemeKodi.h
                                 src/addon/URICheckHandler.h
                                 src/addon/WebBrowserClient.h
                                 src/addon/WidevineControl.h
                                 src/addon/audio/AudioHandler.h
                                 src/addon/gui/DialogBrowserContextMenu.h
                                 src/addon/gui/DialogCookie.h
                                 src/addon/gui/DialogDownload.h
                                 src/addon/gui/DialogFile.h
                                 src/addon/gui/DialogKeyboard.h
                                 src/addon/gui/GUIManager.h
                                 src/addon/interface/Handler.h
                                 src/addon/interface/JSDialogHandler.h
                                 src/addon/interface/v8/v8-kodi.h
                                 src/addon/renderer/IRenderer.h
                                 src/addon/renderer/Renderer.h
                                 src/addon/utils/StringUtils.h
                                 src/addon/utils/SystemTranslator.h
                                 src/addon/utils/Utils.h
                                 src/MessageIds.h)

build_addon(web.browser.chromium KODICHROMIUM DEPLIBS)
add_dependencies(web.browser.chromium cef-lib-creator cef-binary kodichromium)
add_definitions(-DKODICHROMIUM_VERSION="${KODICHROMIUM_VERSION}"
                -DLIBRARY_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}"
                -DLIBRARY_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}")

include(CPack)
