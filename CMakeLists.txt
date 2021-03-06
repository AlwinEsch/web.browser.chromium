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

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

if(CORE_SYSTEM_NAME STREQUAL osx)
  set(PROJECT_ARCH "x86_64")
endif()

set(USE_SANDBOX 1)

# Use on addon depends generated CEF dev kit and set CEF_ROOT
set(CEF_ROOT "${ADDON_DEPENDS_PATH}/src/cef")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CMake
                      ${PROJECT_SOURCE_DIR}
                      ${CEF_ROOT}/cmake
                      ${CMAKE_MODULE_PATH})

find_package(Kodi REQUIRED)
find_package(glm REQUIRED)
find_package(CEF REQUIRED)

include(ExternalProject)
include(KitModules)
include(CheckSymbolExists)
include(ArchSetup)

# if CMAKE_BUILD_TYPE is not set fallback to Release as default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

# Include the libcef_dll_wrapper target.
# Comes from the libcef_dll/CMakeLists.txt file in the binary distribution
# directory.
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

PRINT_CEF_CONFIG()

# Logical target used to link the libcef library.
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_RELEASE}" "${CEF_LIB_RELEASE}")

if(CORE_SYSTEM_NAME STREQUAL windows)
  add_definitions(-DHAS_DX)
else()
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

# Include addon depended folders to find files
include_directories(${INCLUDES}
                    ${CEF_INCLUDE_PATH}
                    ${GLM_INCLUDE_DIRS}
                    ${KODI_INCLUDE_DIR}/..
                    ${PROJECT_BINARY_DIR}/cef
                    ${PROJECT_SOURCE_DIR}/src/
                    ${PROJECT_SOURCE_DIR}/src/addon/)

#-------------------------------------------------------------------------------

set(KODICHROMIUM_BIN_SOURCES src/app/AppOther.cpp
                             src/app/renderer/AppRenderer.cpp
                             src/app/renderer/DOMVisitor.cpp
                             src/app/renderer/V8Handler.cpp
                             src/MessageIds.cpp)

if(CORE_SYSTEM_NAME STREQUAL windows)
  list(APPEND DEPLIBS libcef_lib
                      libcef_dll_wrapper
                      ${CEF_STANDARD_LIBS})

  list(APPEND KODICHROMIUM_BIN_SOURCES src/app/main_win.cpp)

  add_executable(kodichromium WIN32 ${KODICHROMIUM_BIN_SOURCES})
  add_dependencies(kodichromium libcef_dll_wrapper)

  SET_EXECUTABLE_TARGET_PROPERTIES(kodichromium)
  target_link_libraries(kodichromium ${DEPLIBS} d3d11.lib)

  if(USE_ATL)
    # Required by VS2013 to link accessibility API functions.
    target_link_libraries(kodichromium oleacc.lib)
  endif()

   # The by addon dll needed additional Sandbox binary application
  list(APPEND KODICHROMIUM_ADDITIONAL_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/kodichromium${CMAKE_EXECUTABLE_SUFFIX})
elseif(CORE_SYSTEM_NAME STREQUAL osx)
  set(CEF_HELPER_TARGET "kodichromium_Helper")
  set(CEF_HELPER_OUTPUT_NAME "kodichromium Helper")

  # Variable referenced from Info.plist files.
  set(PRODUCT_NAME "kodichromium")

  list(APPEND DEPLIBS libcef_dll_wrapper
                      ${CEF_STANDARD_LIBS})

  list(APPEND KODICHROMIUM_BIN_SOURCES src/app/main_mac.cpp)

  add_executable(${CEF_HELPER_TARGET} MACOSX_BUNDLE ${KODICHROMIUM_BIN_SOURCES})
  # Set Xcode target properties.
  set_target_properties(${CEF_HELPER_TARGET} PROPERTIES
    XCODE_ATTRIBUTE_ALWAYS_SEARCH_USER_PATHS                    NO
    XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD                 "gnu++11"   # -std=gnu++11
    XCODE_ATTRIBUTE_CLANG_LINK_OBJC_RUNTIME                     NO          # -fno-objc-link-runtime
    XCODE_ATTRIBUTE_CLANG_WARN_OBJC_MISSING_PROPERTY_SYNTHESIS  YES         # -Wobjc-missing-property-synthesis
    XCODE_ATTRIBUTE_COPY_PHASE_STRIP                            NO
    XCODE_ATTRIBUTE_DEAD_CODE_STRIPPING[variant=Release]        YES         # -Wl,-dead_strip
    XCODE_ATTRIBUTE_GCC_C_LANGUAGE_STANDARD                     "c99"       # -std=c99
    XCODE_ATTRIBUTE_GCC_CW_ASM_SYNTAX                           NO          # No -fasm-blocks
    XCODE_ATTRIBUTE_GCC_DYNAMIC_NO_PIC                          NO
    XCODE_ATTRIBUTE_GCC_ENABLE_CPP_EXCEPTIONS                   YES          # -fno-exceptions
    XCODE_ATTRIBUTE_GCC_ENABLE_CPP_RTTI                         NO          # -fno-rtti
    XCODE_ATTRIBUTE_GCC_ENABLE_PASCAL_STRINGS                   NO          # No -mpascal-strings
    XCODE_ATTRIBUTE_GCC_INLINES_ARE_PRIVATE_EXTERN              YES         # -fvisibility-inlines-hidden
    XCODE_ATTRIBUTE_GCC_OBJC_CALL_CXX_CDTORS                    YES         # -fobjc-call-cxx-cdtors
    XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN                  YES         # -fvisibility=hidden
    XCODE_ATTRIBUTE_GCC_THREADSAFE_STATICS                      NO          # -fno-threadsafe-statics
    XCODE_ATTRIBUTE_GCC_TREAT_WARNINGS_AS_ERRORS                YES         # -Werror
    XCODE_ATTRIBUTE_GCC_VERSION                                 "com.apple.compilers.llvm.clang.1_0"
    XCODE_ATTRIBUTE_GCC_WARN_ABOUT_MISSING_NEWLINE              YES         # -Wnewline-eof
    XCODE_ATTRIBUTE_USE_HEADERMAP                               NO
    OSX_ARCHITECTURES_DEBUG                                     "${CMAKE_OSX_ARCHITECTURES}"
    OSX_ARCHITECTURES_RELEASE                                   "${CMAKE_OSX_ARCHITECTURES}")
  add_dependencies(${CEF_HELPER_TARGET} libcef_dll_wrapper)
  target_link_libraries(${CEF_HELPER_TARGET} ${DEPLIBS})
  set_target_properties(${CEF_HELPER_TARGET} PROPERTIES
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/src/app/mac/helper-Info.plist
    OUTPUT_NAME ${CEF_HELPER_OUTPUT_NAME})

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${CEF_HELPER_TARGET} cef_sandbox_lib)
  endif()

  # Copy files into the main app bundle.
  add_custom_command(
    TARGET ${CEF_HELPER_TARGET}
    POST_BUILD
    # Copy the helper app bundle into the Frameworks directory.
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_BINARY_DIR}/${CEF_HELPER_OUTPUT_NAME}.app"
            "${CMAKE_CURRENT_BINARY_DIR}/Contents/Frameworks/${CEF_HELPER_OUTPUT_NAME}.app"
    # Copy the CEF framework into the Frameworks directory.
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CEF_BINARY_DIR}/Chromium Embedded Framework.framework"
            "${CMAKE_CURRENT_BINARY_DIR}/Contents/Frameworks/Chromium Embedded Framework.framework"
    VERBATIM)
else()
  list(APPEND DEPLIBS libcef_dll_wrapper
                      dl
                      ${CEF_STANDARD_LIBS})

  list(APPEND KODICHROMIUM_BIN_SOURCES src/app/main_linux.cpp)

  add_executable(kodichromium ${KODICHROMIUM_BIN_SOURCES})
  add_dependencies(kodichromium libcef_dll_wrapper)

  target_link_libraries(kodichromium ${DEPLIBS})

   # The by addon dll needed additional Sandbox binary application
  list(APPEND KODICHROMIUM_ADDITIONAL_BINARY ${CMAKE_CURRENT_BINARY_DIR}/kodichromium${CMAKE_EXECUTABLE_SUFFIX})
endif()

#-------------------------------------------------------------------------------

if(CORE_SYSTEM_NAME STREQUAL osx)
  set(KODICHROMIUM_ADDITIONAL_BINARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}/Contents") # To have all Chromium lib parts on addon binary folder
else()
  set(KODICHROMIUM_CUSTOM_DATA ${CEF_ROOT}/Resources/) # TODO: icudtl.dat defined there but currently needed by dll folder
  set(KODICHROMIUM_ADDITIONAL_BINARY_PARTS ${CEF_ROOT}/Resources/icudtl.dat) # icudtl.dat also required on addon binary folder
  set(KODICHROMIUM_ADDITIONAL_BINARY_DIRS ${CEF_ROOT}/Release/) # To have all Chromium lib parts on addon binary folder
endif()

if(${CORE_SYSTEM_NAME} STREQUAL windows)
  set(KODICHROMIUM_SOURCES src/addon/renderer/RendererDX.cpp
                           src/addon/renderer/d3d11.cpp)
  set(KODICHROMIUM_HEADERS src/addon/renderer/RendererDX.h
                           src/addon/renderer/d3d11.h)
else()
  set(KODICHROMIUM_SOURCES src/addon/renderer/RendererGL.cpp)
  set(KODICHROMIUM_HEADERS src/addon/renderer/RendererGL.h)
endif()

list(APPEND KODICHROMIUM_SOURCES src/addon/addon.cpp
                                 src/addon/AppBrowser.cpp
                                 src/addon/ExtensionUtils.cpp
                                 src/addon/PrintHandler.cpp
                                 src/addon/RequestContextHandler.cpp
                                 src/addon/ResourceManager.cpp
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
                                 src/addon/interface/JSException.cpp
                                 src/addon/interface/v8/v8-kodi.cpp
                                 src/addon/renderer/IRenderer.cpp
                                 src/addon/renderer/Renderer.cpp
                                 src/addon/utils/FileUtils.cpp
                                 src/addon/utils/StringUtils.cpp
                                 src/addon/utils/SystemTranslator.cpp
                                 src/addon/utils/Utils.cpp
                                 src/addon/utils/XMLUtils.cpp
                                 src/addon/third_party/tinyxml/tinystr.cpp
                                 src/addon/third_party/tinyxml/tinyxml.cpp
                                 src/addon/third_party/tinyxml/tinyxmlerror.cpp
                                 src/addon/third_party/tinyxml/tinyxmlparser.cpp
                                 src/MessageIds.cpp)

list(APPEND KODICHROMIUM_HEADERS src/addon/addon.h
                                 src/addon/AppBrowser.h
                                 src/addon/ExtensionUtils.h
                                 src/addon/PrintHandler.h
                                 src/addon/RequestContextHandler.h
                                 src/addon/ResourceManager.h
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
                                 src/addon/interface/JSException.h
                                 src/addon/interface/v8/v8-kodi.h
                                 src/addon/renderer/IRenderer.h
                                 src/addon/renderer/Renderer.h
                                 src/addon/utils/FileUtils.h
                                 src/addon/utils/StringUtils.h
                                 src/addon/utils/SystemTranslator.h
                                 src/addon/utils/Utils.h
                                 src/addon/utils/XMLUtils.h
                                 src/addon/third_party/tinyxml/tinystr.h
                                 src/addon/third_party/tinyxml/tinyxml.h
                                 src/MessageIds.h)

build_addon(web.browser.chromium KODICHROMIUM DEPLIBS)

if(${CORE_SYSTEM_NAME} STREQUAL windows)
  SET_LIBRARY_TARGET_PROPERTIES(web.browser.chromium)
endif()

add_dependencies(web.browser.chromium libcef_dll_wrapper)
add_definitions(-DKODICHROMIUM_VERSION="${KODICHROMIUM_VERSION}"
                -DLIBRARY_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}"
                -DLIBRARY_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}")

include(CPack)
