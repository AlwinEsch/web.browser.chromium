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

include(lib/cef-base/CMakeLists.txt)

if(APP_RENDER_SYSTEM STREQUAL "gl" OR NOT APP_RENDER_SYSTEM)
  find_package(OpenGl REQUIRED)
  set(DEPLIBS ${OPENGL_LIBRARIES})
  set(includes ${OPENGL_INCLUDE_DIR})
  add_definitions(${OPENGL_DEFINITIONS})
elseif(APP_RENDER_SYSTEM STREQUAL "gles")
  find_package(OpenGLES REQUIRED)
  set(DEPLIBS ${OPENGLES_LIBRARIES})
  set(u ${OPENGLES_INCLUDE_DIR})
  add_definitions(${OPENGLES_DEFINITIONS})
endif()

#-------------------------------------------------------------------------------

# Don't allow binary install for CEF (does not support empty INSTALL_COMMAND in system)
set(cef-binary_NO_INSTALL 1)

add_external_project(cef-binary SOURCE_DIR ${PROJECT_SOURCE_DIR}/lib/cef)
ExternalProject_Get_Property(cef-binary SOURCE_DIR)
ExternalProject_Get_Property(cef-binary BINARY_DIR)

set_include_dir(cef-binary ${SOURCE_DIR})
set_libraries(cef-binary cef)

add_custom_command(TARGET cef-binary
  # Static libs which becomes added
  COMMAND ${CMAKE_COMMAND} -E copy ${BINARY_DIR}/libcef_dll_wrapper/${CMAKE_STATIC_LIBRARY_PREFIX}cef_dll_wrapper${CMAKE_STATIC_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}

  # Shared libs and parts which need present on add-on
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/share/cef/locales
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR}/Resources/locales                  ${CMAKE_CURRENT_BINARY_DIR}/share/cef/locales
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef.pak                            ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_100_percent.pak                ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_200_percent.pak                ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/cef_extensions.pak                 ${CMAKE_CURRENT_BINARY_DIR}/share/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/devtools_resources.pak             ${CMAKE_CURRENT_BINARY_DIR}/share/cef

  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_SHARED_LIBRARY_PREFIX}cef${CMAKE_SHARED_LIBRARY_SUFFIX} ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/chrome-sandbox           ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/natives_blob.bin         ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/snapshot_blob.bin        ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}/v8_context_snapshot.bin  ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_DIR}/Resources/icudtl.dat                         ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_DIR}/${CMAKE_BUILD_TYPE}                ${CMAKE_CURRENT_BINARY_DIR}/lib/cef
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
                                     src/app/AppRenderer.cpp)

add_executable(kodichromium ${KODICHROMIUM_BIN_SOURCES})
add_dependencies(kodichromium cef-binary)
target_link_libraries(kodichromium ${DEPLIBS} cef dl)


#-------------------------------------------------------------------------------
  add_definitions(-DDEBUG)
  set(CMAKE_BUILD_TYPE Debug)

set(KODICHROMIUM_CUSTOM_DATA "${CMAKE_CURRENT_BINARY_DIR}/share/cef")
set(KODICHROMIUM_ADDITIONAL_BINARY "${CMAKE_CURRENT_BINARY_DIR}/kodichromium"
                                   "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/chrome-sandbox"
                                   "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/libcef.so")
set(KODICHROMIUM_ADDITIONAL_BINARY_PARTS "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/icudtl.dat"
                                         "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/natives_blob.bin"
                                         "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/snapshot_blob.bin"
                                         "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/v8_context_snapshot.bin")
set(KODICHROMIUM_ADDITIONAL_BINARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}/lib/cef/swiftshader")

set(KODICHROMIUM_SOURCES src/addon/Renderer/RendererOpenGL.cpp)
set(KODICHROMIUM_HEADERS src/addon/Renderer/RendererOpenGL.h)

list(APPEND KODICHROMIUM_SOURCES src/addon/addon.cpp
                                 src/addon/AppBrowser.cpp
                                 src/addon/DOMVisitor.cpp
                                 src/addon/MessageIds.cpp
                                 src/addon/Messenger.cpp
                                 src/addon/RequestContextHandler.cpp
                                 src/addon/URICheckHandler.cpp
                                 src/addon/WebBrowserClient.cpp
                                 src/addon/Cookies/CookieHandler.cpp
                                 src/addon/JSInterface/Handler.cpp
                                 src/addon/JSInterface/JSDialogHandler.cpp
                                 src/addon/JSInterface/V8Handler.cpp
                                 src/addon/Renderer/IRenderer.cpp
                                 src/addon/Renderer/Renderer.cpp
                                 src/addon/audio/AudioHandler.cpp
                                 src/addon/gui/DialogDownload.cpp
                                 src/addon/gui/DialogFile.cpp
                                 src/addon/gui/DialogKeyboard.cpp
                                 src/addon/gui/GUIManager.cpp
                                 src/addon/utils/SystemTranslator.cpp
                                 src/addon/utils/Utils.cpp
                                 src/addon/webApp/RenderProcess.cpp)

list(APPEND KODICHROMIUM_HEADERS src/addon/addon.h
                                 src/addon/AppBrowser.h
                                 src/addon/DOMVisitor.h
                                 src/addon/MessageIds.h
                                 src/addon/Messenger.h
                                 src/addon/RequestContextHandler.h
                                 src/addon/URICheckHandler.h
                                 src/addon/WebBrowserClient.h
                                 src/addon/Cookies/CookieHandler.h
                                 src/addon/JSInterface/Handler.h
                                 src/addon/JSInterface/JSDialogHandler.h
                                 src/addon/JSInterface/V8Handler.h
                                 src/addon/Renderer/IRenderer.h
                                 src/addon/Renderer/Renderer.h
                                 src/addon/audio/AudioHandler.h
                                 src/addon/gui/DialogDownload.h
                                 src/addon/gui/DialogFile.h
                                 src/addon/gui/DialogKeyboard.h
                                 src/addon/gui/GUIManager.h
                                 src/addon/utils/SystemTranslator.h
                                 src/addon/utils/Utils.h
                                 src/addon/webApp/RenderProcess.h)

build_addon(web.browser.chromium KODICHROMIUM DEPLIBS)
add_dependencies(web.browser.chromium cef-lib-creator cef-binary kodichromium)
add_definitions(-DKODICHROMIUM_VERSION="${KODICHROMIUM_VERSION}"
                -DLIBRARY_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}"
                -DLIBRARY_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}")


include(CPack)
