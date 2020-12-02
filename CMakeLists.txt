cmake_minimum_required(VERSION 3.5)
project(web.browser.chromium)

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
find_package(TinyXML REQUIRED)

include(ExternalProject)
include(KitModules)
include(CheckSymbolExists)
include(ArchSetup)
include(CMake/Macros.cmake)

# if CMAKE_BUILD_TYPE is not set fallback to Release as default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT WIN32 AND (APP_RENDER_SYSTEM STREQUAL "gl" OR NOT APP_RENDER_SYSTEM))
  find_package(OpenGl REQUIRED)
  set(DEPLIBS_ADDON ${OPENGL_LIBRARIES})
  set(DEPLIBS_APP ${OPENGL_LIBRARIES})
  set(includes ${OPENGL_INCLUDE_DIR})
  add_definitions(${OPENGL_DEFINITIONS})
else()
  find_package(OpenGLES REQUIRED)
  set(DEPLIBS_ADDON ${OPENGLES_LIBRARIES})
  set(DEPLIBS_APP ${OPENGLES_LIBRARIES})
  set(includes ${OPENGLES_INCLUDE_DIR})
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


# Include the libcef_dll_wrapper target.
# Comes from the libcef_dll/CMakeLists.txt file in the binary distribution
# directory.
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)

add_subdirectory(lib/kodi-dev-kit)

PRINT_CEF_CONFIG()

# Logical target used to link the libcef library.
ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_RELEASE}" "${CEF_LIB_RELEASE}")

include_directories(${KODI_INCLUDE_DIR}/.. # Hack way with "/..", need bigger Kodi cmake rework to match right include ways (becomes done in future)
                    ${includes}
                    ${CEF_INCLUDE_PATH}
                    ${GLM_INCLUDE_DIR}
                    ${TINYXML_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(CORE_SYSTEM_NAME STREQUAL windows)
elseif(CORE_SYSTEM_NAME STREQUAL osx)
else()
  list(APPEND DEPLIBS_APP libcef_dll_wrapper
                          dl
                          ${CEF_STANDARD_LIBS})

   # The by addon dll needed additional Sandbox binary application
  list(APPEND KODICHROMIUM_ADDITIONAL_BINARY ${CMAKE_CURRENT_BINARY_DIR}/kodichromium${CMAKE_EXECUTABLE_SUFFIX})
endif()

if(CORE_SYSTEM_NAME STREQUAL osx)
  set(KODICHROMIUM_ADDITIONAL_BINARY_DIRS "${CMAKE_CURRENT_BINARY_DIR}/Contents") # To have all Chromium lib parts on addon binary folder
else()
  set(KODICHROMIUM_CUSTOM_DATA ${CEF_ROOT}/Resources/) # TODO: icudtl.dat defined there but currently needed by dll folder
  set(KODICHROMIUM_ADDITIONAL_BINARY_PARTS ${CEF_ROOT}/Resources/icudtl.dat) # icudtl.dat also required on addon binary folder
  set(KODICHROMIUM_ADDITIONAL_BINARY_DIRS ${CEF_ROOT}/Release/) # To have all Chromium lib parts on addon binary folder
endif()

#-------------------------------------------------------------------------------

list(APPEND DEPLIBS_APP kodi-dev-kit
                        ${TINYXML_LIBRARIES})

set(CORE_LIBRARY "" CACHE STRING "" FORCE)
mark_as_advanced(CORE_LIBRARY)

add_subdirectory(src/app/app-main/audio)
add_subdirectory(src/app/app-main/certificate)
add_subdirectory(src/app/app-main/gui)
add_subdirectory(src/app/app-main/powerpolicy)
add_subdirectory(src/app/app-main/scheme)
add_subdirectory(src/app/common)

list(APPEND KODICHROMIUM_APP_SOURCES src/app/main_linux.cpp
                                     src/app/LibraryLoader.cpp
                                     src/app/MessageIds.cpp
                                     src/app/app-main/AppBrowser.cpp
                                     src/app/app-main/ExtensionUtils.cpp
                                     src/app/app-main/MainCEFProcess.cpp
                                     src/app/app-main/PrintHandler.cpp
                                     src/app/app-main/RequestContextHandler.cpp
                                     src/app/app-main/ResourceManager.cpp
                                     src/app/app-main/WebBrowserClient.cpp
                                     src/app/app-main/interface/JSException.cpp
                                     src/app/app-main/interface/v8/v8-kodi.cpp
                                     src/app/app-main/renderer/EGLUtil.cpp
                                     src/app/app-main/renderer/IRenderer.cpp
                                     src/app/app-main/renderer/IOffscreenBase.cpp
                                     src/app/app-main/renderer/Renderer.cpp
                                     src/app/app-main/renderer/WorkRendererGL.cpp
                                     src/app/app-other/AppOther.cpp
                                     src/app/app-renderer/AppRenderer.cpp
                                     src/app/app-renderer/DOMVisitor.cpp
                                     src/app/app-renderer/V8Handler.cpp
                                     src/app/utils/FileUtils.cpp
                                     src/app/utils/SystemTranslator.cpp)

list(APPEND KODICHROMIUM_APP_HEADERS src/app/LibraryLoader.h
                                     src/app/MessageIds.h
                                     src/app/app-main/AppBrowser.h
                                     src/app/app-main/ExtensionUtils.h
                                     src/app/app-main/MainCEFProcess.h
                                     src/app/app-main/PrintHandler.h
                                     src/app/app-main/RequestContextHandler.h
                                     src/app/app-main/ResourceManager.h
                                     src/app/app-main/WebBrowserClient.h
                                     src/app/app-main/interface/JSException.h
                                     src/app/app-main/interface/v8/v8-kodi.h
                                     src/app/app-main/renderer/EGLUtil.h
                                     src/app/app-main/renderer/IRenderer.h
                                     src/app/app-main/renderer/IOffscreenBase.h
                                     src/app/app-main/renderer/Renderer.h
                                     src/app/app-main/renderer/WorkRendererGL.h
                                     src/app/app-other/AppOther.h
                                     src/app/app-renderer/AppRenderer.h
                                     src/app/app-renderer/DOMVisitor.h
                                     src/app/app-renderer/V8Handler.h
                                     src/app/utils/FileUtils.h
                                     src/app/utils/SystemTranslator.h)

set(KODICHROMIUM_APP_POSIX_SOURCES src/app/app-main/renderer/OffscreenBasePosix.cpp)

set(KODICHROMIUM_APP_POSIX_HEADERS src/app/app-main/renderer/OffscreenBasePosix.h)

list(APPEND KODICHROMIUM_APP_SOURCES ${KODICHROMIUM_APP_POSIX_SOURCES})
list(APPEND KODICHROMIUM_APP_HEADERS ${KODICHROMIUM_APP_POSIX_HEADERS})

add_executable(kodichromium ${KODICHROMIUM_APP_SOURCES} ${KODICHROMIUM_APP_HEADERS})
target_link_libraries(kodichromium PRIVATE ${CORE_LIBRARY})
target_link_libraries(kodichromium PUBLIC ${DEPLIBS_APP} EGL)

#-------------------------------------------------------------------------------

list(APPEND KODICHROMIUM_SOURCES src/addon/addon.cpp
                                 src/addon/ViewRendererGL.cpp
                                 src/addon/utils/Utils.cpp)

list(APPEND KODICHROMIUM_HEADERS src/addon/addon.h
                                 src/addon/ViewRendererGL.h
                                 src/addon/utils/Utils.h)

list(APPEND DEPLIBS_ADDON kodisandbox-server)

build_addon(web.browser.chromium KODICHROMIUM DEPLIBS_ADDON)

add_dependencies(web.browser.chromium kodisandbox-server)
add_definitions(-DKODICHROMIUM_VERSION="${KODICHROMIUM_VERSION}"
                -DLIBRARY_PREFIX="${CMAKE_SHARED_LIBRARY_PREFIX}"
                -DLIBRARY_SUFFIX="${CMAKE_SHARED_LIBRARY_SUFFIX}"
                -DEXECUTABLE_SUFFIX="${CMAKE_EXECUTABLE_SUFFIX}")

include(CPack)
